/*
 * Copyright (c) 1998-2011 Erez Zadok
 * Copyright (c) 2009	   Shrikar Archak
 * Copyright (c) 2003-2011 Stony Brook University
 * Copyright (c) 2003-2011 The Research Foundation of SUNY
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include "sdcardfs.h"
#include "linux/delay.h"

/* The dentry cache is just so we have properly sized dentries */
static struct kmem_cache *sdcardfs_dentry_cachep;

int sdcardfs_init_dentry_cache(void)
{
	sdcardfs_dentry_cachep =
		kmem_cache_create("sdcardfs_dentry",
				  sizeof(struct sdcardfs_dentry_info),
				  0, SLAB_RECLAIM_ACCOUNT, NULL);

	return sdcardfs_dentry_cachep ? 0 : -ENOMEM;
}

void sdcardfs_destroy_dentry_cache(void)
{
	if (sdcardfs_dentry_cachep)
		kmem_cache_destroy(sdcardfs_dentry_cachep);
}

void free_dentry_private_data(struct dentry *dentry)
{
	if (!dentry || !dentry->d_fsdata)
		return;
	kmem_cache_free(sdcardfs_dentry_cachep, dentry->d_fsdata);
	dentry->d_fsdata = NULL;
}

/* allocate new dentry private data */
int new_dentry_private_data(struct dentry *dentry)
{
	struct sdcardfs_dentry_info *info = SDCARDFS_D(dentry);

	/* use zalloc to init dentry_info.lower_path */
	info = kmem_cache_zalloc(sdcardfs_dentry_cachep, GFP_ATOMIC);
	if (!info)
		return -ENOMEM;

	spin_lock_init(&info->lock);
	dentry->d_fsdata = info;

	return 0;
}

static int sdcardfs_inode_test(struct inode *inode, void *candidate_lower_inode)
{
	struct inode *current_lower_inode = sdcardfs_lower_inode(inode);
	if (current_lower_inode == (struct inode *)candidate_lower_inode)
		return 1; /* found a match */
	else
		return 0; /* no match */
}

static int sdcardfs_inode_set(struct inode *inode, void *lower_inode)
{
	/* we do actual inode initialization in sdcardfs_iget */
	return 0;
}

static struct inode *sdcardfs_iget(struct super_block *sb,
				 struct inode *lower_inode)
{
	struct sdcardfs_inode_info *info;
	struct inode *inode; /* the new inode to return */
	int err;

	/* in order for FAT emulation */
	//struct sdcardfs_sb_info *sb_info = sb->s_fs_info;

	inode = iget5_locked(sb, /* our superblock */
			     /*
			      * hashval: we use inode number, but we can
			      * also use "(unsigned long)lower_inode"
			      * instead.
			      */
			     lower_inode->i_ino, /* hashval */
			     sdcardfs_inode_test,	/* inode comparison function */
			     sdcardfs_inode_set, /* inode init function */
			     lower_inode); /* data passed to test+set fxns */
	if (!inode) {
		err = -EACCES;
		iput(lower_inode);
		return ERR_PTR(err);
	}
	/* if found a cached inode, then just return it */
	if (!(inode->i_state & I_NEW))
		return inode;

	/* initialize new inode */
	info = SDCARDFS_I(inode);

	inode->i_ino = lower_inode->i_ino;
	if (!igrab(lower_inode)) {
		err = -ESTALE;
		return ERR_PTR(err);
	}
	sdcardfs_set_lower_inode(inode, lower_inode);

	inode->i_version++;

	/* use different set of inode ops for symlinks & directories */
	if (S_ISDIR(lower_inode->i_mode))
		inode->i_op = &sdcardfs_dir_iops;
	else if (S_ISLNK(lower_inode->i_mode))
		inode->i_op = &sdcardfs_symlink_iops;
	else
		inode->i_op = &sdcardfs_main_iops;

	/* use different set of file ops for directories */
	if (S_ISDIR(lower_inode->i_mode))
		inode->i_fop = &sdcardfs_dir_fops;
	else
		inode->i_fop = &sdcardfs_main_fops;

	inode->i_mapping->a_ops = &sdcardfs_aops;

	inode->i_atime.tv_sec = 0;
	inode->i_atime.tv_nsec = 0;
	inode->i_mtime.tv_sec = 0;
	inode->i_mtime.tv_nsec = 0;
	inode->i_ctime.tv_sec = 0;
	inode->i_ctime.tv_nsec = 0;

	/* properly initialize special inodes */
	if (S_ISBLK(lower_inode->i_mode) || S_ISCHR(lower_inode->i_mode) ||
	    S_ISFIFO(lower_inode->i_mode) || S_ISSOCK(lower_inode->i_mode))
		init_special_inode(inode, lower_inode->i_mode,
				   lower_inode->i_rdev);

	/* all well, copy inode attributes */
	fsstack_copy_attr_all(inode, lower_inode);
	fsstack_copy_inode_size(inode, lower_inode);

	fix_fat_permission(inode);

	unlock_new_inode(inode);
	return inode;
}

/*
 * Connect a sdcardfs inode dentry/inode with several lower ones.  This is
 * the classic stackable file system "vnode interposition" action.
 *
 * @dentry: sdcardfs's dentry which interposes on lower one
 * @sb: sdcardfs's super_block
 * @lower_path: the lower path (caller does path_get/put)
 */
int sdcardfs_interpose(struct dentry *dentry, struct super_block *sb,
		     struct path *lower_path)
{
	int err = 0;
	struct inode *inode;
	struct inode *lower_inode;
	struct super_block *lower_sb;

	lower_inode = lower_path->dentry->d_inode;
	lower_sb = sdcardfs_lower_super(sb);

	/* check that the lower file system didn't cross a mount point */
	if (lower_inode->i_sb != lower_sb) {
		err = -EXDEV;
		goto out;
	}

	/*
	 * We allocate our new inode below by calling sdcardfs_iget,
	 * which will initialize some of the new inode's fields
	 */

	/* inherit lower inode number for sdcardfs's inode */
	inode = sdcardfs_iget(sb, lower_inode);
	if (IS_ERR(inode)) {
		err = PTR_ERR(inode);
		goto out;
	}

	d_add(dentry, inode);

out:
	return err;
}

#ifdef SDCARDFS_CASE_INSENSITIVE_MATCH_SUPPORT
struct sdcardfs_dirent {
	unsigned long	d_ino;
	unsigned long	d_off;
	unsigned short	d_type;  
	char		d_name[SDCARDFS_DIRENT_SIZE];
};

struct sdcardfs_getdents_callback {
	struct sdcardfs_dirent * dirent;
	int result; 
};

static int sdcardfs_fillonedir(void * __buf, const char * name, int namlen, 
			loff_t offset, u64 ino, unsigned int d_type)
{
	/* This function is copy of fillonedir */
	struct sdcardfs_dirent * dirent;
	struct sdcardfs_getdents_callback * buf;  
	unsigned long d_ino;

	buf = (struct sdcardfs_getdents_callback *) __buf;

	if (buf->result) 
		return -EINVAL; 
	d_ino = ino; 

	if (sizeof(d_ino) < sizeof(ino) && d_ino != ino) {
		buf->result = -EOVERFLOW;
		return -EOVERFLOW;
	}

	buf->result++; 
	dirent = buf->dirent; 

	dirent->d_ino = d_ino;
	dirent->d_off = offset; 
	dirent->d_type = d_type; 
	memcpy(dirent->d_name, name, namlen);
	dirent->d_name[namlen] = 0;

	return 0;
}

static int sdcardfs_getdents(struct file * file, struct sdcardfs_dirent * dirent) 
{
	/* This function is copy old_readdir */
	int error;
	struct sdcardfs_getdents_callback buf;

	error = -EBADF;

	if (!file)
		goto out;

	buf.result = 0; 
	buf.dirent = dirent; 

	error = vfs_readdir(file, sdcardfs_fillonedir, &buf);
	if (buf.result)
		error = buf.result;
out:
	return error;
}

/* 
 * look for case-insensitive matching entry name. 
 * Returns :
 *	- char * of matching entry if found. 
 *	- NULL if no matching entry 
 * 	- ERR_PTR on error 
 *
 */
static void * find_case_insensitive(struct path * lower_parent_path, const char * name)
{
	void * ret = NULL; 
	struct file *lower_dirp = NULL; 
	struct sdcardfs_dirent dirent; 

	int err; 

	/* 
	 * At the end of this function, filp_close or dentry_open (if fails)
	 * will decrease refernce count of lower_parent_path. 
	 * (ie, path->dentry->d_count and path->mnt->mnt_count)
	 * To prevent those counter from dropping to zero, 
	 * we increase the counters in advance. 
	 */
	path_get(lower_parent_path); 

	lower_dirp = dentry_open(lower_parent_path->dentry, 
			lower_parent_path->mnt, O_RDONLY, current_cred()); 
	if (IS_ERR(lower_dirp)) {
		return (void *)lower_dirp; 
	} 

	while (1) {
		err = sdcardfs_getdents(lower_dirp, &dirent);  
		if (likely(err > 0)) { 
			/* we got a direntry */
			if (unlikely(!strcasecmp(dirent.d_name, name))) {
				int len; 
				len = strlen(dirent.d_name) + 1; 
				ret = kmalloc(len, GFP_KERNEL);
				if (ret == NULL) 
					ret = ERR_PTR(-ENOMEM);
				else 
					strcpy(ret, dirent.d_name); 
				break; 
			}
		} else if (err == 0) { 	
			/* end of directory */
			ret = NULL;
			break;
		} else {
			/* err < 0 : error */
			ret = ERR_PTR(err); 
			break;
		} 
	}

	filp_close(lower_dirp, NULL); 
	
	return ret; 
}

#endif

/*
 * Main driver function for sdcardfs's lookup.
 *
 * Returns: NULL (ok), ERR_PTR if an error occurred.
 * Fills in lower_parent_path with <dentry,mnt> on success.
 */
static struct dentry *__sdcardfs_lookup(struct dentry *dentry,
		struct nameidata *nd, struct path *lower_parent_path)
{
	int err = 0;
	struct vfsmount *lower_dir_mnt;
	struct dentry *lower_dir_dentry = NULL;
	struct dentry *lower_dentry;
	const char *name;
	struct path lower_path;
	struct qstr this;

	/* must initialize dentry operations */
	d_set_d_op(dentry, &sdcardfs_dops);

	if (IS_ROOT(dentry))
		goto out;

	name = dentry->d_name.name;

	/* now start the actual lookup procedure */
	lower_dir_dentry = lower_parent_path->dentry;
	lower_dir_mnt = lower_parent_path->mnt;

	/* Use vfs_path_lookup to check if the dentry exists or not */
	err = vfs_path_lookup(lower_dir_dentry, lower_dir_mnt, name, 0,
			&lower_path);

	/* no error: handle positive dentries */
	if (!err) {
#ifdef SDCARDFS_CASE_INSENSITIVE_MATCH_SUPPORT
dentry_found:
#endif
		sdcardfs_set_lower_path(dentry, &lower_path);
		err = sdcardfs_interpose(dentry, dentry->d_sb, &lower_path);
		if (err) /* path_put underlying path on error */
			sdcardfs_put_reset_lower_path(dentry);
		goto out;
	}

#ifdef SDCARDFS_CASE_INSENSITIVE_MATCH_SUPPORT
	if (err == -ENOENT) {
		/* try case insensetive match */
		char * match_name = NULL; 

		match_name = find_case_insensitive(lower_parent_path, name);
		if (unlikely(IS_ERR(match_name))) {
			err = PTR_ERR(match_name); 
		} else if (match_name) {
			/* found */
			err = vfs_path_lookup(lower_dir_dentry, lower_dir_mnt, 
					match_name, 0, &lower_path);
			kfree(match_name); 
			if (!err) 
				goto dentry_found;
		}
		/* no match */
	}
#endif

	/*
	 * We don't consider ENOENT an error, and we want to return a
	 * negative dentry.
	 */
	if (err && err != -ENOENT)
		goto out;

	/* instatiate a new negative dentry */
	this.name = name;
	this.len = strlen(name);
	this.hash = full_name_hash(this.name, this.len);
	lower_dentry = d_lookup(lower_dir_dentry, &this);
	if (lower_dentry)
		goto setup_lower;

	lower_dentry = d_alloc(lower_dir_dentry, &this);
	if (!lower_dentry) {
		err = -ENOMEM;
		goto out;
	}
	d_add(lower_dentry, NULL); /* instantiate and hash */

setup_lower:
	lower_path.dentry = lower_dentry;
	lower_path.mnt = mntget(lower_dir_mnt);
	sdcardfs_set_lower_path(dentry, &lower_path);

	/*
	 * If the intent is to create a file, then don't return an error, so
	 * the VFS will continue the process of making this negative dentry
	 * into a positive one.
	 */
	if (nd) {
		if (nd->flags & (LOOKUP_CREATE|LOOKUP_RENAME_TARGET))
			err = 0;
	} else
		err = 0;

out:
	return ERR_PTR(err);
}

/* 
 * On success:
 * 	fills dentry object appropriate values and returns NULL. 
 * On fail (== error)
 * 	returns error ptr
 *
 * @dir : Parent inode. It is locked (dir->i_mutex)
 * @dentry : Target dentry to lookup. we should set each of fields.
 *	     (dentry->d_name is initialized already)
 * @nd : nameidata of parent inode 
 */
struct dentry *sdcardfs_lookup(struct inode *dir, struct dentry *dentry,
			     struct nameidata *nd)
{
	struct dentry *ret, *parent;
	struct path lower_parent_path;
	int err = 0;

	OVERRIDE_CRED_PTR(SDCARDFS_SB(dir->i_sb));

	parent = dget_parent(dentry);

	sdcardfs_get_lower_path(parent, &lower_parent_path);

	/* allocate dentry private data.  We free it in ->d_release */
	err = new_dentry_private_data(dentry);
	if (err) {
		ret = ERR_PTR(err);
		goto out;
	}

	ret = __sdcardfs_lookup(dentry, nd, &lower_parent_path);
	if (IS_ERR(ret))
	{
		goto out;
	}
	if (ret) 
		dentry = ret;
	if (dentry->d_inode)
		fsstack_copy_attr_times(dentry->d_inode,
					sdcardfs_lower_inode(dentry->d_inode));
	/* update parent directory's atime */
	fsstack_copy_attr_atime(parent->d_inode,
				sdcardfs_lower_inode(parent->d_inode));

out:
	sdcardfs_put_lower_path(parent, &lower_parent_path);
	dput(parent);
	REVERT_CRED();
	return ret;
}

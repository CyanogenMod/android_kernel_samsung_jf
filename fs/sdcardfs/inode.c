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

/* Do not directly use this function. Use OVERRIDE_CRED() instead. */
const struct cred * override_fsids(struct sdcardfs_sb_info* sbi)
{
	struct cred * cred; 
	const struct cred * old_cred; 

	cred = prepare_creds(); 
	if (!cred) 
		return NULL; 

	cred->fsuid = sbi->options.fs_low_uid;
	cred->fsgid = sbi->options.fs_low_gid; 

	old_cred = override_creds(cred); 

	return old_cred; 
}

/* Do not directly use this function, use REVERT_CRED() instead. */
void revert_fsids(const struct cred * old_cred)
{
	const struct cred * cur_cred; 

	cur_cred = current->cred; 
	revert_creds(old_cred); 
	put_cred(cur_cred); 
}

static int sdcardfs_create(struct inode *dir, struct dentry *dentry,
			 umode_t mode, struct nameidata *nd)
{
	int err = 0;
	struct dentry *lower_dentry;
	struct dentry *lower_parent_dentry = NULL;
	struct path lower_path, saved_path;


	OVERRIDE_CRED(SDCARDFS_SB(dir->i_sb));

	sdcardfs_get_lower_path(dentry, &lower_path);
	lower_dentry = lower_path.dentry;
	lower_parent_dentry = lock_parent(lower_dentry);

	err = mnt_want_write(lower_path.mnt);
	if (err)
		goto out_unlock;

	pathcpy(&saved_path, &nd->path);
	pathcpy(&nd->path, &lower_path);

	/* set last 16bytes of mode field to 0664 */
	mode = (mode & S_IFMT) | 00664; 
	err = vfs_create(lower_parent_dentry->d_inode, lower_dentry, mode, nd);

	pathcpy(&nd->path, &saved_path);
	if (err)
		goto out;

	err = sdcardfs_interpose(dentry, dir->i_sb, &lower_path);
	if (err)
		goto out;
	fsstack_copy_attr_times(dir, sdcardfs_lower_inode(dir));
	fsstack_copy_inode_size(dir, lower_parent_dentry->d_inode);

out:
	mnt_drop_write(lower_path.mnt);
out_unlock:
	unlock_dir(lower_parent_dentry);
	sdcardfs_put_lower_path(dentry, &lower_path);
	REVERT_CRED();
	return err;
}

#if 0
static int sdcardfs_link(struct dentry *old_dentry, struct inode *dir,
		       struct dentry *new_dentry)
{
	struct dentry *lower_old_dentry;
	struct dentry *lower_new_dentry;
	struct dentry *lower_dir_dentry;
	u64 file_size_save;
	int err;
	struct path lower_old_path, lower_new_path;

	OVERRIDE_CRED(SDCARDFS_SB(dir->i_sb));
	
	file_size_save = i_size_read(old_dentry->d_inode);
	sdcardfs_get_lower_path(old_dentry, &lower_old_path);
	sdcardfs_get_lower_path(new_dentry, &lower_new_path);
	lower_old_dentry = lower_old_path.dentry;
	lower_new_dentry = lower_new_path.dentry;
	lower_dir_dentry = lock_parent(lower_new_dentry);

	err = mnt_want_write(lower_new_path.mnt);
	if (err)
		goto out_unlock;

	err = vfs_link(lower_old_dentry, lower_dir_dentry->d_inode,
		       lower_new_dentry);
	if (err || !lower_new_dentry->d_inode)
		goto out;

	err = sdcardfs_interpose(new_dentry, dir->i_sb, &lower_new_path);
	if (err)
		goto out;
	fsstack_copy_attr_times(dir, lower_new_dentry->d_inode);
	fsstack_copy_inode_size(dir, lower_new_dentry->d_inode);
	set_nlink(old_dentry->d_inode,
		  sdcardfs_lower_inode(old_dentry->d_inode)->i_nlink);
	i_size_write(new_dentry->d_inode, file_size_save);
out:
	mnt_drop_write(lower_new_path.mnt);
out_unlock:
	unlock_dir(lower_dir_dentry);
	sdcardfs_put_lower_path(old_dentry, &lower_old_path);
	sdcardfs_put_lower_path(new_dentry, &lower_new_path);
	REVERT_CRED();
	return err;
}
#endif

static int sdcardfs_unlink(struct inode *dir, struct dentry *dentry)
{
	int err;
	struct dentry *lower_dentry;
	struct inode *lower_dir_inode = sdcardfs_lower_inode(dir);
	struct dentry *lower_dir_dentry;
	struct path lower_path;

	OVERRIDE_CRED(SDCARDFS_SB(dir->i_sb));

	sdcardfs_get_lower_path(dentry, &lower_path);
	lower_dentry = lower_path.dentry;
	dget(lower_dentry);
	lower_dir_dentry = lock_parent(lower_dentry);

	err = mnt_want_write(lower_path.mnt);
	if (err)
		goto out_unlock;
	err = vfs_unlink(lower_dir_inode, lower_dentry);

	/*
	 * Note: unlinking on top of NFS can cause silly-renamed files.
	 * Trying to delete such files results in EBUSY from NFS
	 * below.  Silly-renamed files will get deleted by NFS later on, so
	 * we just need to detect them here and treat such EBUSY errors as
	 * if the upper file was successfully deleted.
	 */
	if (err == -EBUSY && lower_dentry->d_flags & DCACHE_NFSFS_RENAMED)
		err = 0;
	if (err)
		goto out;
	fsstack_copy_attr_times(dir, lower_dir_inode);
	fsstack_copy_inode_size(dir, lower_dir_inode);
	set_nlink(dentry->d_inode,
		  sdcardfs_lower_inode(dentry->d_inode)->i_nlink);
	dentry->d_inode->i_ctime = dir->i_ctime;
	d_drop(dentry); /* this is needed, else LTP fails (VFS won't do it) */
out:
	mnt_drop_write(lower_path.mnt);
out_unlock:
	unlock_dir(lower_dir_dentry);
	dput(lower_dentry);
	sdcardfs_put_lower_path(dentry, &lower_path);
	REVERT_CRED();
	return err;
}

#if 0
static int sdcardfs_symlink(struct inode *dir, struct dentry *dentry,
			  const char *symname)
{
	int err = 0;
	struct dentry *lower_dentry;
	struct dentry *lower_parent_dentry = NULL;
	struct path lower_path;

	OVERRIDE_CRED(SDCARDFS_SB(dir->i_sb));

	sdcardfs_get_lower_path(dentry, &lower_path);
	lower_dentry = lower_path.dentry;
	lower_parent_dentry = lock_parent(lower_dentry);

	err = mnt_want_write(lower_path.mnt);
	if (err)
		goto out_unlock;
	err = vfs_symlink(lower_parent_dentry->d_inode, lower_dentry, symname);
	if (err)
		goto out;
	err = sdcardfs_interpose(dentry, dir->i_sb, &lower_path);
	if (err)
		goto out;
	fsstack_copy_attr_times(dir, sdcardfs_lower_inode(dir));
	fsstack_copy_inode_size(dir, lower_parent_dentry->d_inode);

out:
	mnt_drop_write(lower_path.mnt);
out_unlock:
	unlock_dir(lower_parent_dentry);
	sdcardfs_put_lower_path(dentry, &lower_path);
	REVERT_CRED();
	return err;
}
#endif

static int sdcardfs_mkdir(struct inode *dir, struct dentry *dentry, umode_t mode)
{
	int err = 0;
	struct dentry *lower_dentry;
	struct dentry *lower_parent_dentry = NULL;
	struct path lower_path;

	OVERRIDE_CRED(SDCARDFS_SB(dir->i_sb));

#if defined(LOWER_FS_MIN_FREE_SIZE)
	/* check disk space */
	if (!check_min_free_space(dentry, 0, 1)) {
		REVERT_CRED();
		printk(KERN_INFO "No minimum free space.\n");
		return -ENOSPC;
	}
#endif
	sdcardfs_get_lower_path(dentry, &lower_path);
	lower_dentry = lower_path.dentry;
	lower_parent_dentry = lock_parent(lower_dentry);

	err = mnt_want_write(lower_path.mnt);
	if (err)
		goto out_unlock;

	/* set last 16bytes of mode field to 0775 */
	mode = (mode & S_IFMT) | 00775; 
	err = vfs_mkdir(lower_parent_dentry->d_inode, lower_dentry, mode);

	if (err)
		goto out;

	err = sdcardfs_interpose(dentry, dir->i_sb, &lower_path);
	if (err)
		goto out;

	fsstack_copy_attr_times(dir, sdcardfs_lower_inode(dir));
	fsstack_copy_inode_size(dir, lower_parent_dentry->d_inode);
	/* update number of links on parent directory */
	set_nlink(dir, sdcardfs_lower_inode(dir)->i_nlink);

out:
	mnt_drop_write(lower_path.mnt);
out_unlock:
	unlock_dir(lower_parent_dentry);
	sdcardfs_put_lower_path(dentry, &lower_path);
	REVERT_CRED();
	return err;
}

static int sdcardfs_rmdir(struct inode *dir, struct dentry *dentry)
{
	struct dentry *lower_dentry;
	struct dentry *lower_dir_dentry;
	int err;
	struct path lower_path;

	OVERRIDE_CRED(SDCARDFS_SB(dir->i_sb));

	sdcardfs_get_lower_path(dentry, &lower_path);
	lower_dentry = lower_path.dentry;
	lower_dir_dentry = lock_parent(lower_dentry);

	err = mnt_want_write(lower_path.mnt);
	if (err)
		goto out_unlock;
	err = vfs_rmdir(lower_dir_dentry->d_inode, lower_dentry);
	if (err)
		goto out;

	d_drop(dentry);	/* drop our dentry on success (why not VFS's job?) */
	if (dentry->d_inode)
		clear_nlink(dentry->d_inode);
	fsstack_copy_attr_times(dir, lower_dir_dentry->d_inode);
	fsstack_copy_inode_size(dir, lower_dir_dentry->d_inode);
	set_nlink(dir, lower_dir_dentry->d_inode->i_nlink);

out:
	mnt_drop_write(lower_path.mnt);
out_unlock:
	unlock_dir(lower_dir_dentry);
	sdcardfs_put_lower_path(dentry, &lower_path);
	REVERT_CRED();
	return err;
}

#if 0
static int sdcardfs_mknod(struct inode *dir, struct dentry *dentry, umode_t mode,
			dev_t dev)
{
	int err = 0;
	struct dentry *lower_dentry;
	struct dentry *lower_parent_dentry = NULL;
	struct path lower_path;

	OVERRIDE_CRED(SDCARDFS_SB(dir->i_sb));

	sdcardfs_get_lower_path(dentry, &lower_path);
	lower_dentry = lower_path.dentry;
	lower_parent_dentry = lock_parent(lower_dentry);

	err = mnt_want_write(lower_path.mnt);
	if (err)
		goto out_unlock;
	err = vfs_mknod(lower_parent_dentry->d_inode, lower_dentry, mode, dev);
	if (err)
		goto out;

	err = sdcardfs_interpose(dentry, dir->i_sb, &lower_path);
	if (err)
		goto out;
	fsstack_copy_attr_times(dir, sdcardfs_lower_inode(dir));
	fsstack_copy_inode_size(dir, lower_parent_dentry->d_inode);

out:
	mnt_drop_write(lower_path.mnt);
out_unlock:
	unlock_dir(lower_parent_dentry);
	sdcardfs_put_lower_path(dentry, &lower_path);
	REVERT_CRED();
	return err;
}
#endif

/*
 * The locking rules in sdcardfs_rename are complex.  We could use a simpler
 * superblock-level name-space lock for renames and copy-ups.
 */
static int sdcardfs_rename(struct inode *old_dir, struct dentry *old_dentry,
			 struct inode *new_dir, struct dentry *new_dentry)
{
	int err = 0;
	struct dentry *lower_old_dentry = NULL;
	struct dentry *lower_new_dentry = NULL;
	struct dentry *lower_old_dir_dentry = NULL;
	struct dentry *lower_new_dir_dentry = NULL;
	struct dentry *trap = NULL;
	struct path lower_old_path, lower_new_path;

	OVERRIDE_CRED(SDCARDFS_SB(old_dir->i_sb));

	sdcardfs_get_lower_path(old_dentry, &lower_old_path);
	sdcardfs_get_lower_path(new_dentry, &lower_new_path);
	lower_old_dentry = lower_old_path.dentry;
	lower_new_dentry = lower_new_path.dentry;
	lower_old_dir_dentry = dget_parent(lower_old_dentry);
	lower_new_dir_dentry = dget_parent(lower_new_dentry);

	trap = lock_rename(lower_old_dir_dentry, lower_new_dir_dentry);
	/* source should not be ancestor of target */
	if (trap == lower_old_dentry) {
		err = -EINVAL;
		goto out;
	}
	/* target should not be ancestor of source */
	if (trap == lower_new_dentry) {
		err = -ENOTEMPTY;
		goto out;
	}

	err = mnt_want_write(lower_old_path.mnt);
	if (err)
		goto out;
	err = mnt_want_write(lower_new_path.mnt);
	if (err)
		goto out_drop_old_write;

	err = vfs_rename(lower_old_dir_dentry->d_inode, lower_old_dentry,
			 lower_new_dir_dentry->d_inode, lower_new_dentry);
	if (err)
		goto out_err;

	/* Copy attrs from lower dir, but i_uid/i_gid */
	fsstack_copy_attr_all(new_dir, lower_new_dir_dentry->d_inode);
	fsstack_copy_inode_size(new_dir, lower_new_dir_dentry->d_inode);
	fix_fat_permission(new_dir); 
	if (new_dir != old_dir) {
		fsstack_copy_attr_all(old_dir, lower_old_dir_dentry->d_inode);
		fsstack_copy_inode_size(old_dir, lower_old_dir_dentry->d_inode);
		fix_fat_permission(old_dir); 
	}

out_err:
	mnt_drop_write(lower_new_path.mnt);
out_drop_old_write:
	mnt_drop_write(lower_old_path.mnt);
out:
	unlock_rename(lower_old_dir_dentry, lower_new_dir_dentry);
	dput(lower_old_dir_dentry);
	dput(lower_new_dir_dentry);
	sdcardfs_put_lower_path(old_dentry, &lower_old_path);
	sdcardfs_put_lower_path(new_dentry, &lower_new_path);
	REVERT_CRED();
	return err;
}

#if 0
static int sdcardfs_readlink(struct dentry *dentry, char __user *buf, int bufsiz)
{
	int err;
	struct dentry *lower_dentry;
	struct path lower_path;
	/* XXX readlink does not requires overriding credential */

	sdcardfs_get_lower_path(dentry, &lower_path);
	lower_dentry = lower_path.dentry;
	if (!lower_dentry->d_inode->i_op ||
	    !lower_dentry->d_inode->i_op->readlink) {
		err = -EINVAL;
		goto out;
	}

	err = lower_dentry->d_inode->i_op->readlink(lower_dentry,
						    buf, bufsiz);
	if (err < 0)
		goto out;
	fsstack_copy_attr_atime(dentry->d_inode, lower_dentry->d_inode);

out:
	sdcardfs_put_lower_path(dentry, &lower_path);
	return err;
}
#endif

#if 0
static void *sdcardfs_follow_link(struct dentry *dentry, struct nameidata *nd)
{
	char *buf;
	int len = PAGE_SIZE, err;
	mm_segment_t old_fs;

	/* This is freed by the put_link method assuming a successful call. */
	buf = kmalloc(len, GFP_KERNEL);
	if (!buf) {
		buf = ERR_PTR(-ENOMEM);
		goto out;
	}

	/* read the symlink, and then we will follow it */
	old_fs = get_fs();
	set_fs(KERNEL_DS);
	err = sdcardfs_readlink(dentry, buf, len);
	set_fs(old_fs);
	if (err < 0) {
		kfree(buf);
		buf = ERR_PTR(err);
	} else {
		buf[err] = '\0';
	}
out:
	nd_set_link(nd, buf);
	return NULL;
}
#endif

#if 0
/* this @nd *IS* still used */
static void sdcardfs_put_link(struct dentry *dentry, struct nameidata *nd,
			    void *cookie)
{
	char *buf = nd_get_link(nd);
	if (!IS_ERR(buf))	/* free the char* */
		kfree(buf);
}
#endif

static int sdcardfs_permission(struct inode *inode, int mask)
{
	int err;

	/*
	 * Permission check on sdcardfs inode.
	 * Calling process should have AID_SDCARD_RW permission 
	 */
	err = generic_permission(inode, mask);

	/* XXX 
	 * Original sdcardfs code calls inode_permission(lower_inode,.. )
	 * for checking inode permission. But doing such things here seems
	 * duplicated work, because the functions called after this func, 
	 * such as vfs_create, vfs_unlink, vfs_rename, and etc, 
	 * does exactly same thing, i.e., they calls inode_permission(). 
	 * So we just let they do the things. 
	 * If there are any security hole, just uncomment following if block. 
	 */
#if 0
	if (!err) {
		/* 
		 * Permission check on lower_inode(=EXT4). 
		 * we check it with AID_MEDIA_RW permission 
		 */
		struct inode *lower_inode;
		OVERRIDE_CRED(SDCARDFS_SB(inode->sb));

		lower_inode = sdcardfs_lower_inode(inode);
		err = inode_permission(lower_inode, mask);

		REVERT_CRED();
	}
#endif
	return err; 

}

static int sdcardfs_getattr(struct vfsmount *mnt, struct dentry *dentry,
		 struct kstat *stat)
{
	struct dentry *lower_dentry;
	struct inode *inode;
	struct inode *lower_inode;
	struct path lower_path;

	inode = dentry->d_inode;

	sdcardfs_get_lower_path(dentry, &lower_path);
	lower_dentry = lower_path.dentry;
	lower_inode = sdcardfs_lower_inode(inode);

	fsstack_copy_attr_all(inode, lower_inode);
	fix_fat_permission(inode); 

	generic_fillattr(inode, stat);
	sdcardfs_put_lower_path(dentry, &lower_path);
	return 0;
}

static int sdcardfs_setattr(struct dentry *dentry, struct iattr *ia)
{
	int err = 0;
	struct dentry *lower_dentry;
	struct inode *inode;
	struct inode *lower_inode;
	struct path lower_path;
	struct iattr lower_ia;

	inode = dentry->d_inode;

	/*
	 * Check if user has permission to change inode.  We don't check if
	 * this user can change the lower inode: that should happen when
	 * calling notify_change on the lower inode.
	 */
	err = inode_change_ok(inode, ia);
	if (err)
		goto out_err;

	sdcardfs_get_lower_path(dentry, &lower_path);
	lower_dentry = lower_path.dentry;
	lower_inode = sdcardfs_lower_inode(inode);


	/* fix warpfs file owner and permission. cannot change them. */ 
	ia->ia_uid = AID_ROOT; 
	ia->ia_gid = AID_SDCARD_RW; 
	fix_mode(ia->ia_mode); 

	/* prepare our own lower struct iattr (with the lower file) */
	memcpy(&lower_ia, ia, sizeof(lower_ia));
	if (ia->ia_valid & ATTR_FILE)
		lower_ia.ia_file = sdcardfs_lower_file(ia->ia_file);

	lower_ia.ia_uid = AID_MEDIA_RW;
	lower_ia.ia_gid = AID_MEDIA_RW;
	fix_mode(lower_ia.ia_mode); 

	/*
	 * If shrinking, first truncate upper level to cancel writing dirty
	 * pages beyond the new eof; and also if its' maxbytes is more
	 * limiting (fail with -EFBIG before making any change to the lower
	 * level).  There is no need to vmtruncate the upper level
	 * afterwards in the other cases: we fsstack_copy_inode_size from
	 * the lower level.
	 */
	if (ia->ia_valid & ATTR_SIZE) {
		err = inode_newsize_ok(inode, ia->ia_size);
		if (err)
			goto out;
		truncate_setsize(inode, ia->ia_size);
	}

	/* for FAT emulation */
	/*
	 * mode change is for clearing setuid/setgid bits. Allow lower fs
	 * to interpret this in its own way.
	 */
	if (lower_ia.ia_valid & (ATTR_KILL_SUID | ATTR_KILL_SGID))
		lower_ia.ia_valid &= ~ATTR_MODE;

	/* notify the (possibly copied-up) lower inode */
	/*
	 * Note: we use lower_dentry->d_inode, because lower_inode may be
	 * unlinked (no inode->i_sb and i_ino==0.  This happens if someone
	 * tries to open(), unlink(), then ftruncate() a file.
	 */
	mutex_lock(&lower_dentry->d_inode->i_mutex);
	err = notify_change(lower_dentry, &lower_ia); /* note: lower_ia */
	mutex_unlock(&lower_dentry->d_inode->i_mutex);
	if (err)
		goto out;

	/* get attributes from the lower inode */
	fsstack_copy_attr_all(inode, lower_inode);
	fix_fat_permission(inode); 
	
	/*
	 * Not running fsstack_copy_inode_size(inode, lower_inode), because
	 * VFS should update our inode size, and notify_change on
	 * lower_inode should update its size.
	 */

out:
	sdcardfs_put_lower_path(dentry, &lower_path);
out_err:
	return err;
}

const struct inode_operations sdcardfs_symlink_iops = {
	.permission	= sdcardfs_permission,
	.setattr	= sdcardfs_setattr,
	/* XXX Following operations are implemented, 
	 *     but FUSE(sdcard) or FAT does not support them
	 *     These methods are *NOT* perfectly tested. 
	.readlink	= sdcardfs_readlink, 
	.follow_link	= sdcardfs_follow_link, 
	.put_link	= sdcardfs_put_link, 
	 */
};

const struct inode_operations sdcardfs_dir_iops = {
	.create		= sdcardfs_create,
	.lookup		= sdcardfs_lookup,
	.permission	= sdcardfs_permission,
	.unlink		= sdcardfs_unlink,
	.mkdir		= sdcardfs_mkdir,
	.rmdir		= sdcardfs_rmdir,
	.rename		= sdcardfs_rename,
	.setattr	= sdcardfs_setattr,
	.getattr	= sdcardfs_getattr,
	/* XXX Following operations are implemented, 
	 *     but FUSE(sdcard) or FAT does not support them
	 *     These methods are *NOT* perfectly tested. 
	.symlink	= sdcardfs_symlink, 
	.link		= sdcardfs_link, 
	.mknod		= sdcardfs_mknod, 
	 */
};

const struct inode_operations sdcardfs_main_iops = {
	.permission	= sdcardfs_permission, 
	.setattr	= sdcardfs_setattr, 
	.getattr	= sdcardfs_getattr,
};

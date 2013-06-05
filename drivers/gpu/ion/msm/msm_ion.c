/* Copyright (c) 2011-2012, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/export.h>
#include <linux/err.h>
#include <linux/msm_ion.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/memory_alloc.h>
#include <linux/fmem.h>
#include <linux/of.h>
#include <linux/mm.h>
#include <linux/mm_types.h>
#include <linux/sched.h>
#include <linux/rwsem.h>
#include <linux/uaccess.h>
#include <mach/ion.h>
#include <mach/msm_memtypes.h>
#include "../ion_priv.h"
#include "ion_cp_common.h"

#define ION_COMPAT_STR	"qcom,msm-ion"
#define ION_COMPAT_MEM_RESERVE_STR "qcom,msm-ion-reserve"

static struct ion_device *idev;
static int num_heaps;
static struct ion_heap **heaps;

struct ion_heap_desc {
	unsigned int id;
	enum ion_heap_type type;
	const char *name;
	unsigned int permission_type;
};


static struct ion_heap_desc ion_heap_meta[] = {
	{
		.id	= ION_SYSTEM_HEAP_ID,
		.type	= ION_HEAP_TYPE_SYSTEM,
		.name	= ION_VMALLOC_HEAP_NAME,
	},
	{
		.id	= ION_SYSTEM_CONTIG_HEAP_ID,
		.type	= ION_HEAP_TYPE_SYSTEM_CONTIG,
		.name	= ION_KMALLOC_HEAP_NAME,
	},
	{
		.id	= ION_CP_MM_HEAP_ID,
		.type	= ION_HEAP_TYPE_CP,
		.name	= ION_MM_HEAP_NAME,
		.permission_type = IPT_TYPE_MM_CARVEOUT,
	},
	{
		.id	= ION_MM_FIRMWARE_HEAP_ID,
		.type	= ION_HEAP_TYPE_CARVEOUT,
		.name	= ION_MM_FIRMWARE_HEAP_NAME,
	},
	{
		.id	= ION_CP_MFC_HEAP_ID,
		.type	= ION_HEAP_TYPE_CP,
		.name	= ION_MFC_HEAP_NAME,
		.permission_type = IPT_TYPE_MFC_SHAREDMEM,
	},
	{
		.id	= ION_SF_HEAP_ID,
		.type	= ION_HEAP_TYPE_CARVEOUT,
		.name	= ION_SF_HEAP_NAME,
	},
	{
		.id	= ION_IOMMU_HEAP_ID,
		.type	= ION_HEAP_TYPE_IOMMU,
		.name	= ION_IOMMU_HEAP_NAME,
	},
	{
		.id	= ION_QSECOM_HEAP_ID,
		.type	= ION_HEAP_TYPE_CARVEOUT,
		.name	= ION_QSECOM_HEAP_NAME,
	},
	{
		.id	= ION_AUDIO_HEAP_ID,
		.type	= ION_HEAP_TYPE_CARVEOUT,
		.name	= ION_AUDIO_HEAP_NAME,
	},
	{
		.id	= ION_CP_WB_HEAP_ID,
		.type	= ION_HEAP_TYPE_CP,
		.name	= ION_WB_HEAP_NAME,
	},
	{
		.id	= ION_CAMERA_HEAP_ID,
		.type	= ION_HEAP_TYPE_CARVEOUT,
		.name	= ION_CAMERA_HEAP_NAME,
	},
};

struct ion_client *msm_ion_client_create(unsigned int heap_mask,
					const char *name)
{
	return ion_client_create(idev, heap_mask, name);
}
EXPORT_SYMBOL(msm_ion_client_create);

int msm_ion_secure_heap(int heap_id)
{
	return ion_secure_heap(idev, heap_id, ION_CP_V1, NULL);
}
EXPORT_SYMBOL(msm_ion_secure_heap);

int msm_ion_unsecure_heap(int heap_id)
{
	return ion_unsecure_heap(idev, heap_id, ION_CP_V1, NULL);
}
EXPORT_SYMBOL(msm_ion_unsecure_heap);

int msm_ion_secure_heap_2_0(int heap_id, enum cp_mem_usage usage)
{
	return ion_secure_heap(idev, heap_id, ION_CP_V2, (void *)usage);
}
EXPORT_SYMBOL(msm_ion_secure_heap_2_0);

int msm_ion_unsecure_heap_2_0(int heap_id, enum cp_mem_usage usage)
{
	return ion_unsecure_heap(idev, heap_id, ION_CP_V2, (void *)usage);
}
EXPORT_SYMBOL(msm_ion_unsecure_heap_2_0);

int msm_ion_do_cache_op(struct ion_client *client, struct ion_handle *handle,
			void *vaddr, unsigned long len, unsigned int cmd)
{
	return ion_do_cache_op(client, handle, vaddr, 0, len, cmd);
}
EXPORT_SYMBOL(msm_ion_do_cache_op);

static unsigned long msm_ion_get_base(unsigned long size, int memory_type,
				    unsigned int align)
{
	switch (memory_type) {
	case ION_EBI_TYPE:
		return allocate_contiguous_ebi_nomap(size, align);
		break;
	case ION_SMI_TYPE:
		return allocate_contiguous_memory_nomap(size, MEMTYPE_SMI,
							align);
		break;
	default:
		pr_err("%s: Unknown memory type %d\n", __func__, memory_type);
		return 0;
	}
}

static struct ion_platform_heap *find_heap(const struct ion_platform_heap
					   heap_data[],
					   unsigned int nr_heaps,
					   int heap_id)
{
	unsigned int i;
	for (i = 0; i < nr_heaps; ++i) {
		const struct ion_platform_heap *heap = &heap_data[i];
		if (heap->id == heap_id)
			return (struct ion_platform_heap *) heap;
	}
	return 0;
}

static void ion_set_base_address(struct ion_platform_heap *heap,
			    struct ion_platform_heap *shared_heap,
			    struct ion_co_heap_pdata *co_heap_data,
			    struct ion_cp_heap_pdata *cp_data)
{
	if (cp_data->reusable) {
		const struct fmem_data *fmem_info = fmem_get_info();

		if (!fmem_info) {
			pr_err("fmem info pointer NULL!\n");
			BUG();
		}

		heap->base = fmem_info->phys - fmem_info->reserved_size_low;
		cp_data->virt_addr = fmem_info->virt;
		pr_info("ION heap %s using FMEM\n", shared_heap->name);
	} else {
		heap->base = msm_ion_get_base(heap->size + shared_heap->size,
						shared_heap->memory_type,
						co_heap_data->align);
	}
	if (heap->base) {
		shared_heap->base = heap->base + heap->size;
		cp_data->secure_base = heap->base;
		cp_data->secure_size = heap->size + shared_heap->size;
	} else {
		pr_err("%s: could not get memory for heap %s (id %x)\n",
			__func__, heap->name, heap->id);
	}
}

static void allocate_co_memory(struct ion_platform_heap *heap,
			       struct ion_platform_heap heap_data[],
			       unsigned int nr_heaps)
{
	struct ion_co_heap_pdata *co_heap_data =
		(struct ion_co_heap_pdata *) heap->extra_data;

	if (co_heap_data->adjacent_mem_id != INVALID_HEAP_ID) {
		struct ion_platform_heap *shared_heap =
			find_heap(heap_data, nr_heaps,
				  co_heap_data->adjacent_mem_id);
		if (shared_heap) {
			struct ion_cp_heap_pdata *cp_data =
			   (struct ion_cp_heap_pdata *) shared_heap->extra_data;
			if (cp_data->mem_is_fmem && cp_data->fixed_position == FIXED_MIDDLE) {
				const struct fmem_data *fmem_info =
					fmem_get_info();

				if (!fmem_info) {
					pr_err("fmem info pointer NULL!\n");
					BUG();
				}

				cp_data->virt_addr = fmem_info->virt;
				if (!cp_data->secure_base) {
					cp_data->secure_base = heap->base;
					cp_data->secure_size =
						heap->size + shared_heap->size;
				}
			} else if (!heap->base) {
				ion_set_base_address(heap, shared_heap,
					co_heap_data, cp_data);
			}
		}
	}
}

/* Fixup heaps in board file to support two heaps being adjacent to each other.
 * A flag (adjacent_mem_id) in the platform data tells us that the heap phy
 * memory location must be adjacent to the specified heap. We do this by
 * carving out memory for both heaps and then splitting up the memory to the
 * two heaps. The heap specifying the "adjacent_mem_id" get the base of the
 * memory while heap specified in "adjacent_mem_id" get base+size as its
 * base address.
 * Note: Modifies platform data and allocates memory.
 */
static void msm_ion_heap_fixup(struct ion_platform_heap heap_data[],
			       unsigned int nr_heaps)
{
	unsigned int i;

	for (i = 0; i < nr_heaps; i++) {
		struct ion_platform_heap *heap = &heap_data[i];
		if (heap->type == ION_HEAP_TYPE_CARVEOUT) {
			if (heap->extra_data)
				allocate_co_memory(heap, heap_data, nr_heaps);
		}
	}
}

static void msm_ion_allocate(struct ion_platform_heap *heap)
{

	if (!heap->base && heap->extra_data) {
		unsigned int align = 0;
		switch ((int) heap->type) {
		case ION_HEAP_TYPE_CARVEOUT:
			align =
			((struct ion_co_heap_pdata *) heap->extra_data)->align;
			break;
		case ION_HEAP_TYPE_CP:
		{
			struct ion_cp_heap_pdata *data =
				(struct ion_cp_heap_pdata *)
				heap->extra_data;
			if (data->reusable) {
				const struct fmem_data *fmem_info =
					fmem_get_info();
				heap->base = fmem_info->phys;
				data->virt_addr = fmem_info->virt;
				pr_info("ION heap %s using FMEM\n", heap->name);
			} else if (data->mem_is_fmem) {
				const struct fmem_data *fmem_info =
					fmem_get_info();
				heap->base = fmem_info->phys + fmem_info->size;
			}
			align = data->align;
			break;
		}
		default:
			break;
		}
		if (align && !heap->base) {
			heap->base = msm_ion_get_base(heap->size,
						      heap->memory_type,
						      align);
			if (!heap->base)
				pr_err("%s: could not get memory for heap %s "
				   "(id %x)\n", __func__, heap->name, heap->id);
		}
	}
}

static int is_heap_overlapping(const struct ion_platform_heap *heap1,
				const struct ion_platform_heap *heap2)
{
	unsigned long heap1_base = heap1->base;
	unsigned long heap2_base = heap2->base;
	unsigned long heap1_end = heap1->base + heap1->size - 1;
	unsigned long heap2_end = heap2->base + heap2->size - 1;

	if (heap1_base == heap2_base)
		return 1;
	if (heap1_base < heap2_base && heap1_end >= heap2_base)
		return 1;
	if (heap2_base < heap1_base && heap2_end >= heap1_base)
		return 1;
	return 0;
}

static void check_for_heap_overlap(const struct ion_platform_heap heap_list[],
				   unsigned long nheaps)
{
	unsigned long i;
	unsigned long j;

	for (i = 0; i < nheaps; ++i) {
		const struct ion_platform_heap *heap1 = &heap_list[i];
		if (!heap1->base)
			continue;
		for (j = i + 1; j < nheaps; ++j) {
			const struct ion_platform_heap *heap2 = &heap_list[j];
			if (!heap2->base)
				continue;
			if (is_heap_overlapping(heap1, heap2)) {
				panic("Memory in heap %s overlaps with heap %s\n",
					heap1->name, heap2->name);
			}
		}
	}
}

static int msm_init_extra_data(struct ion_platform_heap *heap,
			       const struct ion_heap_desc *heap_desc)
{
	int ret = 0;

	switch ((int) heap->type) {
	case ION_HEAP_TYPE_CP:
	{
		heap->extra_data = kzalloc(sizeof(struct ion_cp_heap_pdata),
					   GFP_KERNEL);
		if (!heap->extra_data) {
			ret = -ENOMEM;
		} else {
			struct ion_cp_heap_pdata *extra = heap->extra_data;
			extra->permission_type = heap_desc->permission_type;
		}
		break;
	}
	case ION_HEAP_TYPE_CARVEOUT:
	{
		heap->extra_data = kzalloc(sizeof(struct ion_co_heap_pdata),
					   GFP_KERNEL);
		if (!heap->extra_data)
			ret = -ENOMEM;
		break;
	}
	default:
		heap->extra_data = 0;
		break;
	}
	return ret;
}

static int msm_ion_populate_heap(struct ion_platform_heap *heap)
{
	unsigned int i;
	int ret = -EINVAL;
	unsigned int len = ARRAY_SIZE(ion_heap_meta);
	for (i = 0; i < len; ++i) {
		if (ion_heap_meta[i].id == heap->id) {
			heap->name = ion_heap_meta[i].name;
			heap->type = ion_heap_meta[i].type;
			ret = msm_init_extra_data(heap, &ion_heap_meta[i]);
			break;
		}
	}
	if (ret)
		pr_err("%s: Unable to populate heap, error: %d", __func__, ret);
	return ret;
}

static void free_pdata(const struct ion_platform_data *pdata)
{
	unsigned int i;
	for (i = 0; i < pdata->nr; ++i)
		kfree(pdata->heaps[i].extra_data);
	kfree(pdata);
}

static int memtype_to_ion_memtype[] = {
	[MEMTYPE_SMI_KERNEL] = ION_SMI_TYPE,
	[MEMTYPE_SMI]	= ION_SMI_TYPE,
	[MEMTYPE_EBI0] = ION_EBI_TYPE,
	[MEMTYPE_EBI1] = ION_EBI_TYPE,
};

static void msm_ion_get_heap_align(struct device_node *node,
				   struct ion_platform_heap *heap)
{
	unsigned int val;

	int ret = of_property_read_u32(node, "qcom,heap-align", &val);
	if (!ret) {
		switch ((int) heap->type) {
		case ION_HEAP_TYPE_CP:
		{
			struct ion_cp_heap_pdata *extra =
						heap->extra_data;
			extra->align = val;
			break;
		}
		case ION_HEAP_TYPE_CARVEOUT:
		{
			struct ion_co_heap_pdata *extra =
						heap->extra_data;
			extra->align = val;
			break;
		}
		default:
			pr_err("ION-heap %s: Cannot specify alignment for this type of heap\n",
					heap->name);
			break;
		}
	}
}

static int msm_ion_get_heap_size(struct device_node *node,
				 struct ion_platform_heap *heap)
{
	unsigned int val;
	int ret = 0;
	const char *memory_name_prop;

	ret = of_property_read_u32(node, "qcom,memory-reservation-size", &val);
	if (!ret) {
		heap->size = val;
		ret = of_property_read_string(node,
					      "qcom,memory-reservation-type",
					      &memory_name_prop);

		if (!ret && memory_name_prop) {
			val = msm_get_memory_type_from_name(memory_name_prop);
			if (val < 0) {
				ret = -EINVAL;
				goto out;
			}
			heap->memory_type = memtype_to_ion_memtype[val];
		}
		if (heap->size && (ret || !memory_name_prop)) {
			pr_err("%s: Need to specify reservation type\n",
				__func__);
			ret = -EINVAL;
		}
	} else {
		ret = 0;
	}
out:
	return ret;
}


static void msm_ion_get_heap_adjacent(struct device_node *node,
				      struct ion_platform_heap *heap)
{
	unsigned int val;
	int ret = of_property_read_u32(node, "qcom,heap-adjacent", &val);
	if (!ret) {
		switch (heap->type) {
		case ION_HEAP_TYPE_CARVEOUT:
		{
			struct ion_co_heap_pdata *extra = heap->extra_data;
			extra->adjacent_mem_id = val;
			break;
		}
		default:
			pr_err("ION-heap %s: Cannot specify adjcent mem id for this type of heap\n",
				heap->name);
			break;
		}
	} else {
		switch (heap->type) {
		case ION_HEAP_TYPE_CARVEOUT:
		{
			struct ion_co_heap_pdata *extra = heap->extra_data;
			extra->adjacent_mem_id = INVALID_HEAP_ID;
			break;
		}
		default:
			break;
		}
	}
}

static struct ion_platform_data *msm_ion_parse_dt(
					const struct device_node *dt_node)
{
	struct ion_platform_data *pdata = 0;
	struct device_node *node;
	uint32_t val = 0;
	int ret = 0;
	uint32_t num_heaps = 0;
	int idx = 0;

	for_each_child_of_node(dt_node, node)
		num_heaps++;

	if (!num_heaps)
		return ERR_PTR(-EINVAL);

	pdata = kzalloc(sizeof(struct ion_platform_data) +
			num_heaps*sizeof(struct ion_platform_heap), GFP_KERNEL);
	if (!pdata)
		return ERR_PTR(-ENOMEM);

	pdata->nr = num_heaps;

	for_each_child_of_node(dt_node, node) {
		/**
		 * TODO: Replace this with of_get_address() when this patch
		 * gets merged: http://
		 * permalink.gmane.org/gmane.linux.drivers.devicetree/18614
		*/
		ret = of_property_read_u32(node, "reg", &val);
		if (ret) {
			pr_err("%s: Unable to find reg key", __func__);
			goto free_heaps;
		}
		pdata->heaps[idx].id = val;

		ret = msm_ion_populate_heap(&pdata->heaps[idx]);
		if (ret)
			goto free_heaps;

		msm_ion_get_heap_align(node, &pdata->heaps[idx]);

		ret = msm_ion_get_heap_size(node, &pdata->heaps[idx]);
		if (ret)
			goto free_heaps;

		msm_ion_get_heap_adjacent(node, &pdata->heaps[idx]);

		++idx;
	}
	return pdata;

free_heaps:
	free_pdata(pdata);
	return ERR_PTR(ret);
}

static int check_vaddr_bounds(unsigned long start, unsigned long end)
{
	struct mm_struct *mm = current->active_mm;
	struct vm_area_struct *vma;
	int ret = 1;

	if (end < start)
		goto out;

	down_read(&mm->mmap_sem);
	vma = find_vma(mm, start);
	if (vma && vma->vm_start < end) {
		if (start < vma->vm_start)
			goto out_up;
		if (end > vma->vm_end)
			goto out_up;
		ret = 0;
	}

out_up:
	up_read(&mm->mmap_sem);
out:
	return ret;
}

static long msm_ion_custom_ioctl(struct ion_client *client,
				unsigned int cmd,
				unsigned long arg)
{
	switch (cmd) {
	case ION_IOC_CLEAN_CACHES:
	case ION_IOC_INV_CACHES:
	case ION_IOC_CLEAN_INV_CACHES:
	{
		struct ion_flush_data data;
		unsigned long start, end;
		struct ion_handle *handle = NULL;
		int ret;

		if (copy_from_user(&data, (void __user *)arg,
					sizeof(struct ion_flush_data)))
			return -EFAULT;

		start = (unsigned long) data.vaddr;
		end = (unsigned long) data.vaddr + data.length;

		if (start && check_vaddr_bounds(start, end)) {
			pr_err("%s: virtual address %p is out of bounds\n",
				__func__, data.vaddr);
			return -EINVAL;
		}

		if (!data.handle) {
			handle = ion_import_dma_buf(client, data.fd);
			if (IS_ERR(handle)) {
				pr_info("%s: Could not import handle: %d\n",
					__func__, (int)handle);
				return -EINVAL;
			}
		}

		ret = ion_do_cache_op(client,
				data.handle ? data.handle : handle,
				data.vaddr, data.offset, data.length,
				cmd);

		if (!data.handle)
			ion_free(client, handle);

		if (ret < 0)
			return ret;
		break;

	}
	case ION_IOC_GET_FLAGS:
	{
		struct ion_flag_data data;
		int ret;
		if (copy_from_user(&data, (void __user *)arg,
					sizeof(struct ion_flag_data)))
			return -EFAULT;

		ret = ion_handle_get_flags(client, data.handle, &data.flags);
		if (ret < 0)
			return ret;
		if (copy_to_user((void __user *)arg, &data,
					sizeof(struct ion_flag_data)))
			return -EFAULT;
		break;
	}
	case ION_IOC_GET_PHYS:
	{
		struct ion_buffer_data data;
		int ret = 0;

		if (copy_from_user(&data, (void __user *)arg,
					sizeof(struct ion_buffer_data)))
			return -EFAULT;

		ret = ion_phys(client, data.handle,
				(ion_phys_addr_t*)(&data.paddr), &data.length);
		if (ret < 0)
			return ret;

		if (copy_to_user((void __user *)arg, &data,
					sizeof(struct ion_buffer_data)))
			return -EFAULT;
		break;
	}
	default:
		return -ENOTTY;
	}
	return 0;
}

static int msm_ion_probe(struct platform_device *pdev)
{
	struct ion_platform_data *pdata;
	unsigned int pdata_needs_to_be_freed;
	int err = -1;
	int i;
	if (pdev->dev.of_node) {
		pdata = msm_ion_parse_dt(pdev->dev.of_node);
		if (IS_ERR(pdata)) {
			err = PTR_ERR(pdata);
			goto out;
		}
		pdata_needs_to_be_freed = 1;
	} else {
		pdata = pdev->dev.platform_data;
		pdata_needs_to_be_freed = 0;
	}

	num_heaps = pdata->nr;

	heaps = kcalloc(pdata->nr, sizeof(struct ion_heap *), GFP_KERNEL);

	if (!heaps) {
		err = -ENOMEM;
		goto out;
	}

	idev = ion_device_create(msm_ion_custom_ioctl);
	if (IS_ERR_OR_NULL(idev)) {
		err = PTR_ERR(idev);
		goto freeheaps;
	}

	msm_ion_heap_fixup(pdata->heaps, num_heaps);

	/* create the heaps as specified in the board file */
	for (i = 0; i < num_heaps; i++) {
		struct ion_platform_heap *heap_data = &pdata->heaps[i];
		msm_ion_allocate(heap_data);

		heap_data->has_outer_cache = pdata->has_outer_cache;
		heaps[i] = ion_heap_create(heap_data);
		if (IS_ERR_OR_NULL(heaps[i])) {
			heaps[i] = 0;
			continue;
		} else {
			if (heap_data->size)
				pr_info("ION heap %s created at %lx "
					"with size %x\n", heap_data->name,
							  heap_data->base,
							  heap_data->size);
			else
				pr_info("ION heap %s created\n",
							  heap_data->name);
		}

		ion_device_add_heap(idev, heaps[i]);
	}
	if (pdata_needs_to_be_freed)
		free_pdata(pdata);

	check_for_heap_overlap(pdata->heaps, num_heaps);
	platform_set_drvdata(pdev, idev);
	return 0;

freeheaps:
	kfree(heaps);
	if (pdata_needs_to_be_freed)
		free_pdata(pdata);
out:
	return err;
}

static int msm_ion_remove(struct platform_device *pdev)
{
	struct ion_device *idev = platform_get_drvdata(pdev);
	int i;

	for (i = 0; i < num_heaps; i++)
		ion_heap_destroy(heaps[i]);

	ion_device_destroy(idev);
	kfree(heaps);
	return 0;
}

static struct of_device_id msm_ion_match_table[] = {
	{.compatible = ION_COMPAT_STR},
	{},
};
EXPORT_COMPAT(ION_COMPAT_MEM_RESERVE_STR);

static struct platform_driver msm_ion_driver = {
	.probe = msm_ion_probe,
	.remove = msm_ion_remove,
	.driver = {
		.name = "ion-msm",
		.of_match_table = msm_ion_match_table,
	},
};

static int __init msm_ion_init(void)
{
	return platform_driver_register(&msm_ion_driver);
}

static void __exit msm_ion_exit(void)
{
	platform_driver_unregister(&msm_ion_driver);
}

subsys_initcall(msm_ion_init);
module_exit(msm_ion_exit);


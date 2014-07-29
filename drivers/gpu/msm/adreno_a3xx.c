/* Copyright (c) 2012-2014, The Linux Foundation. All rights reserved.
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

#include <linux/delay.h>
#include <linux/sched.h>
#include <mach/socinfo.h>

#include "kgsl.h"
#include "adreno.h"
#include "kgsl_sharedmem.h"
#include "kgsl_cffdump.h"
#include "a3xx_reg.h"
#include "adreno_a3xx_trace.h"

/*
 * Set of registers to dump for A3XX on postmortem and snapshot.
 * Registers in pairs - first value is the start offset, second
 * is the stop offset (inclusive)
 */

const unsigned int a3xx_registers[] = {
	0x0000, 0x0002, 0x0010, 0x0012, 0x0018, 0x0018, 0x0020, 0x0027,
	0x0029, 0x002b, 0x002e, 0x0033, 0x0040, 0x0042, 0x0050, 0x005c,
	0x0060, 0x006c, 0x0080, 0x0082, 0x0084, 0x0088, 0x0090, 0x00e5,
	0x00ea, 0x00ed, 0x0100, 0x0100, 0x0110, 0x0123, 0x01c0, 0x01c1,
	0x01c3, 0x01c5, 0x01c7, 0x01c7, 0x01d5, 0x01d9, 0x01dc, 0x01dd,
	0x01ea, 0x01ea, 0x01ee, 0x01f1, 0x01f5, 0x01f5, 0x01fc, 0x01ff,
	0x0440, 0x0440, 0x0443, 0x0443, 0x0445, 0x0445, 0x044d, 0x044f,
	0x0452, 0x0452, 0x0454, 0x046f, 0x047c, 0x047c, 0x047f, 0x047f,
	0x0578, 0x057f, 0x0600, 0x0602, 0x0605, 0x0607, 0x060a, 0x060e,
	0x0612, 0x0614, 0x0c01, 0x0c02, 0x0c06, 0x0c1d, 0x0c3d, 0x0c3f,
	0x0c48, 0x0c4b, 0x0c80, 0x0c80, 0x0c88, 0x0c8b, 0x0ca0, 0x0cb7,
	0x0cc0, 0x0cc1, 0x0cc6, 0x0cc7, 0x0ce4, 0x0ce5,
	0x0e41, 0x0e45, 0x0e64, 0x0e65,
	0x0e80, 0x0e82, 0x0e84, 0x0e89, 0x0ea0, 0x0ea1, 0x0ea4, 0x0ea7,
	0x0ec4, 0x0ecb, 0x0ee0, 0x0ee0, 0x0f00, 0x0f01, 0x0f03, 0x0f09,
	0x2040, 0x2040, 0x2044, 0x2044, 0x2048, 0x204d, 0x2068, 0x2069,
	0x206c, 0x206d, 0x2070, 0x2070, 0x2072, 0x2072, 0x2074, 0x2075,
	0x2079, 0x207a, 0x20c0, 0x20d3, 0x20e4, 0x20ef, 0x2100, 0x2109,
	0x210c, 0x210c, 0x210e, 0x210e, 0x2110, 0x2111, 0x2114, 0x2115,
	0x21e4, 0x21e4, 0x21ea, 0x21ea, 0x21ec, 0x21ed, 0x21f0, 0x21f0,
	0x2240, 0x227e,
	0x2280, 0x228b, 0x22c0, 0x22c0, 0x22c4, 0x22ce, 0x22d0, 0x22d8,
	0x22df, 0x22e6, 0x22e8, 0x22e9, 0x22ec, 0x22ec, 0x22f0, 0x22f7,
	0x22ff, 0x22ff, 0x2340, 0x2343, 0x2348, 0x2349, 0x2350, 0x2356,
	0x2360, 0x2360, 0x2440, 0x2440, 0x2444, 0x2444, 0x2448, 0x244d,
	0x2468, 0x2469, 0x246c, 0x246d, 0x2470, 0x2470, 0x2472, 0x2472,
	0x2474, 0x2475, 0x2479, 0x247a, 0x24c0, 0x24d3, 0x24e4, 0x24ef,
	0x2500, 0x2509, 0x250c, 0x250c, 0x250e, 0x250e, 0x2510, 0x2511,
	0x2514, 0x2515, 0x25e4, 0x25e4, 0x25ea, 0x25ea, 0x25ec, 0x25ed,
	0x25f0, 0x25f0,
	0x2640, 0x267e, 0x2680, 0x268b, 0x26c0, 0x26c0, 0x26c4, 0x26ce,
	0x26d0, 0x26d8, 0x26df, 0x26e6, 0x26e8, 0x26e9, 0x26ec, 0x26ec,
	0x26f0, 0x26f7, 0x26ff, 0x26ff, 0x2740, 0x2743, 0x2748, 0x2749,
	0x2750, 0x2756, 0x2760, 0x2760, 0x300C, 0x300E, 0x301C, 0x301D,
	0x302A, 0x302A, 0x302C, 0x302D, 0x3030, 0x3031, 0x3034, 0x3036,
	0x303C, 0x303C, 0x305E, 0x305F,
};

const unsigned int a3xx_registers_count = ARRAY_SIZE(a3xx_registers) / 2;

/* Removed the following HLSQ register ranges from being read during
 * fault tolerance since reading the registers may cause the device to hang:
 */
const unsigned int a3xx_hlsq_registers[] = {
	0x0e00, 0x0e05, 0x0e0c, 0x0e0c, 0x0e22, 0x0e23,
	0x2200, 0x2212, 0x2214, 0x2217, 0x221a, 0x221a,
	0x2600, 0x2612, 0x2614, 0x2617, 0x261a, 0x261a,
};

const unsigned int a3xx_hlsq_registers_count =
			ARRAY_SIZE(a3xx_hlsq_registers) / 2;

/* The set of additional registers to be dumped for A330 */

const unsigned int a330_registers[] = {
	0x1d0, 0x1d0, 0x1d4, 0x1d4, 0x453, 0x453,
};

const unsigned int a330_registers_count = ARRAY_SIZE(a330_registers) / 2;

/* Simple macro to facilitate bit setting in the gmem2sys and sys2gmem
 * functions.
 */

#define _SET(_shift, _val) ((_val) << (_shift))

/*
 ****************************************************************************
 *
 * Context state shadow structure:
 *
 * +---------------------+------------+-------------+---------------------+---+
 * | ALU Constant Shadow | Reg Shadow | C&V Buffers | Shader Instr Shadow |Tex|
 * +---------------------+------------+-------------+---------------------+---+
 *
 *		 8K - ALU Constant Shadow (8K aligned)
 *		 4K - H/W Register Shadow (8K aligned)
 *		 5K - Command and Vertex Buffers
 *		 8K - Shader Instruction Shadow
 *		 ~6K - Texture Constant Shadow
 *
 *
 ***************************************************************************
 */

/* Sizes of all sections in state shadow memory */
#define ALU_SHADOW_SIZE      (8*1024) /* 8KB */
#define REG_SHADOW_SIZE      (4*1024) /* 4KB */
#define CMD_BUFFER_SIZE      (5*1024) /* 5KB */
#define TEX_SIZE_MEM_OBJECTS 896      /* bytes */
#define TEX_SIZE_MIPMAP      1936     /* bytes */
#define TEX_SIZE_SAMPLER_OBJ 256      /* bytes */
#define TEX_SHADOW_SIZE                            \
	((TEX_SIZE_MEM_OBJECTS + TEX_SIZE_MIPMAP + \
	TEX_SIZE_SAMPLER_OBJ)*2) /* ~6KB */
#define SHADER_SHADOW_SIZE   (8*1024) /* 8KB */

/* Total context size, excluding GMEM shadow */
#define CONTEXT_SIZE                         \
	(ALU_SHADOW_SIZE+REG_SHADOW_SIZE +   \
	CMD_BUFFER_SIZE+SHADER_SHADOW_SIZE + \
	TEX_SHADOW_SIZE)

/* Offsets to different sections in context shadow memory */
#define REG_OFFSET ALU_SHADOW_SIZE
#define CMD_OFFSET (REG_OFFSET+REG_SHADOW_SIZE)
#define SHADER_OFFSET (CMD_OFFSET+CMD_BUFFER_SIZE)
#define TEX_OFFSET (SHADER_OFFSET+SHADER_SHADOW_SIZE)
#define VS_TEX_OFFSET_MEM_OBJECTS TEX_OFFSET
#define VS_TEX_OFFSET_MIPMAP (VS_TEX_OFFSET_MEM_OBJECTS+TEX_SIZE_MEM_OBJECTS)
#define VS_TEX_OFFSET_SAMPLER_OBJ (VS_TEX_OFFSET_MIPMAP+TEX_SIZE_MIPMAP)
#define FS_TEX_OFFSET_MEM_OBJECTS \
	(VS_TEX_OFFSET_SAMPLER_OBJ+TEX_SIZE_SAMPLER_OBJ)
#define FS_TEX_OFFSET_MIPMAP (FS_TEX_OFFSET_MEM_OBJECTS+TEX_SIZE_MEM_OBJECTS)
#define FS_TEX_OFFSET_SAMPLER_OBJ (FS_TEX_OFFSET_MIPMAP+TEX_SIZE_MIPMAP)

/* The offset for fragment shader data in HLSQ context */
#define SSIZE (16*1024)

#define HLSQ_SAMPLER_OFFSET 0x000
#define HLSQ_MEMOBJ_OFFSET  0x400
#define HLSQ_MIPMAP_OFFSET  0x800

/* Use shadow RAM */
#define HLSQ_SHADOW_BASE		(0x10000+SSIZE*2)

#define REG_TO_MEM_LOOP_COUNT_SHIFT	18

#define BUILD_PC_DRAW_INITIATOR(prim_type, source_select, index_size, \
	vis_cull_mode) \
	(((prim_type)      << PC_DRAW_INITIATOR_PRIM_TYPE) | \
	((source_select)   << PC_DRAW_INITIATOR_SOURCE_SELECT) | \
	((index_size & 1)  << PC_DRAW_INITIATOR_INDEX_SIZE) | \
	((index_size >> 1) << PC_DRAW_INITIATOR_SMALL_INDEX) | \
	((vis_cull_mode)   << PC_DRAW_INITIATOR_VISIBILITY_CULLING_MODE) | \
	(1                 << PC_DRAW_INITIATOR_PRE_DRAW_INITIATOR_ENABLE))

/*
 * List of context registers (starting from dword offset 0x2000).
 * Each line contains start and end of a range of registers.
 */
static const unsigned int context_register_ranges[] = {
	A3XX_GRAS_CL_CLIP_CNTL, A3XX_GRAS_CL_CLIP_CNTL,
	A3XX_GRAS_CL_GB_CLIP_ADJ, A3XX_GRAS_CL_GB_CLIP_ADJ,
	A3XX_GRAS_CL_VPORT_XOFFSET, A3XX_GRAS_CL_VPORT_ZSCALE,
	A3XX_GRAS_SU_POINT_MINMAX, A3XX_GRAS_SU_POINT_SIZE,
	A3XX_GRAS_SU_POLY_OFFSET_SCALE, A3XX_GRAS_SU_POLY_OFFSET_OFFSET,
	A3XX_GRAS_SU_MODE_CONTROL, A3XX_GRAS_SU_MODE_CONTROL,
	A3XX_GRAS_SC_CONTROL, A3XX_GRAS_SC_CONTROL,
	A3XX_GRAS_SC_SCREEN_SCISSOR_TL, A3XX_GRAS_SC_SCREEN_SCISSOR_BR,
	A3XX_GRAS_SC_WINDOW_SCISSOR_TL, A3XX_GRAS_SC_WINDOW_SCISSOR_BR,
	A3XX_RB_MODE_CONTROL, A3XX_RB_MRT_BLEND_CONTROL3,
	A3XX_RB_BLEND_RED, A3XX_RB_COPY_DEST_INFO,
	A3XX_RB_DEPTH_CONTROL, A3XX_RB_DEPTH_CONTROL,
	A3XX_PC_VSTREAM_CONTROL, A3XX_PC_VSTREAM_CONTROL,
	A3XX_PC_VERTEX_REUSE_BLOCK_CNTL, A3XX_PC_VERTEX_REUSE_BLOCK_CNTL,
	A3XX_PC_PRIM_VTX_CNTL, A3XX_PC_RESTART_INDEX,
	A3XX_HLSQ_CONTROL_0_REG, A3XX_HLSQ_CONST_FSPRESV_RANGE_REG,
	A3XX_HLSQ_CL_NDRANGE_0_REG, A3XX_HLSQ_CL_NDRANGE_0_REG,
	A3XX_HLSQ_CL_NDRANGE_2_REG, A3XX_HLSQ_CL_CONTROL_1_REG,
	A3XX_HLSQ_CL_KERNEL_CONST_REG, A3XX_HLSQ_CL_KERNEL_GROUP_Z_REG,
	A3XX_HLSQ_CL_WG_OFFSET_REG, A3XX_HLSQ_CL_WG_OFFSET_REG,
	A3XX_VFD_CONTROL_0, A3XX_VFD_VS_THREADING_THRESHOLD,
	A3XX_SP_SP_CTRL_REG, A3XX_SP_SP_CTRL_REG,
	A3XX_SP_VS_CTRL_REG0, A3XX_SP_VS_OUT_REG_7,
	A3XX_SP_VS_VPC_DST_REG_0, A3XX_SP_VS_PVT_MEM_SIZE_REG,
	A3XX_SP_VS_LENGTH_REG, A3XX_SP_FS_PVT_MEM_SIZE_REG,
	A3XX_SP_FS_FLAT_SHAD_MODE_REG_0, A3XX_SP_FS_FLAT_SHAD_MODE_REG_1,
	A3XX_SP_FS_OUTPUT_REG, A3XX_SP_FS_OUTPUT_REG,
	A3XX_SP_FS_MRT_REG_0, A3XX_SP_FS_IMAGE_OUTPUT_REG_3,
	A3XX_SP_FS_LENGTH_REG, A3XX_SP_FS_LENGTH_REG,
	A3XX_TPL1_TP_VS_TEX_OFFSET, A3XX_TPL1_TP_FS_BORDER_COLOR_BASE_ADDR,
	A3XX_VPC_ATTR, A3XX_VPC_VARY_CYLWRAP_ENABLE_1,
};

/* Global registers that need to be saved separately */
static const unsigned int global_registers[] = {
	A3XX_GRAS_CL_USER_PLANE_X0, A3XX_GRAS_CL_USER_PLANE_Y0,
	A3XX_GRAS_CL_USER_PLANE_Z0, A3XX_GRAS_CL_USER_PLANE_W0,
	A3XX_GRAS_CL_USER_PLANE_X1, A3XX_GRAS_CL_USER_PLANE_Y1,
	A3XX_GRAS_CL_USER_PLANE_Z1, A3XX_GRAS_CL_USER_PLANE_W1,
	A3XX_GRAS_CL_USER_PLANE_X2, A3XX_GRAS_CL_USER_PLANE_Y2,
	A3XX_GRAS_CL_USER_PLANE_Z2, A3XX_GRAS_CL_USER_PLANE_W2,
	A3XX_GRAS_CL_USER_PLANE_X3, A3XX_GRAS_CL_USER_PLANE_Y3,
	A3XX_GRAS_CL_USER_PLANE_Z3, A3XX_GRAS_CL_USER_PLANE_W3,
	A3XX_GRAS_CL_USER_PLANE_X4, A3XX_GRAS_CL_USER_PLANE_Y4,
	A3XX_GRAS_CL_USER_PLANE_Z4, A3XX_GRAS_CL_USER_PLANE_W4,
	A3XX_GRAS_CL_USER_PLANE_X5, A3XX_GRAS_CL_USER_PLANE_Y5,
	A3XX_GRAS_CL_USER_PLANE_Z5, A3XX_GRAS_CL_USER_PLANE_W5,
	A3XX_VSC_BIN_SIZE,
	A3XX_VSC_PIPE_CONFIG_0, A3XX_VSC_PIPE_CONFIG_1,
	A3XX_VSC_PIPE_CONFIG_2, A3XX_VSC_PIPE_CONFIG_3,
	A3XX_VSC_PIPE_CONFIG_4, A3XX_VSC_PIPE_CONFIG_5,
	A3XX_VSC_PIPE_CONFIG_6, A3XX_VSC_PIPE_CONFIG_7,
	A3XX_VSC_PIPE_DATA_ADDRESS_0, A3XX_VSC_PIPE_DATA_ADDRESS_1,
	A3XX_VSC_PIPE_DATA_ADDRESS_2, A3XX_VSC_PIPE_DATA_ADDRESS_3,
	A3XX_VSC_PIPE_DATA_ADDRESS_4, A3XX_VSC_PIPE_DATA_ADDRESS_5,
	A3XX_VSC_PIPE_DATA_ADDRESS_6, A3XX_VSC_PIPE_DATA_ADDRESS_7,
	A3XX_VSC_PIPE_DATA_LENGTH_0, A3XX_VSC_PIPE_DATA_LENGTH_1,
	A3XX_VSC_PIPE_DATA_LENGTH_2, A3XX_VSC_PIPE_DATA_LENGTH_3,
	A3XX_VSC_PIPE_DATA_LENGTH_4, A3XX_VSC_PIPE_DATA_LENGTH_5,
	A3XX_VSC_PIPE_DATA_LENGTH_6, A3XX_VSC_PIPE_DATA_LENGTH_7,
	A3XX_VSC_SIZE_ADDRESS
};

#define GLOBAL_REGISTER_COUNT ARRAY_SIZE(global_registers)

/* A scratchpad used to build commands during context create */
static struct tmp_ctx {
	unsigned int *cmd; /* Next available dword in C&V buffer */

	/* Addresses in comamnd buffer where registers are saved */
	uint32_t reg_values[GLOBAL_REGISTER_COUNT];
	uint32_t gmem_base; /* Base GPU address of GMEM */
} tmp_ctx;

#ifndef GSL_CONTEXT_SWITCH_CPU_SYNC
/*
 * Function for executing dest = ( (reg & and) ROL rol ) | or
 */
static unsigned int *rmw_regtomem(unsigned int *cmd,
				  unsigned int reg, unsigned int and,
				  unsigned int rol, unsigned int or,
				  unsigned int dest)
{
	/* CP_SCRATCH_REG2 = (CP_SCRATCH_REG2 & 0x00000000) | reg */
	*cmd++ = cp_type3_packet(CP_REG_RMW, 3);
	*cmd++ = (1 << 30) | A3XX_CP_SCRATCH_REG2;
	*cmd++ = 0x00000000;	/* AND value */
	*cmd++ = reg;		/* OR address */

	/* CP_SCRATCH_REG2 = ( (CP_SCRATCH_REG2 & and) ROL rol ) |  or */
	*cmd++ = cp_type3_packet(CP_REG_RMW, 3);
	*cmd++ = (rol << 24) | A3XX_CP_SCRATCH_REG2;
	*cmd++ = and;		/* AND value */
	*cmd++ = or;		/* OR value */

	*cmd++ = cp_type3_packet(CP_REG_TO_MEM, 2);
	*cmd++ = A3XX_CP_SCRATCH_REG2;
	*cmd++ = dest;

	return cmd;
}
#endif

static void build_regconstantsave_cmds(struct adreno_device *adreno_dev,
				       struct adreno_context *drawctxt)
{
	unsigned int *cmd = tmp_ctx.cmd;
	unsigned int *start;
	unsigned int i;

	drawctxt->constant_save_commands[0].hostptr = cmd;
	drawctxt->constant_save_commands[0].gpuaddr =
	    virt2gpu(cmd, &drawctxt->gpustate);
	cmd++;

	start = cmd;

	*cmd++ = cp_type3_packet(CP_WAIT_FOR_IDLE, 1);
	*cmd++ = 0;

#ifndef CONFIG_MSM_KGSL_DISABLE_SHADOW_WRITES
	/*
	 * Context registers are already shadowed; just need to
	 * disable shadowing to prevent corruption.
	 */

	*cmd++ = cp_type3_packet(CP_LOAD_CONSTANT_CONTEXT, 3);
	*cmd++ = (drawctxt->gpustate.gpuaddr + REG_OFFSET) & 0xFFFFE000;
	*cmd++ = 4 << 16;	/* regs, start=0 */
	*cmd++ = 0x0;		/* count = 0 */

#else
	/*
	 * Make sure the HW context has the correct register values before
	 * reading them.
	 */

	/* Write context registers into shadow */
	for (i = 0; i < ARRAY_SIZE(context_register_ranges) / 2; i++) {
		unsigned int start = context_register_ranges[i * 2];
		unsigned int end = context_register_ranges[i * 2 + 1];
		*cmd++ = cp_type3_packet(CP_REG_TO_MEM, 2);
		*cmd++ = ((end - start + 1) << REG_TO_MEM_LOOP_COUNT_SHIFT) |
			start;
		*cmd++ = ((drawctxt->gpustate.gpuaddr + REG_OFFSET)
			  & 0xFFFFE000) + (start - 0x2000) * 4;
	}
#endif

	/* Need to handle some of the global registers separately */
	for (i = 0; i < ARRAY_SIZE(global_registers); i++) {
		*cmd++ = cp_type3_packet(CP_REG_TO_MEM, 2);
		*cmd++ = global_registers[i];
		*cmd++ = tmp_ctx.reg_values[i];
	}

	/* Save vertex shader constants */
	*cmd++ = cp_type3_packet(CP_COND_EXEC, 4);
	*cmd++ = drawctxt->cond_execs[2].gpuaddr >> 2;
	*cmd++ = drawctxt->cond_execs[2].gpuaddr >> 2;
	*cmd++ = 0x0000FFFF;
	*cmd++ = 3; /* EXEC_COUNT */
	*cmd++ = cp_type3_packet(CP_REG_TO_MEM, 2);
	drawctxt->constant_save_commands[1].hostptr = cmd;
	drawctxt->constant_save_commands[1].gpuaddr =
	    virt2gpu(cmd, &drawctxt->gpustate);
	/*
	   From fixup:

	   dwords = SP_VS_CTRL_REG1.VSCONSTLENGTH / 4
	   src = (HLSQ_SHADOW_BASE + 0x2000) / 4

	   From register spec:
	   SP_VS_CTRL_REG1.VSCONSTLENGTH [09:00]: 0-512, unit = 128bits.
	 */
	*cmd++ = 0;	/* (dwords << REG_TO_MEM_LOOP_COUNT_SHIFT) | src  */
	/* ALU constant shadow base */
	*cmd++ = drawctxt->gpustate.gpuaddr & 0xfffffffc;

	/* Save fragment shader constants */
	*cmd++ = cp_type3_packet(CP_COND_EXEC, 4);
	*cmd++ = drawctxt->cond_execs[3].gpuaddr >> 2;
	*cmd++ = drawctxt->cond_execs[3].gpuaddr >> 2;
	*cmd++ = 0x0000FFFF;
	*cmd++ = 3; /* EXEC_COUNT */
	*cmd++ = cp_type3_packet(CP_REG_TO_MEM, 2);
	drawctxt->constant_save_commands[2].hostptr = cmd;
	drawctxt->constant_save_commands[2].gpuaddr =
	    virt2gpu(cmd, &drawctxt->gpustate);
	/*
	   From fixup:

	   dwords = SP_FS_CTRL_REG1.FSCONSTLENGTH / 4
	   src = (HLSQ_SHADOW_BASE + 0x2000 + SSIZE) / 4

	   From register spec:
	   SP_FS_CTRL_REG1.FSCONSTLENGTH [09:00]: 0-512, unit = 128bits.
	 */
	*cmd++ = 0;	/* (dwords << REG_TO_MEM_LOOP_COUNT_SHIFT) | src  */

	/*
	   From fixup:

	   base = drawctxt->gpustate.gpuaddr (ALU constant shadow base)
	   offset = SP_FS_OBJ_OFFSET_REG.CONSTOBJECTSTARTOFFSET

	   From register spec:
	   SP_FS_OBJ_OFFSET_REG.CONSTOBJECTSTARTOFFSET [16:24]: Constant object
	   start offset in on chip RAM,
	   128bit aligned

	   dst = base + offset
	   Because of the base alignment we can use
	   dst = base | offset
	 */
	*cmd++ = 0;		/* dst */

	/* Save VS texture memory objects */
	*cmd++ = cp_type3_packet(CP_REG_TO_MEM, 2);
	*cmd++ =
	    ((TEX_SIZE_MEM_OBJECTS / 4) << REG_TO_MEM_LOOP_COUNT_SHIFT) |
		((HLSQ_SHADOW_BASE + HLSQ_MEMOBJ_OFFSET) / 4);
	*cmd++ =
	    (drawctxt->gpustate.gpuaddr +
	     VS_TEX_OFFSET_MEM_OBJECTS) & 0xfffffffc;

	/* Save VS texture mipmap pointers */
	*cmd++ = cp_type3_packet(CP_REG_TO_MEM, 2);
	*cmd++ =
	    ((TEX_SIZE_MIPMAP / 4) << REG_TO_MEM_LOOP_COUNT_SHIFT) |
		((HLSQ_SHADOW_BASE + HLSQ_MIPMAP_OFFSET) / 4);
	*cmd++ =
	    (drawctxt->gpustate.gpuaddr + VS_TEX_OFFSET_MIPMAP) & 0xfffffffc;

	/* Save VS texture sampler objects */
	*cmd++ = cp_type3_packet(CP_REG_TO_MEM, 2);
	*cmd++ = ((TEX_SIZE_SAMPLER_OBJ / 4) << REG_TO_MEM_LOOP_COUNT_SHIFT) |
		((HLSQ_SHADOW_BASE + HLSQ_SAMPLER_OFFSET) / 4);
	*cmd++ =
	    (drawctxt->gpustate.gpuaddr +
	     VS_TEX_OFFSET_SAMPLER_OBJ) & 0xfffffffc;

	/* Save FS texture memory objects */
	*cmd++ = cp_type3_packet(CP_REG_TO_MEM, 2);
	*cmd++ =
	    ((TEX_SIZE_MEM_OBJECTS / 4) << REG_TO_MEM_LOOP_COUNT_SHIFT) |
		((HLSQ_SHADOW_BASE + HLSQ_MEMOBJ_OFFSET + SSIZE) / 4);
	*cmd++ =
	    (drawctxt->gpustate.gpuaddr +
	     FS_TEX_OFFSET_MEM_OBJECTS) & 0xfffffffc;

	/* Save FS texture mipmap pointers */
	*cmd++ = cp_type3_packet(CP_REG_TO_MEM, 2);
	*cmd++ =
	    ((TEX_SIZE_MIPMAP / 4) << REG_TO_MEM_LOOP_COUNT_SHIFT) |
		((HLSQ_SHADOW_BASE + HLSQ_MIPMAP_OFFSET + SSIZE) / 4);
	*cmd++ =
	    (drawctxt->gpustate.gpuaddr + FS_TEX_OFFSET_MIPMAP) & 0xfffffffc;

	/* Save FS texture sampler objects */
	*cmd++ = cp_type3_packet(CP_REG_TO_MEM, 2);
	*cmd++ =
	    ((TEX_SIZE_SAMPLER_OBJ / 4) << REG_TO_MEM_LOOP_COUNT_SHIFT) |
		((HLSQ_SHADOW_BASE + HLSQ_SAMPLER_OFFSET + SSIZE) / 4);
	*cmd++ =
	    (drawctxt->gpustate.gpuaddr +
	     FS_TEX_OFFSET_SAMPLER_OBJ) & 0xfffffffc;

	/* Create indirect buffer command for above command sequence */
	create_ib1(drawctxt, drawctxt->regconstant_save, start, cmd);

	tmp_ctx.cmd = cmd;
}

unsigned int adreno_a3xx_rbbm_clock_ctl_default(struct adreno_device
							*adreno_dev)
{
	if (adreno_is_a305(adreno_dev))
		return A305_RBBM_CLOCK_CTL_DEFAULT;
	else if (adreno_is_a320(adreno_dev))
		return A320_RBBM_CLOCK_CTL_DEFAULT;
	else if (adreno_is_a330v2(adreno_dev))
		return A330v2_RBBM_CLOCK_CTL_DEFAULT;
	else if (adreno_is_a330(adreno_dev))
		return A330_RBBM_CLOCK_CTL_DEFAULT;

	BUG_ON(1);
}

/* Copy GMEM contents to system memory shadow. */
static unsigned int *build_gmem2sys_cmds(struct adreno_device *adreno_dev,
					 struct adreno_context *drawctxt,
					 struct gmem_shadow_t *shadow)
{
	unsigned int *cmds = tmp_ctx.cmd;
	unsigned int *start = cmds;

	*cmds++ = cp_type0_packet(A3XX_RBBM_CLOCK_CTL, 1);
	*cmds++ = adreno_a3xx_rbbm_clock_ctl_default(adreno_dev);

	*cmds++ = cp_type3_packet(CP_SET_CONSTANT, 3);
	*cmds++ = CP_REG(A3XX_RB_MODE_CONTROL);

	/* RB_MODE_CONTROL */
	*cmds++ = _SET(RB_MODECONTROL_RENDER_MODE, RB_RESOLVE_PASS) |
		_SET(RB_MODECONTROL_MARB_CACHE_SPLIT_MODE, 1) |
		_SET(RB_MODECONTROL_PACKER_TIMER_ENABLE, 1);
	/* RB_RENDER_CONTROL */
	*cmds++ = _SET(RB_RENDERCONTROL_BIN_WIDTH, shadow->width >> 5) |
		_SET(RB_RENDERCONTROL_DISABLE_COLOR_PIPE, 1);

	*cmds++ = cp_type3_packet(CP_SET_CONSTANT, 5);
	*cmds++ = CP_REG(A3XX_RB_COPY_CONTROL);
	/* RB_COPY_CONTROL */
	*cmds++ = _SET(RB_COPYCONTROL_RESOLVE_CLEAR_MODE,
		RB_CLEAR_MODE_RESOLVE) |
		_SET(RB_COPYCONTROL_COPY_GMEM_BASE,
		tmp_ctx.gmem_base >> 14);
	/* RB_COPY_DEST_BASE */
	*cmds++ = _SET(RB_COPYDESTBASE_COPY_DEST_BASE,
		shadow->gmemshadow.gpuaddr >> 5);
	/* RB_COPY_DEST_PITCH */
	*cmds++ = _SET(RB_COPYDESTPITCH_COPY_DEST_PITCH,
		(shadow->pitch * 4) / 32);
	/* RB_COPY_DEST_INFO */
	*cmds++ = _SET(RB_COPYDESTINFO_COPY_DEST_TILE,
		RB_TILINGMODE_LINEAR) |
		_SET(RB_COPYDESTINFO_COPY_DEST_FORMAT, RB_R8G8B8A8_UNORM) |
		_SET(RB_COPYDESTINFO_COPY_COMPONENT_ENABLE, 0X0F) |
		_SET(RB_COPYDESTINFO_COPY_DEST_ENDIAN, RB_ENDIAN_NONE);

	*cmds++ = cp_type3_packet(CP_SET_CONSTANT, 2);
	*cmds++ = CP_REG(A3XX_GRAS_SC_CONTROL);
	/* GRAS_SC_CONTROL */
	*cmds++ = _SET(GRAS_SC_CONTROL_RENDER_MODE, 2);

	*cmds++ = cp_type3_packet(CP_SET_CONSTANT, 3);
	*cmds++ = CP_REG(A3XX_VFD_CONTROL_0);
	/* VFD_CONTROL_0 */
	*cmds++ = _SET(VFD_CTRLREG0_TOTALATTRTOVS, 4) |
		_SET(VFD_CTRLREG0_PACKETSIZE, 2) |
		_SET(VFD_CTRLREG0_STRMDECINSTRCNT, 1) |
		_SET(VFD_CTRLREG0_STRMFETCHINSTRCNT, 1);
	/* VFD_CONTROL_1 */
	*cmds++ = _SET(VFD_CTRLREG1_MAXSTORAGE, 1) |
		_SET(VFD_CTRLREG1_REGID4VTX,  252) |
		_SET(VFD_CTRLREG1_REGID4INST,  252);

	*cmds++ = cp_type3_packet(CP_SET_CONSTANT, 3);
	*cmds++ = CP_REG(A3XX_VFD_FETCH_INSTR_0_0);
	/* VFD_FETCH_INSTR_0_0 */
	*cmds++ = _SET(VFD_FETCHINSTRUCTIONS_FETCHSIZE, 11) |
		_SET(VFD_FETCHINSTRUCTIONS_BUFSTRIDE, 12) |
		_SET(VFD_FETCHINSTRUCTIONS_STEPRATE, 1);
	/* VFD_FETCH_INSTR_1_0 */
	*cmds++ = _SET(VFD_BASEADDR_BASEADDR,
		shadow->quad_vertices.gpuaddr);

	*cmds++ = cp_type3_packet(CP_SET_CONSTANT, 2);
	*cmds++ = CP_REG(A3XX_VFD_DECODE_INSTR_0);
	/* VFD_DECODE_INSTR_0 */
	*cmds++ = _SET(VFD_DECODEINSTRUCTIONS_WRITEMASK, 0x0F) |
		_SET(VFD_DECODEINSTRUCTIONS_CONSTFILL, 1) |
		_SET(VFD_DECODEINSTRUCTIONS_FORMAT, 2) |
		_SET(VFD_DECODEINSTRUCTIONS_SHIFTCNT, 12) |
		_SET(VFD_DECODEINSTRUCTIONS_LASTCOMPVALID, 1);

	*cmds++ = cp_type3_packet(CP_SET_CONSTANT, 5);
	*cmds++ = CP_REG(A3XX_HLSQ_CONTROL_0_REG);
	/* HLSQ_CONTROL_0_REG */
	*cmds++ = _SET(HLSQ_CTRL0REG_FSTHREADSIZE, HLSQ_FOUR_PIX_QUADS) |
		_SET(HLSQ_CTRL0REG_FSSUPERTHREADENABLE, 1) |
		_SET(HLSQ_CTRL0REG_RESERVED2, 1) |
		_SET(HLSQ_CTRL0REG_SPCONSTFULLUPDATE, 1);
	/* HLSQ_CONTROL_1_REG */
	*cmds++ = _SET(HLSQ_CTRL1REG_VSTHREADSIZE, HLSQ_TWO_VTX_QUADS) |
		_SET(HLSQ_CTRL1REG_VSSUPERTHREADENABLE, 1);
	/* HLSQ_CONTROL_2_REG */
	*cmds++ = _SET(HLSQ_CTRL2REG_PRIMALLOCTHRESHOLD, 31);
	/* HLSQ_CONTROL_3_REG */
	*cmds++ = 0x00000000;

	*cmds++ = cp_type3_packet(CP_SET_CONSTANT, 5);
	*cmds++ = CP_REG(A3XX_HLSQ_VS_CONTROL_REG);
	/* HLSQ_VS_CONTROL_REG */
	*cmds++ = _SET(HLSQ_VSCTRLREG_VSINSTRLENGTH, 1);
	/* HLSQ_FS_CONTROL_REG */
	*cmds++ = _SET(HLSQ_FSCTRLREG_FSCONSTLENGTH, 1) |
		_SET(HLSQ_FSCTRLREG_FSCONSTSTARTOFFSET, 128) |
		_SET(HLSQ_FSCTRLREG_FSINSTRLENGTH, 1);
	/* HLSQ_CONST_VSPRESV_RANGE_REG */
	*cmds++ = 0x00000000;
	/* HLSQ_CONST_FSPRESV_RANGE_REQ */
	*cmds++ = _SET(HLSQ_CONSTFSPRESERVEDRANGEREG_STARTENTRY, 32) |
		_SET(HLSQ_CONSTFSPRESERVEDRANGEREG_ENDENTRY, 32);

	*cmds++ = cp_type3_packet(CP_SET_CONSTANT, 2);
	*cmds++ = CP_REG(A3XX_SP_FS_LENGTH_REG);
	/* SP_FS_LENGTH_REG */
	*cmds++ = _SET(SP_SHADERLENGTH_LEN, 1);

	*cmds++ = cp_type3_packet(CP_SET_CONSTANT, 2);
	*cmds++ = CP_REG(A3XX_SP_SP_CTRL_REG);
	/* SP_SP_CTRL_REG */
	*cmds++ = _SET(SP_SPCTRLREG_SLEEPMODE, 1) |
		_SET(SP_SPCTRLREG_LOMODE, 1);

	*cmds++ = cp_type3_packet(CP_SET_CONSTANT, 12);
	*cmds++ = CP_REG(A3XX_SP_VS_CTRL_REG0);
	/* SP_VS_CTRL_REG0 */
	*cmds++ = _SET(SP_VSCTRLREG0_VSTHREADMODE, SP_MULTI) |
		_SET(SP_VSCTRLREG0_VSINSTRBUFFERMODE, SP_BUFFER_MODE) |
		_SET(SP_VSCTRLREG0_VSICACHEINVALID, 1) |
		_SET(SP_VSCTRLREG0_VSFULLREGFOOTPRINT, 1) |
		_SET(SP_VSCTRLREG0_VSTHREADSIZE, SP_TWO_VTX_QUADS) |
		_SET(SP_VSCTRLREG0_VSSUPERTHREADMODE, 1) |
		_SET(SP_VSCTRLREG0_VSLENGTH, 1);
	/* SP_VS_CTRL_REG1 */
	*cmds++ = _SET(SP_VSCTRLREG1_VSINITIALOUTSTANDING, 4);
	/* SP_VS_PARAM_REG */
	*cmds++ = _SET(SP_VSPARAMREG_PSIZEREGID, 252);
	/* SP_VS_OUT_REG_0 */
	*cmds++ = 0x00000000;
	/* SP_VS_OUT_REG_1 */
	*cmds++ = 0x00000000;
	/* SP_VS_OUT_REG_2 */
	*cmds++ = 0x00000000;
	/* SP_VS_OUT_REG_3 */
	*cmds++ = 0x00000000;
	/* SP_VS_OUT_REG_4 */
	*cmds++ = 0x00000000;
	/* SP_VS_OUT_REG_5 */
	*cmds++ = 0x00000000;
	/* SP_VS_OUT_REG_6 */
	*cmds++ = 0x00000000;
	/* SP_VS_OUT_REG_7 */
	*cmds++ = 0x00000000;

	*cmds++ = cp_type3_packet(CP_SET_CONSTANT, 7);
	*cmds++ = CP_REG(A3XX_SP_VS_VPC_DST_REG_0);
	/* SP_VS_VPC_DST_REG_0 */
	*cmds++ = 0x00000000;
	/* SP_VS_VPC_DST_REG_1 */
	*cmds++ = 0x00000000;
	/* SP_VS_VPC_DST_REG_2 */
	*cmds++ = 0x00000000;
	/* SP_VS_VPC_DST_REG_3 */
	*cmds++ = 0x00000000;
	/* SP_VS_OBJ_OFFSET_REG */
	*cmds++ = 0x00000000;
	/* SP_VS_OBJ_START_REG */
	*cmds++ = 0x00000000;

	*cmds++ = cp_type3_packet(CP_SET_CONSTANT, 6);
	*cmds++ = CP_REG(A3XX_SP_VS_LENGTH_REG);
	/* SP_VS_LENGTH_REG */
	*cmds++ = _SET(SP_SHADERLENGTH_LEN, 1);
	/* SP_FS_CTRL_REG0 */
	*cmds++ = _SET(SP_FSCTRLREG0_FSTHREADMODE, SP_MULTI) |
		_SET(SP_FSCTRLREG0_FSINSTRBUFFERMODE, SP_BUFFER_MODE) |
		_SET(SP_FSCTRLREG0_FSICACHEINVALID, 1) |
		_SET(SP_FSCTRLREG0_FSHALFREGFOOTPRINT, 1) |
		_SET(SP_FSCTRLREG0_FSINOUTREGOVERLAP, 1) |
		_SET(SP_FSCTRLREG0_FSTHREADSIZE, SP_FOUR_PIX_QUADS) |
		_SET(SP_FSCTRLREG0_FSSUPERTHREADMODE, 1) |
		_SET(SP_FSCTRLREG0_FSLENGTH, 1);
	/* SP_FS_CTRL_REG1 */
	*cmds++ = _SET(SP_FSCTRLREG1_FSCONSTLENGTH, 1) |
		_SET(SP_FSCTRLREG1_HALFPRECVAROFFSET, 63);
	/* SP_FS_OBJ_OFFSET_REG */
	*cmds++ = _SET(SP_OBJOFFSETREG_CONSTOBJECTSTARTOFFSET, 128) |
		_SET(SP_OBJOFFSETREG_SHADEROBJOFFSETINIC, 127);
	/* SP_FS_OBJ_START_REG */
	*cmds++ = 0x00000000;

	*cmds++ = cp_type3_packet(CP_SET_CONSTANT, 3);
	*cmds++ = CP_REG(A3XX_SP_FS_FLAT_SHAD_MODE_REG_0);
	/* SP_FS_FLAT_SHAD_MODE_REG_0 */
	*cmds++ = 0x00000000;
	/* SP_FS_FLAT_SHAD_MODE_REG_1 */
	*cmds++ = 0x00000000;

	*cmds++ = cp_type3_packet(CP_SET_CONSTANT, 2);
	*cmds++ = CP_REG(A3XX_SP_FS_OUTPUT_REG);
	/* SP_FS_OUTPUT_REG */
	*cmds++ = _SET(SP_IMAGEOUTPUTREG_DEPTHOUTMODE, SP_PIXEL_BASED);

	*cmds++ = cp_type3_packet(CP_SET_CONSTANT, 5);
	*cmds++ = CP_REG(A3XX_SP_FS_MRT_REG_0);
	/* SP_FS_MRT_REG_0 */
	*cmds++ = _SET(SP_FSMRTREG_PRECISION, 1);

	/* SP_FS_MRT_REG_1 */
	*cmds++ = 0x00000000;
	/* SP_FS_MRT_REG_2 */
	*cmds++ = 0x00000000;
	/* SP_FS_MRT_REG_3 */
	*cmds++ = 0x00000000;

	*cmds++ = cp_type3_packet(CP_SET_CONSTANT, 11);
	*cmds++ = CP_REG(A3XX_VPC_ATTR);
	/* VPC_ATTR */
	*cmds++ = _SET(VPC_VPCATTR_THRHDASSIGN, 1) |
		_SET(VPC_VPCATTR_LMSIZE, 1);
	/* VPC_PACK */
	*cmds++ = 0x00000000;
	/* VPC_VARRYING_INTERUPT_MODE_0 */
	*cmds++ = 0x00000000;
	/* VPC_VARRYING_INTERUPT_MODE_1 */
	*cmds++ = 0x00000000;
	/* VPC_VARRYING_INTERUPT_MODE_2 */
	*cmds++ = 0x00000000;
	/* VPC_VARRYING_INTERUPT_MODE_3 */
	*cmds++ = 0x00000000;
	/* VPC_VARYING_PS_REPL_MODE_0 */
	*cmds++ = 0x00000000;
	/* VPC_VARYING_PS_REPL_MODE_1 */
	*cmds++ = 0x00000000;
	/* VPC_VARYING_PS_REPL_MODE_2 */
	*cmds++ = 0x00000000;
	/* VPC_VARYING_PS_REPL_MODE_3 */
	*cmds++ = 0x00000000;

	*cmds++ = cp_type3_packet(CP_LOAD_STATE, 10);
	*cmds++ = (0 << CP_LOADSTATE_DSTOFFSET_SHIFT)
		| (HLSQ_DIRECT << CP_LOADSTATE_STATESRC_SHIFT)
		| (HLSQ_BLOCK_ID_SP_VS << CP_LOADSTATE_STATEBLOCKID_SHIFT)
		| (1 << CP_LOADSTATE_NUMOFUNITS_SHIFT);
	*cmds++ = (HLSQ_SP_VS_INSTR << CP_LOADSTATE_STATETYPE_SHIFT)
		| (0 << CP_LOADSTATE_EXTSRCADDR_SHIFT);

	/* (sy)(rpt3)mov.f32f32 r0.y, (r)r1.y; */
	*cmds++ = 0x00000000; *cmds++ = 0x13001000;
	/* end; */
	*cmds++ = 0x00000000; *cmds++ = 0x00000000;
	/* nop; */
	*cmds++ = 0x00000000; *cmds++ = 0x00000000;
	/* nop; */
	*cmds++ = 0x00000000; *cmds++ = 0x00000000;


	*cmds++ = cp_type0_packet(A3XX_VFD_PERFCOUNTER0_SELECT, 1);
	*cmds++ = 0x00000000;

	*cmds++ = cp_type3_packet(CP_WAIT_FOR_IDLE, 1);
	*cmds++ = 0x00000000;

	*cmds++ = cp_type3_packet(CP_LOAD_STATE, 10);
	*cmds++ = (0 << CP_LOADSTATE_DSTOFFSET_SHIFT)
		| (HLSQ_DIRECT << CP_LOADSTATE_STATESRC_SHIFT)
		| (HLSQ_BLOCK_ID_SP_FS << CP_LOADSTATE_STATEBLOCKID_SHIFT)
		| (1 << CP_LOADSTATE_NUMOFUNITS_SHIFT);
	*cmds++ = (HLSQ_SP_FS_INSTR << CP_LOADSTATE_STATETYPE_SHIFT)
		| (0 << CP_LOADSTATE_EXTSRCADDR_SHIFT);

	/* (sy)(rpt3)mov.f32f32 r0.y, (r)c0.x; */
	*cmds++ = 0x00000000; *cmds++ = 0x30201b00;
	/* end; */
	*cmds++ = 0x00000000; *cmds++ = 0x03000000;
	/* nop; */
	*cmds++ = 0x00000000; *cmds++ = 0x00000000;
	/* nop; */
	*cmds++ = 0x00000000; *cmds++ = 0x00000000;



	*cmds++ = cp_type0_packet(A3XX_VFD_PERFCOUNTER0_SELECT, 1);
	*cmds++ = 0x00000000;

	*cmds++ = cp_type3_packet(CP_WAIT_FOR_IDLE, 1);
	*cmds++ = 0x00000000;


	*cmds++ = cp_type3_packet(CP_SET_CONSTANT, 2);
	*cmds++ = CP_REG(A3XX_RB_MSAA_CONTROL);
	/* RB_MSAA_CONTROL */
	*cmds++ = _SET(RB_MSAACONTROL_MSAA_DISABLE, 1) |
		_SET(RB_MSAACONTROL_SAMPLE_MASK, 0xFFFF);

	*cmds++ = cp_type3_packet(CP_SET_CONSTANT, 2);
	*cmds++ = CP_REG(A3XX_RB_DEPTH_CONTROL);
	/* RB_DEPTH_CONTROL */
	*cmds++ = _SET(RB_DEPTHCONTROL_Z_TEST_FUNC, RB_FRAG_NEVER);

	*cmds++ = cp_type3_packet(CP_SET_CONSTANT, 2);
	*cmds++ = CP_REG(A3XX_RB_STENCIL_CONTROL);
	/* RB_STENCIL_CONTROL */
	*cmds++ = _SET(RB_STENCILCONTROL_STENCIL_FUNC, RB_REF_NEVER) |
		_SET(RB_STENCILCONTROL_STENCIL_FAIL, RB_STENCIL_KEEP) |
		_SET(RB_STENCILCONTROL_STENCIL_ZPASS, RB_STENCIL_KEEP) |
		_SET(RB_STENCILCONTROL_STENCIL_ZFAIL, RB_STENCIL_KEEP) |
		_SET(RB_STENCILCONTROL_STENCIL_FUNC_BF, RB_REF_NEVER) |
		_SET(RB_STENCILCONTROL_STENCIL_FAIL_BF, RB_STENCIL_KEEP) |
		_SET(RB_STENCILCONTROL_STENCIL_ZPASS_BF, RB_STENCIL_KEEP) |
		_SET(RB_STENCILCONTROL_STENCIL_ZFAIL_BF, RB_STENCIL_KEEP);

	*cmds++ = cp_type3_packet(CP_SET_CONSTANT, 2);
	*cmds++ = CP_REG(A3XX_GRAS_SU_MODE_CONTROL);
	/* GRAS_SU_MODE_CONTROL */
	*cmds++ = 0x00000000;

	*cmds++ = cp_type3_packet(CP_SET_CONSTANT, 2);
	*cmds++ = CP_REG(A3XX_RB_MRT_CONTROL0);
	/* RB_MRT_CONTROL0 */
	*cmds++ = _SET(RB_MRTCONTROL_READ_DEST_ENABLE, 1) |
		_SET(RB_MRTCONTROL_ROP_CODE, 12) |
		_SET(RB_MRTCONTROL_DITHER_MODE, RB_DITHER_ALWAYS) |
		_SET(RB_MRTCONTROL_COMPONENT_ENABLE, 0xF);

	*cmds++ = cp_type3_packet(CP_SET_CONSTANT, 3);
	*cmds++ = CP_REG(A3XX_RB_MRT_BLEND_CONTROL0);
	/* RB_MRT_BLEND_CONTROL0 */
	*cmds++ = _SET(RB_MRTBLENDCONTROL_RGB_SRC_FACTOR, RB_FACTOR_ONE) |
		_SET(RB_MRTBLENDCONTROL_RGB_BLEND_OPCODE, RB_BLEND_OP_ADD) |
		_SET(RB_MRTBLENDCONTROL_RGB_DEST_FACTOR, RB_FACTOR_ZERO) |
		_SET(RB_MRTBLENDCONTROL_ALPHA_SRC_FACTOR, RB_FACTOR_ONE) |
		_SET(RB_MRTBLENDCONTROL_ALPHA_DEST_FACTOR, RB_FACTOR_ZERO) |
		_SET(RB_MRTBLENDCONTROL_CLAMP_ENABLE, 1);
	/* RB_MRT_CONTROL1 */
	*cmds++ = _SET(RB_MRTCONTROL_READ_DEST_ENABLE, 1) |
		_SET(RB_MRTCONTROL_DITHER_MODE, RB_DITHER_DISABLE) |
		_SET(RB_MRTCONTROL_COMPONENT_ENABLE, 0xF);

	*cmds++ = cp_type3_packet(CP_SET_CONSTANT, 3);
	*cmds++ = CP_REG(A3XX_RB_MRT_BLEND_CONTROL1);
	/* RB_MRT_BLEND_CONTROL1 */
	*cmds++ = _SET(RB_MRTBLENDCONTROL_RGB_SRC_FACTOR, RB_FACTOR_ONE) |
		_SET(RB_MRTBLENDCONTROL_RGB_BLEND_OPCODE, RB_BLEND_OP_ADD) |
		_SET(RB_MRTBLENDCONTROL_RGB_DEST_FACTOR, RB_FACTOR_ZERO) |
		_SET(RB_MRTBLENDCONTROL_ALPHA_SRC_FACTOR, RB_FACTOR_ONE) |
		_SET(RB_MRTBLENDCONTROL_ALPHA_DEST_FACTOR, RB_FACTOR_ZERO) |
		_SET(RB_MRTBLENDCONTROL_CLAMP_ENABLE, 1);
	/* RB_MRT_CONTROL2 */
	*cmds++ = _SET(RB_MRTCONTROL_READ_DEST_ENABLE, 1) |
		_SET(RB_MRTCONTROL_DITHER_MODE, RB_DITHER_DISABLE) |
		_SET(RB_MRTCONTROL_COMPONENT_ENABLE, 0xF);

	*cmds++ = cp_type3_packet(CP_SET_CONSTANT, 3);
	*cmds++ = CP_REG(A3XX_RB_MRT_BLEND_CONTROL2);
	/* RB_MRT_BLEND_CONTROL2 */
	*cmds++ = _SET(RB_MRTBLENDCONTROL_RGB_SRC_FACTOR, RB_FACTOR_ONE) |
		_SET(RB_MRTBLENDCONTROL_RGB_BLEND_OPCODE, RB_BLEND_OP_ADD) |
		_SET(RB_MRTBLENDCONTROL_RGB_DEST_FACTOR, RB_FACTOR_ZERO) |
		_SET(RB_MRTBLENDCONTROL_ALPHA_SRC_FACTOR, RB_FACTOR_ONE) |
		_SET(RB_MRTBLENDCONTROL_ALPHA_DEST_FACTOR, RB_FACTOR_ZERO) |
		_SET(RB_MRTBLENDCONTROL_CLAMP_ENABLE, 1);
	/* RB_MRT_CONTROL3 */
	*cmds++ = _SET(RB_MRTCONTROL_READ_DEST_ENABLE, 1) |
		_SET(RB_MRTCONTROL_DITHER_MODE, RB_DITHER_DISABLE) |
		_SET(RB_MRTCONTROL_COMPONENT_ENABLE, 0xF);

	*cmds++ = cp_type3_packet(CP_SET_CONSTANT, 2);
	*cmds++ = CP_REG(A3XX_RB_MRT_BLEND_CONTROL3);
	/* RB_MRT_BLEND_CONTROL3 */
	*cmds++ = _SET(RB_MRTBLENDCONTROL_RGB_SRC_FACTOR, RB_FACTOR_ONE) |
		_SET(RB_MRTBLENDCONTROL_RGB_BLEND_OPCODE, RB_BLEND_OP_ADD) |
		_SET(RB_MRTBLENDCONTROL_RGB_DEST_FACTOR, RB_FACTOR_ZERO) |
		_SET(RB_MRTBLENDCONTROL_ALPHA_SRC_FACTOR, RB_FACTOR_ONE) |
		_SET(RB_MRTBLENDCONTROL_ALPHA_DEST_FACTOR, RB_FACTOR_ZERO) |
		_SET(RB_MRTBLENDCONTROL_CLAMP_ENABLE, 1);

	*cmds++ = cp_type3_packet(CP_SET_CONSTANT, 5);
	*cmds++ = CP_REG(A3XX_VFD_INDEX_MIN);
	/* VFD_INDEX_MIN */
	*cmds++ = 0x00000000;
	/* VFD_INDEX_MAX */
	*cmds++ = 0x155;
	/* VFD_INSTANCEID_OFFSET */
	*cmds++ = 0x00000000;
	/* VFD_INDEX_OFFSET */
	*cmds++ = 0x00000000;

	*cmds++ = cp_type3_packet(CP_SET_CONSTANT, 2);
	*cmds++ = CP_REG(A3XX_VFD_VS_THREADING_THRESHOLD);
	/* VFD_VS_THREADING_THRESHOLD */
	*cmds++ = _SET(VFD_THREADINGTHRESHOLD_REGID_THRESHOLD, 15) |
		_SET(VFD_THREADINGTHRESHOLD_REGID_VTXCNT, 252);

	*cmds++ = cp_type3_packet(CP_SET_CONSTANT, 2);
	*cmds++ = CP_REG(A3XX_TPL1_TP_VS_TEX_OFFSET);
	/* TPL1_TP_VS_TEX_OFFSET */
	*cmds++ = 0;

	*cmds++ = cp_type3_packet(CP_SET_CONSTANT, 2);
	*cmds++ = CP_REG(A3XX_TPL1_TP_FS_TEX_OFFSET);
	/* TPL1_TP_FS_TEX_OFFSET */
	*cmds++ = _SET(TPL1_TPTEXOFFSETREG_SAMPLEROFFSET, 16) |
		_SET(TPL1_TPTEXOFFSETREG_MEMOBJOFFSET, 16) |
		_SET(TPL1_TPTEXOFFSETREG_BASETABLEPTR, 224);

	*cmds++ = cp_type3_packet(CP_SET_CONSTANT, 2);
	*cmds++ = CP_REG(A3XX_PC_PRIM_VTX_CNTL);
	/* PC_PRIM_VTX_CNTL */
	*cmds++ = _SET(PC_PRIM_VTX_CONTROL_POLYMODE_FRONT_PTYPE,
		PC_DRAW_TRIANGLES) |
		_SET(PC_PRIM_VTX_CONTROL_POLYMODE_BACK_PTYPE,
		PC_DRAW_TRIANGLES) |
		_SET(PC_PRIM_VTX_CONTROL_PROVOKING_VTX_LAST, 1);

	*cmds++ = cp_type3_packet(CP_SET_CONSTANT, 3);
	*cmds++ = CP_REG(A3XX_GRAS_SC_WINDOW_SCISSOR_TL);
	/* GRAS_SC_WINDOW_SCISSOR_TL */
	*cmds++ = 0x00000000;
	/* GRAS_SC_WINDOW_SCISSOR_BR */
	*cmds++ = _SET(GRAS_SC_WINDOW_SCISSOR_BR_BR_X, shadow->width - 1) |
		_SET(GRAS_SC_WINDOW_SCISSOR_BR_BR_Y, shadow->height - 1);

	*cmds++ = cp_type3_packet(CP_SET_CONSTANT, 3);
	*cmds++ = CP_REG(A3XX_GRAS_SC_SCREEN_SCISSOR_TL);
	/* GRAS_SC_SCREEN_SCISSOR_TL */
	*cmds++ = 0x00000000;
	/* GRAS_SC_SCREEN_SCISSOR_BR */
	*cmds++ = _SET(GRAS_SC_SCREEN_SCISSOR_BR_BR_X, shadow->width - 1) |
		_SET(GRAS_SC_SCREEN_SCISSOR_BR_BR_Y, shadow->height - 1);

	*cmds++ = cp_type3_packet(CP_SET_CONSTANT, 5);
	*cmds++ = CP_REG(A3XX_GRAS_CL_VPORT_XOFFSET);
	/* GRAS_CL_VPORT_XOFFSET */
	*cmds++ = 0x00000000;
	/* GRAS_CL_VPORT_XSCALE */
	*cmds++ = _SET(GRAS_CL_VPORT_XSCALE_VPORT_XSCALE, 0x3f800000);
	/* GRAS_CL_VPORT_YOFFSET */
	*cmds++ = 0x00000000;
	/* GRAS_CL_VPORT_YSCALE */
	*cmds++ = _SET(GRAS_CL_VPORT_YSCALE_VPORT_YSCALE, 0x3f800000);

	*cmds++ = cp_type3_packet(CP_SET_CONSTANT, 3);
	*cmds++ = CP_REG(A3XX_GRAS_CL_VPORT_ZOFFSET);
	/* GRAS_CL_VPORT_ZOFFSET */
	*cmds++ = 0x00000000;
	/* GRAS_CL_VPORT_ZSCALE */
	*cmds++ = _SET(GRAS_CL_VPORT_ZSCALE_VPORT_ZSCALE, 0x3f800000);

	*cmds++ = cp_type3_packet(CP_SET_CONSTANT, 2);
	*cmds++ = CP_REG(A3XX_GRAS_CL_CLIP_CNTL);
	/* GRAS_CL_CLIP_CNTL */
	*cmds++ = _SET(GRAS_CL_CLIP_CNTL_CLIP_DISABLE, 1) |
		_SET(GRAS_CL_CLIP_CNTL_ZFAR_CLIP_DISABLE, 1) |
		_SET(GRAS_CL_CLIP_CNTL_VP_CLIP_CODE_IGNORE, 1) |
		_SET(GRAS_CL_CLIP_CNTL_VP_XFORM_DISABLE, 1) |
		_SET(GRAS_CL_CLIP_CNTL_PERSP_DIVISION_DISABLE, 1);

	*cmds++ = cp_type3_packet(CP_SET_CONSTANT, 2);
	*cmds++ = CP_REG(A3XX_GRAS_CL_GB_CLIP_ADJ);
	/* GRAS_CL_GB_CLIP_ADJ */
	*cmds++ = 0x00000000;

	*cmds++ = cp_type3_packet(CP_WAIT_FOR_IDLE, 1);
	*cmds++ = 0x00000000;


	/* oxili_generate_context_roll_packets */
	*cmds++ = cp_type0_packet(A3XX_SP_VS_CTRL_REG0, 1);
	*cmds++ = 0x00000400;

	*cmds++ = cp_type0_packet(A3XX_SP_FS_CTRL_REG0, 1);
	*cmds++ = 0x00000400;

	*cmds++ = cp_type0_packet(A3XX_SP_VS_PVT_MEM_SIZE_REG, 1);
	*cmds++ = 0x00008000; /* SP_VS_MEM_SIZE_REG */

	*cmds++ = cp_type0_packet(A3XX_SP_FS_PVT_MEM_SIZE_REG, 1);
	*cmds++ = 0x00008000; /* SP_FS_MEM_SIZE_REG */

	/* Clear cache invalidate bit when re-loading the shader control regs */
	*cmds++ = cp_type0_packet(A3XX_SP_VS_CTRL_REG0, 1);
	*cmds++ = _SET(SP_VSCTRLREG0_VSTHREADMODE, SP_MULTI) |
		_SET(SP_VSCTRLREG0_VSINSTRBUFFERMODE, SP_BUFFER_MODE) |
		_SET(SP_VSCTRLREG0_VSFULLREGFOOTPRINT, 1) |
		_SET(SP_VSCTRLREG0_VSTHREADSIZE, SP_TWO_VTX_QUADS) |
		_SET(SP_VSCTRLREG0_VSSUPERTHREADMODE, 1) |
		_SET(SP_VSCTRLREG0_VSLENGTH, 1);

	*cmds++ = cp_type0_packet(A3XX_SP_FS_CTRL_REG0, 1);
	*cmds++ = _SET(SP_FSCTRLREG0_FSTHREADMODE, SP_MULTI) |
		_SET(SP_FSCTRLREG0_FSINSTRBUFFERMODE, SP_BUFFER_MODE) |
		_SET(SP_FSCTRLREG0_FSHALFREGFOOTPRINT, 1) |
		_SET(SP_FSCTRLREG0_FSINOUTREGOVERLAP, 1) |
		_SET(SP_FSCTRLREG0_FSTHREADSIZE, SP_FOUR_PIX_QUADS) |
		_SET(SP_FSCTRLREG0_FSSUPERTHREADMODE, 1) |
		_SET(SP_FSCTRLREG0_FSLENGTH, 1);

	*cmds++ = cp_type0_packet(A3XX_SP_VS_PVT_MEM_SIZE_REG, 1);
	*cmds++ = 0x00000000;		 /* SP_VS_MEM_SIZE_REG */

	*cmds++ = cp_type0_packet(A3XX_SP_FS_PVT_MEM_SIZE_REG, 1);
	*cmds++ = 0x00000000;		 /* SP_FS_MEM_SIZE_REG */

	/* end oxili_generate_context_roll_packets */

	/*
	 * Resolve using two draw calls with a dummy register
	 * write in between. This is a HLM workaround
	 * that should be removed later.
	 */
	*cmds++ = cp_type3_packet(CP_DRAW_INDX_2, 6);
	*cmds++ = 0x00000000; /* Viz query info */
	*cmds++ = BUILD_PC_DRAW_INITIATOR(PC_DI_PT_TRILIST,
					  PC_DI_SRC_SEL_IMMEDIATE,
					  PC_DI_INDEX_SIZE_32_BIT,
					  PC_DI_IGNORE_VISIBILITY);
	*cmds++ = 0x00000003; /* Num indices */
	*cmds++ = 0x00000000; /* Index 0 */
	*cmds++ = 0x00000001; /* Index 1 */
	*cmds++ = 0x00000002; /* Index 2 */

	*cmds++ = cp_type3_packet(CP_SET_CONSTANT, 2);
	*cmds++ = CP_REG(A3XX_HLSQ_CL_CONTROL_0_REG);
	*cmds++ = 0x00000000;

	*cmds++ = cp_type3_packet(CP_DRAW_INDX_2, 6);
	*cmds++ = 0x00000000; /* Viz query info */
	*cmds++ = BUILD_PC_DRAW_INITIATOR(PC_DI_PT_TRILIST,
					  PC_DI_SRC_SEL_IMMEDIATE,
					  PC_DI_INDEX_SIZE_32_BIT,
					  PC_DI_IGNORE_VISIBILITY);
	*cmds++ = 0x00000003; /* Num indices */
	*cmds++ = 0x00000002; /* Index 0 */
	*cmds++ = 0x00000001; /* Index 1 */
	*cmds++ = 0x00000003; /* Index 2 */

	*cmds++ = cp_type3_packet(CP_SET_CONSTANT, 2);
	*cmds++ = CP_REG(A3XX_HLSQ_CL_CONTROL_0_REG);
	*cmds++ = 0x00000000;

	*cmds++ = cp_type3_packet(CP_WAIT_FOR_IDLE, 1);
	*cmds++ = 0x00000000;

	/* Create indirect buffer command for above command sequence */
	create_ib1(drawctxt, shadow->gmem_save, start, cmds);

	return cmds;
}
static void build_shader_save_cmds(struct adreno_device *adreno_dev,
				   struct adreno_context *drawctxt)
{
	unsigned int *cmd = tmp_ctx.cmd;
	unsigned int *start;

	/* Reserve space for boolean values used for COND_EXEC packet */
	drawctxt->cond_execs[0].hostptr = cmd;
	drawctxt->cond_execs[0].gpuaddr = virt2gpu(cmd, &drawctxt->gpustate);
	*cmd++ = 0;
	drawctxt->cond_execs[1].hostptr = cmd;
	drawctxt->cond_execs[1].gpuaddr = virt2gpu(cmd, &drawctxt->gpustate);
	*cmd++ = 0;

	drawctxt->shader_save_commands[0].hostptr = cmd;
	drawctxt->shader_save_commands[0].gpuaddr =
	    virt2gpu(cmd, &drawctxt->gpustate);
	*cmd++ = 0;
	drawctxt->shader_save_commands[1].hostptr = cmd;
	drawctxt->shader_save_commands[1].gpuaddr =
	    virt2gpu(cmd, &drawctxt->gpustate);
	*cmd++ = 0;

	start = cmd;

	/* Save vertex shader */

	*cmd++ = cp_type3_packet(CP_COND_EXEC, 4);
	*cmd++ = drawctxt->cond_execs[0].gpuaddr >> 2;
	*cmd++ = drawctxt->cond_execs[0].gpuaddr >> 2;
	*cmd++ = 0x0000FFFF;
	*cmd++ = 3;		/* EXEC_COUNT */

	*cmd++ = cp_type3_packet(CP_REG_TO_MEM, 2);
	drawctxt->shader_save_commands[2].hostptr = cmd;
	drawctxt->shader_save_commands[2].gpuaddr =
	    virt2gpu(cmd, &drawctxt->gpustate);
	/*
	   From fixup:

	   dwords = SP_VS_CTRL_REG0.VS_LENGTH * 8

	   From regspec:
	   SP_VS_CTRL_REG0.VS_LENGTH [31:24]: VS length, unit = 256bits.
	   If bit31 is 1, it means overflow
	   or any long shader.

	   src = (HLSQ_SHADOW_BASE + 0x1000)/4
	 */
	*cmd++ = 0;	/*(dwords << REG_TO_MEM_LOOP_COUNT_SHIFT) | src */
	*cmd++ = (drawctxt->gpustate.gpuaddr + SHADER_OFFSET) & 0xfffffffc;

	/* Save fragment shader */
	*cmd++ = cp_type3_packet(CP_COND_EXEC, 4);
	*cmd++ = drawctxt->cond_execs[1].gpuaddr >> 2;
	*cmd++ = drawctxt->cond_execs[1].gpuaddr >> 2;
	*cmd++ = 0x0000FFFF;
	*cmd++ = 3;		/* EXEC_COUNT */

	*cmd++ = cp_type3_packet(CP_REG_TO_MEM, 2);
	drawctxt->shader_save_commands[3].hostptr = cmd;
	drawctxt->shader_save_commands[3].gpuaddr =
	    virt2gpu(cmd, &drawctxt->gpustate);
	/*
	   From fixup:

	   dwords = SP_FS_CTRL_REG0.FS_LENGTH * 8

	   From regspec:
	   SP_FS_CTRL_REG0.FS_LENGTH [31:24]: FS length, unit = 256bits.
	   If bit31 is 1, it means overflow
	   or any long shader.

	   fs_offset = SP_FS_OBJ_OFFSET_REG.SHADEROBJOFFSETINIC * 32
	   From regspec:

	   SP_FS_OBJ_OFFSET_REG.SHADEROBJOFFSETINIC [31:25]:
	   First instruction of the whole shader will be stored from
	   the offset in instruction cache, unit = 256bits, a cache line.
	   It can start from 0 if no VS available.

	   src = (HLSQ_SHADOW_BASE + 0x1000 + SSIZE + fs_offset)/4
	 */
	*cmd++ = 0;	/*(dwords << REG_TO_MEM_LOOP_COUNT_SHIFT) | src */
	*cmd++ = (drawctxt->gpustate.gpuaddr + SHADER_OFFSET
		  + (SHADER_SHADOW_SIZE / 2)) & 0xfffffffc;

	/* Create indirect buffer command for above command sequence */
	create_ib1(drawctxt, drawctxt->shader_save, start, cmd);

	tmp_ctx.cmd = cmd;
}

/*
 * Make an IB to modify context save IBs with the correct shader instruction
 * and constant sizes and offsets.
 */

static void build_save_fixup_cmds(struct adreno_device *adreno_dev,
				  struct adreno_context *drawctxt)
{
	unsigned int *cmd = tmp_ctx.cmd;
	unsigned int *start = cmd;

	/* Flush HLSQ lazy updates */
	*cmd++ = cp_type3_packet(CP_EVENT_WRITE, 1);
	*cmd++ = 0x7;		/* HLSQ_FLUSH */
	*cmd++ = cp_type3_packet(CP_WAIT_FOR_IDLE, 1);
	*cmd++ = 0;

	*cmd++ = cp_type0_packet(A3XX_UCHE_CACHE_INVALIDATE0_REG, 2);
	*cmd++ = 0x00000000; /* No start addr for full invalidate */
	*cmd++ = (unsigned int)
		UCHE_ENTIRE_CACHE << UCHE_INVALIDATE1REG_ALLORPORTION |
		UCHE_OP_INVALIDATE << UCHE_INVALIDATE1REG_OPCODE |
		0; /* No end addr for full invalidate */

	/* Make sure registers are flushed */
	*cmd++ = cp_type3_packet(CP_CONTEXT_UPDATE, 1);
	*cmd++ = 0;

#ifdef GSL_CONTEXT_SWITCH_CPU_SYNC

	/* Save shader sizes */
	*cmd++ = cp_type3_packet(CP_REG_TO_MEM, 2);
	*cmd++ = A3XX_SP_VS_CTRL_REG0;
	*cmd++ = drawctxt->shader_save_commands[2].gpuaddr;

	*cmd++ = cp_type3_packet(CP_REG_TO_MEM, 2);
	*cmd++ = A3XX_SP_FS_CTRL_REG0;
	*cmd++ = drawctxt->shader_save_commands[3].gpuaddr;

	/* Save shader offsets */
	*cmd++ = cp_type3_packet(CP_REG_TO_MEM, 2);
	*cmd++ = A3XX_SP_FS_OBJ_OFFSET_REG;
	*cmd++ = drawctxt->shader_save_commands[1].gpuaddr;

	/* Save constant sizes */
	*cmd++ = cp_type3_packet(CP_REG_TO_MEM, 2);
	*cmd++ = A3XX_SP_VS_CTRL_REG1;
	*cmd++ = drawctxt->constant_save_commands[1].gpuaddr;
	*cmd++ = cp_type3_packet(CP_REG_TO_MEM, 2);
	*cmd++ = A3XX_SP_FS_CTRL_REG1;
	*cmd++ = drawctxt->constant_save_commands[2].gpuaddr;

	/* Save FS constant offset */
	*cmd++ = cp_type3_packet(CP_REG_TO_MEM, 2);
	*cmd++ = A3XX_SP_FS_OBJ_OFFSET_REG;
	*cmd++ = drawctxt->constant_save_commands[0].gpuaddr;


	/* Save VS instruction store mode */
	*cmd++ = cp_type3_packet(CP_REG_TO_MEM, 2);
	*cmd++ = A3XX_SP_VS_CTRL_REG0;
	*cmd++ = drawctxt->cond_execs[0].gpuaddr;

	/* Save FS instruction store mode */
	*cmd++ = cp_type3_packet(CP_REG_TO_MEM, 2);
	*cmd++ = A3XX_SP_FS_CTRL_REG0;
	*cmd++ = drawctxt->cond_execs[1].gpuaddr;
#else

	/* Shader save */
	cmd = rmw_regtomem(cmd, A3XX_SP_VS_CTRL_REG0, 0x7f000000,
			11+REG_TO_MEM_LOOP_COUNT_SHIFT,
			(HLSQ_SHADOW_BASE + 0x1000) / 4,
			drawctxt->shader_save_commands[2].gpuaddr);

	/* CP_SCRATCH_REG2 = (CP_SCRATCH_REG2 & 0x00000000) | SP_FS_CTRL_REG0 */
	*cmd++ = cp_type3_packet(CP_REG_RMW, 3);
	*cmd++ = (1 << 30) | A3XX_CP_SCRATCH_REG2;
	*cmd++ = 0x00000000;	/* AND value */
	*cmd++ = A3XX_SP_FS_CTRL_REG0;	/* OR address */
	/* CP_SCRATCH_REG2 = ( (CP_SCRATCH_REG2 & 0x7f000000) >> 21 )
	   |  ((HLSQ_SHADOW_BASE+0x1000+SSIZE)/4) */
	*cmd++ = cp_type3_packet(CP_REG_RMW, 3);
	*cmd++ = ((11 + REG_TO_MEM_LOOP_COUNT_SHIFT) << 24) |
		A3XX_CP_SCRATCH_REG2;
	*cmd++ = 0x7f000000;	/* AND value */
	*cmd++ = (HLSQ_SHADOW_BASE + 0x1000 + SSIZE) / 4;	/* OR value */

	/*
	 * CP_SCRATCH_REG3 = (CP_SCRATCH_REG3 & 0x00000000) |
	 * SP_FS_OBJ_OFFSET_REG
	 */

	*cmd++ = cp_type3_packet(CP_REG_RMW, 3);
	*cmd++ = (1 << 30) | A3XX_CP_SCRATCH_REG3;
	*cmd++ = 0x00000000;	/* AND value */
	*cmd++ = A3XX_SP_FS_OBJ_OFFSET_REG;	/* OR address */
	/*
	 * CP_SCRATCH_REG3 = ( (CP_SCRATCH_REG3 & 0xfe000000) >> 25 ) |
	 * 0x00000000
	 */
	*cmd++ = cp_type3_packet(CP_REG_RMW, 3);
	*cmd++ = A3XX_CP_SCRATCH_REG3;
	*cmd++ = 0xfe000000;	/* AND value */
	*cmd++ = 0x00000000;	/* OR value */
	/*
	 * CP_SCRATCH_REG2 =  (CP_SCRATCH_REG2 & 0xffffffff) | CP_SCRATCH_REG3
	 */
	*cmd++ = cp_type3_packet(CP_REG_RMW, 3);
	*cmd++ = (1 << 30) | A3XX_CP_SCRATCH_REG2;
	*cmd++ = 0xffffffff;	/* AND value */
	*cmd++ = A3XX_CP_SCRATCH_REG3;	/* OR address */

	*cmd++ = cp_type3_packet(CP_REG_TO_MEM, 2);
	*cmd++ = A3XX_CP_SCRATCH_REG2;
	*cmd++ = drawctxt->shader_save_commands[3].gpuaddr;

	/* Constant save */
	cmd = rmw_regtomem(cmd, A3XX_SP_VS_CTRL_REG1, 0x000003ff,
			   2 + REG_TO_MEM_LOOP_COUNT_SHIFT,
			   (HLSQ_SHADOW_BASE + 0x2000) / 4,
			   drawctxt->constant_save_commands[1].gpuaddr);

	cmd = rmw_regtomem(cmd, A3XX_SP_FS_CTRL_REG1, 0x000003ff,
			   2 + REG_TO_MEM_LOOP_COUNT_SHIFT,
			   (HLSQ_SHADOW_BASE + 0x2000 + SSIZE) / 4,
			   drawctxt->constant_save_commands[2].gpuaddr);

	cmd = rmw_regtomem(cmd, A3XX_SP_FS_OBJ_OFFSET_REG, 0x00ff0000,
			   18, drawctxt->gpustate.gpuaddr & 0xfffffe00,
			   drawctxt->constant_save_commands[2].gpuaddr
			   + sizeof(unsigned int));

	/* Modify constant save conditionals */
	cmd = rmw_regtomem(cmd, A3XX_SP_VS_CTRL_REG1, 0x000003ff,
		0, 0, drawctxt->cond_execs[2].gpuaddr);

	cmd = rmw_regtomem(cmd, A3XX_SP_FS_CTRL_REG1, 0x000003ff,
		0, 0, drawctxt->cond_execs[3].gpuaddr);

	/* Save VS instruction store mode */

	cmd = rmw_regtomem(cmd, A3XX_SP_VS_CTRL_REG0, 0x00000002,
			   31, 0, drawctxt->cond_execs[0].gpuaddr);

	/* Save FS instruction store mode */
	cmd = rmw_regtomem(cmd, A3XX_SP_FS_CTRL_REG0, 0x00000002,
			   31, 0, drawctxt->cond_execs[1].gpuaddr);

#endif

	create_ib1(drawctxt, drawctxt->save_fixup, start, cmd);

	tmp_ctx.cmd = cmd;
}

/****************************************************************************/
/* Functions to build context restore IBs                                   */
/****************************************************************************/

static unsigned int *build_sys2gmem_cmds(struct adreno_device *adreno_dev,
					 struct adreno_context *drawctxt,
					 struct gmem_shadow_t *shadow)
{
	unsigned int *cmds = tmp_ctx.cmd;
	unsigned int *start = cmds;

	*cmds++ = cp_type0_packet(A3XX_RBBM_CLOCK_CTL, 1);
	*cmds++ = adreno_a3xx_rbbm_clock_ctl_default(adreno_dev);

	*cmds++ = cp_type3_packet(CP_SET_CONSTANT, 5);
	*cmds++ = CP_REG(A3XX_HLSQ_CONTROL_0_REG);
	/* HLSQ_CONTROL_0_REG */
	*cmds++ = _SET(HLSQ_CTRL0REG_FSTHREADSIZE, HLSQ_FOUR_PIX_QUADS) |
		_SET(HLSQ_CTRL0REG_FSSUPERTHREADENABLE, 1) |
		_SET(HLSQ_CTRL0REG_SPSHADERRESTART, 1) |
		_SET(HLSQ_CTRL0REG_CHUNKDISABLE, 1) |
		_SET(HLSQ_CTRL0REG_SPCONSTFULLUPDATE, 1);
	/* HLSQ_CONTROL_1_REG */
	*cmds++ = _SET(HLSQ_CTRL1REG_VSTHREADSIZE, HLSQ_TWO_VTX_QUADS) |
		_SET(HLSQ_CTRL1REG_VSSUPERTHREADENABLE, 1);
	/* HLSQ_CONTROL_2_REG */
	*cmds++ = _SET(HLSQ_CTRL2REG_PRIMALLOCTHRESHOLD, 31);
	/* HLSQ_CONTROL3_REG */
	*cmds++ = 0x00000000;

	*cmds++ = cp_type3_packet(CP_SET_CONSTANT, 3);
	*cmds++ = CP_REG(A3XX_RB_MRT_BUF_INFO0);
	/* RB_MRT_BUF_INFO0 */
	*cmds++ = _SET(RB_MRTBUFINFO_COLOR_FORMAT, RB_R8G8B8A8_UNORM) |
		_SET(RB_MRTBUFINFO_COLOR_TILE_MODE, RB_TILINGMODE_32X32) |
		_SET(RB_MRTBUFINFO_COLOR_BUF_PITCH,
		(shadow->gmem_pitch * 4 * 8) / 256);
	/* RB_MRT_BUF_BASE0 */
	*cmds++ = _SET(RB_MRTBUFBASE_COLOR_BUF_BASE, tmp_ctx.gmem_base >> 5);

	/* Texture samplers */
	*cmds++ = cp_type3_packet(CP_LOAD_STATE, 4);
	*cmds++ = (16 << CP_LOADSTATE_DSTOFFSET_SHIFT)
		| (HLSQ_DIRECT << CP_LOADSTATE_STATESRC_SHIFT)
		| (HLSQ_BLOCK_ID_TP_TEX << CP_LOADSTATE_STATEBLOCKID_SHIFT)
		| (1 << CP_LOADSTATE_NUMOFUNITS_SHIFT);
	*cmds++ = (HLSQ_TP_TEX_SAMPLERS << CP_LOADSTATE_STATETYPE_SHIFT)
		| (0 << CP_LOADSTATE_EXTSRCADDR_SHIFT);
	*cmds++ = 0x00000240;
	*cmds++ = 0x00000000;

	*cmds++ = cp_type0_packet(A3XX_VFD_PERFCOUNTER0_SELECT, 1);
	*cmds++ = 0x00000000;

	/* Texture memobjs */
	*cmds++ = cp_type3_packet(CP_LOAD_STATE, 6);
	*cmds++ = (16 << CP_LOADSTATE_DSTOFFSET_SHIFT)
		| (HLSQ_DIRECT << CP_LOADSTATE_STATESRC_SHIFT)
		| (HLSQ_BLOCK_ID_TP_TEX << CP_LOADSTATE_STATEBLOCKID_SHIFT)
		| (1 << CP_LOADSTATE_NUMOFUNITS_SHIFT);
	*cmds++ = (HLSQ_TP_TEX_MEMOBJ << CP_LOADSTATE_STATETYPE_SHIFT)
		| (0 << CP_LOADSTATE_EXTSRCADDR_SHIFT);
	*cmds++ = 0x4cc06880;
	*cmds++ = shadow->height | (shadow->width << 14);
	*cmds++ = (shadow->pitch*4*8) << 9;
	*cmds++ = 0x00000000;

	*cmds++ = cp_type0_packet(A3XX_VFD_PERFCOUNTER0_SELECT, 1);
	*cmds++ = 0x00000000;

	/* Mipmap bases */
	*cmds++ = cp_type3_packet(CP_LOAD_STATE, 16);
	*cmds++ = (224 << CP_LOADSTATE_DSTOFFSET_SHIFT)
		| (HLSQ_DIRECT << CP_LOADSTATE_STATESRC_SHIFT)
		| (HLSQ_BLOCK_ID_TP_MIPMAP << CP_LOADSTATE_STATEBLOCKID_SHIFT)
		| (14 << CP_LOADSTATE_NUMOFUNITS_SHIFT);
	*cmds++ = (HLSQ_TP_MIPMAP_BASE << CP_LOADSTATE_STATETYPE_SHIFT)
		| (0 << CP_LOADSTATE_EXTSRCADDR_SHIFT);
	*cmds++ = shadow->gmemshadow.gpuaddr;
	*cmds++ = 0x00000000;
	*cmds++ = 0x00000000;
	*cmds++ = 0x00000000;
	*cmds++ = 0x00000000;
	*cmds++ = 0x00000000;
	*cmds++ = 0x00000000;
	*cmds++ = 0x00000000;
	*cmds++ = 0x00000000;
	*cmds++ = 0x00000000;
	*cmds++ = 0x00000000;
	*cmds++ = 0x00000000;
	*cmds++ = 0x00000000;
	*cmds++ = 0x00000000;

	*cmds++ = cp_type0_packet(A3XX_VFD_PERFCOUNTER0_SELECT, 1);
	*cmds++ = 0x00000000;

	*cmds++ = cp_type3_packet(CP_SET_CONSTANT, 5);
	*cmds++ = CP_REG(A3XX_HLSQ_VS_CONTROL_REG);
	/* HLSQ_VS_CONTROL_REG */
	*cmds++ = _SET(HLSQ_VSCTRLREG_VSINSTRLENGTH, 1);
	/* HLSQ_FS_CONTROL_REG */
	*cmds++ = _SET(HLSQ_FSCTRLREG_FSCONSTLENGTH, 1) |
		_SET(HLSQ_FSCTRLREG_FSCONSTSTARTOFFSET, 128) |
		_SET(HLSQ_FSCTRLREG_FSINSTRLENGTH, 2);
	/* HLSQ_CONST_VSPRESV_RANGE_REG */
	*cmds++ = 0x00000000;
	/* HLSQ_CONST_FSPRESV_RANGE_REG */
	*cmds++ = 0x00000000;

	*cmds++ = cp_type3_packet(CP_SET_CONSTANT, 2);
	*cmds++ = CP_REG(A3XX_SP_FS_LENGTH_REG);
	/* SP_FS_LENGTH_REG */
	*cmds++ = _SET(SP_SHADERLENGTH_LEN, 2);

	*cmds++ = cp_type3_packet(CP_SET_CONSTANT, 12);
	*cmds++ = CP_REG(A3XX_SP_VS_CTRL_REG0);
	/* SP_VS_CTRL_REG0 */
	*cmds++ = _SET(SP_VSCTRLREG0_VSTHREADMODE, SP_MULTI) |
		_SET(SP_VSCTRLREG0_VSINSTRBUFFERMODE, SP_BUFFER_MODE) |
		_SET(SP_VSCTRLREG0_VSICACHEINVALID, 1) |
		_SET(SP_VSCTRLREG0_VSFULLREGFOOTPRINT, 2) |
		_SET(SP_VSCTRLREG0_VSTHREADSIZE, SP_TWO_VTX_QUADS) |
		_SET(SP_VSCTRLREG0_VSLENGTH, 1);
	/* SP_VS_CTRL_REG1 */
	*cmds++ = _SET(SP_VSCTRLREG1_VSINITIALOUTSTANDING, 8);
	/* SP_VS_PARAM_REG */
	*cmds++ = _SET(SP_VSPARAMREG_POSREGID, 4) |
		_SET(SP_VSPARAMREG_PSIZEREGID, 252) |
		_SET(SP_VSPARAMREG_TOTALVSOUTVAR, 1);
	/* SP_VS_OUT_REG0 */
	*cmds++ = _SET(SP_VSOUTREG_COMPMASK0, 3);
	/* SP_VS_OUT_REG1 */
	*cmds++ = 0x00000000;
	/* SP_VS_OUT_REG2 */
	*cmds++ = 0x00000000;
	/* SP_VS_OUT_REG3 */
	*cmds++ = 0x00000000;
	/* SP_VS_OUT_REG4 */
	*cmds++ = 0x00000000;
	/* SP_VS_OUT_REG5 */
	*cmds++ = 0x00000000;
	/* SP_VS_OUT_REG6 */
	*cmds++ = 0x00000000;
	/* SP_VS_OUT_REG7 */
	*cmds++ = 0x00000000;

	*cmds++ = cp_type3_packet(CP_SET_CONSTANT, 7);
	*cmds++ = CP_REG(A3XX_SP_VS_VPC_DST_REG_0);
	/* SP_VS_VPC_DST_REG0 */
	*cmds++ = _SET(SP_VSVPCDSTREG_OUTLOC0, 8);
	/* SP_VS_VPC_DST_REG1 */
	*cmds++ = 0x00000000;
	/* SP_VS_VPC_DST_REG2 */
	*cmds++ = 0x00000000;
	/* SP_VS_VPC_DST_REG3 */
	*cmds++ = 0x00000000;
	/* SP_VS_OBJ_OFFSET_REG */
	*cmds++ = 0x00000000;
	/* SP_VS_OBJ_START_REG */
	*cmds++ = 0x00000000;

	*cmds++ = cp_type3_packet(CP_SET_CONSTANT, 6);
	*cmds++ = CP_REG(A3XX_SP_VS_LENGTH_REG);
	/* SP_VS_LENGTH_REG */
	*cmds++ = _SET(SP_SHADERLENGTH_LEN, 1);
	/* SP_FS_CTRL_REG0 */
	*cmds++ = _SET(SP_FSCTRLREG0_FSTHREADMODE, SP_MULTI) |
		_SET(SP_FSCTRLREG0_FSINSTRBUFFERMODE, SP_BUFFER_MODE) |
		_SET(SP_FSCTRLREG0_FSICACHEINVALID, 1) |
		_SET(SP_FSCTRLREG0_FSHALFREGFOOTPRINT, 1) |
		_SET(SP_FSCTRLREG0_FSFULLREGFOOTPRINT, 1) |
		_SET(SP_FSCTRLREG0_FSINOUTREGOVERLAP, 1) |
		_SET(SP_FSCTRLREG0_FSTHREADSIZE, SP_FOUR_PIX_QUADS) |
		_SET(SP_FSCTRLREG0_FSSUPERTHREADMODE, 1) |
		_SET(SP_FSCTRLREG0_PIXLODENABLE, 1) |
		_SET(SP_FSCTRLREG0_FSLENGTH, 2);
	/* SP_FS_CTRL_REG1 */
	*cmds++ = _SET(SP_FSCTRLREG1_FSCONSTLENGTH, 1) |
		_SET(SP_FSCTRLREG1_FSINITIALOUTSTANDING, 2) |
		_SET(SP_FSCTRLREG1_HALFPRECVAROFFSET, 63);
	/* SP_FS_OBJ_OFFSET_REG */
	*cmds++ = _SET(SP_OBJOFFSETREG_CONSTOBJECTSTARTOFFSET, 128) |
		_SET(SP_OBJOFFSETREG_SHADEROBJOFFSETINIC, 126);
	/* SP_FS_OBJ_START_REG */
	*cmds++ = 0x00000000;

	*cmds++ = cp_type3_packet(CP_SET_CONSTANT, 3);
	*cmds++ = CP_REG(A3XX_SP_FS_FLAT_SHAD_MODE_REG_0);
	/* SP_FS_FLAT_SHAD_MODE_REG0 */
	*cmds++ = 0x00000000;
	/* SP_FS_FLAT_SHAD_MODE_REG1 */
	*cmds++ = 0x00000000;

	*cmds++ = cp_type3_packet(CP_SET_CONSTANT, 2);
	*cmds++ = CP_REG(A3XX_SP_FS_OUTPUT_REG);
	/* SP_FS_OUT_REG */
	*cmds++ = _SET(SP_FSOUTREG_PAD0, SP_PIXEL_BASED);

	*cmds++ = cp_type3_packet(CP_SET_CONSTANT, 5);
	*cmds++ = CP_REG(A3XX_SP_FS_MRT_REG_0);
	/* SP_FS_MRT_REG0 */
	*cmds++ = _SET(SP_FSMRTREG_PRECISION, 1);
	/* SP_FS_MRT_REG1 */
	*cmds++ = 0;
	/* SP_FS_MRT_REG2 */
	*cmds++ = 0;
	/* SP_FS_MRT_REG3 */
	*cmds++ = 0;

	*cmds++ = cp_type3_packet(CP_SET_CONSTANT, 11);
	*cmds++ = CP_REG(A3XX_VPC_ATTR);
	/* VPC_ATTR */
	*cmds++ = _SET(VPC_VPCATTR_TOTALATTR, 2) |
		_SET(VPC_VPCATTR_THRHDASSIGN, 1) |
		_SET(VPC_VPCATTR_LMSIZE, 1);
	/* VPC_PACK */
	*cmds++ = _SET(VPC_VPCPACK_NUMFPNONPOSVAR, 2) |
		_SET(VPC_VPCPACK_NUMNONPOSVSVAR, 2);
	/* VPC_VARYING_INTERP_MODE_0 */
	*cmds++ = 0x00000000;
	/* VPC_VARYING_INTERP_MODE1 */
	*cmds++ = 0x00000000;
	/* VPC_VARYING_INTERP_MODE2 */
	*cmds++ = 0x00000000;
	/* VPC_VARYING_IINTERP_MODE3 */
	*cmds++ = 0x00000000;
	/* VPC_VARRYING_PS_REPL_MODE_0 */
	*cmds++ = _SET(VPC_VPCVARPSREPLMODE_COMPONENT08, 1) |
		_SET(VPC_VPCVARPSREPLMODE_COMPONENT09, 2) |
		_SET(VPC_VPCVARPSREPLMODE_COMPONENT0A,	1) |
		_SET(VPC_VPCVARPSREPLMODE_COMPONENT0B, 2) |
		_SET(VPC_VPCVARPSREPLMODE_COMPONENT0C, 1) |
		_SET(VPC_VPCVARPSREPLMODE_COMPONENT0D, 2) |
		_SET(VPC_VPCVARPSREPLMODE_COMPONENT0E, 1) |
		_SET(VPC_VPCVARPSREPLMODE_COMPONENT0F, 2) |
		_SET(VPC_VPCVARPSREPLMODE_COMPONENT10, 1) |
		_SET(VPC_VPCVARPSREPLMODE_COMPONENT11, 2) |
		_SET(VPC_VPCVARPSREPLMODE_COMPONENT12, 1) |
		_SET(VPC_VPCVARPSREPLMODE_COMPONENT13, 2) |
		_SET(VPC_VPCVARPSREPLMODE_COMPONENT14, 1) |
		_SET(VPC_VPCVARPSREPLMODE_COMPONENT15, 2) |
		_SET(VPC_VPCVARPSREPLMODE_COMPONENT16, 1) |
		_SET(VPC_VPCVARPSREPLMODE_COMPONENT17, 2);
	/* VPC_VARRYING_PS_REPL_MODE_1 */
	*cmds++ = _SET(VPC_VPCVARPSREPLMODE_COMPONENT08, 1) |
		_SET(VPC_VPCVARPSREPLMODE_COMPONENT09, 2) |
		_SET(VPC_VPCVARPSREPLMODE_COMPONENT0A,	1) |
		_SET(VPC_VPCVARPSREPLMODE_COMPONENT0B, 2) |
		_SET(VPC_VPCVARPSREPLMODE_COMPONENT0C, 1) |
		_SET(VPC_VPCVARPSREPLMODE_COMPONENT0D, 2) |
		_SET(VPC_VPCVARPSREPLMODE_COMPONENT0E, 1) |
		_SET(VPC_VPCVARPSREPLMODE_COMPONENT0F, 2) |
		_SET(VPC_VPCVARPSREPLMODE_COMPONENT10, 1) |
		_SET(VPC_VPCVARPSREPLMODE_COMPONENT11, 2) |
		_SET(VPC_VPCVARPSREPLMODE_COMPONENT12, 1) |
		_SET(VPC_VPCVARPSREPLMODE_COMPONENT13, 2) |
		_SET(VPC_VPCVARPSREPLMODE_COMPONENT14, 1) |
		_SET(VPC_VPCVARPSREPLMODE_COMPONENT15, 2) |
		_SET(VPC_VPCVARPSREPLMODE_COMPONENT16, 1) |
		_SET(VPC_VPCVARPSREPLMODE_COMPONENT17, 2);
	/* VPC_VARRYING_PS_REPL_MODE_2 */
	*cmds++ = _SET(VPC_VPCVARPSREPLMODE_COMPONENT08, 1) |
		_SET(VPC_VPCVARPSREPLMODE_COMPONENT09, 2) |
		_SET(VPC_VPCVARPSREPLMODE_COMPONENT0A,	1) |
		_SET(VPC_VPCVARPSREPLMODE_COMPONENT0B, 2) |
		_SET(VPC_VPCVARPSREPLMODE_COMPONENT0C, 1) |
		_SET(VPC_VPCVARPSREPLMODE_COMPONENT0D, 2) |
		_SET(VPC_VPCVARPSREPLMODE_COMPONENT0E, 1) |
		_SET(VPC_VPCVARPSREPLMODE_COMPONENT0F, 2) |
		_SET(VPC_VPCVARPSREPLMODE_COMPONENT10, 1) |
		_SET(VPC_VPCVARPSREPLMODE_COMPONENT11, 2) |
		_SET(VPC_VPCVARPSREPLMODE_COMPONENT12, 1) |
		_SET(VPC_VPCVARPSREPLMODE_COMPONENT13, 2) |
		_SET(VPC_VPCVARPSREPLMODE_COMPONENT14, 1) |
		_SET(VPC_VPCVARPSREPLMODE_COMPONENT15, 2) |
		_SET(VPC_VPCVARPSREPLMODE_COMPONENT16, 1) |
		_SET(VPC_VPCVARPSREPLMODE_COMPONENT17, 2);
	/* VPC_VARRYING_PS_REPL_MODE_3 */
	*cmds++ = _SET(VPC_VPCVARPSREPLMODE_COMPONENT08, 1) |
		_SET(VPC_VPCVARPSREPLMODE_COMPONENT09, 2) |
		_SET(VPC_VPCVARPSREPLMODE_COMPONENT0A,	1) |
		_SET(VPC_VPCVARPSREPLMODE_COMPONENT0B, 2) |
		_SET(VPC_VPCVARPSREPLMODE_COMPONENT0C, 1) |
		_SET(VPC_VPCVARPSREPLMODE_COMPONENT0D, 2) |
		_SET(VPC_VPCVARPSREPLMODE_COMPONENT0E, 1) |
		_SET(VPC_VPCVARPSREPLMODE_COMPONENT0F, 2) |
		_SET(VPC_VPCVARPSREPLMODE_COMPONENT10, 1) |
		_SET(VPC_VPCVARPSREPLMODE_COMPONENT11, 2) |
		_SET(VPC_VPCVARPSREPLMODE_COMPONENT12, 1) |
		_SET(VPC_VPCVARPSREPLMODE_COMPONENT13, 2) |
		_SET(VPC_VPCVARPSREPLMODE_COMPONENT14, 1) |
		_SET(VPC_VPCVARPSREPLMODE_COMPONENT15, 2) |
		_SET(VPC_VPCVARPSREPLMODE_COMPONENT16, 1) |
		_SET(VPC_VPCVARPSREPLMODE_COMPONENT17, 2);

	*cmds++ = cp_type3_packet(CP_SET_CONSTANT, 2);
	*cmds++ = CP_REG(A3XX_SP_SP_CTRL_REG);
	/* SP_SP_CTRL_REG */
	*cmds++ = _SET(SP_SPCTRLREG_SLEEPMODE, 1) |
		_SET(SP_SPCTRLREG_LOMODE, 1);

	/* Load vertex shader */
	*cmds++ = cp_type3_packet(CP_LOAD_STATE, 10);
	*cmds++ = (0 << CP_LOADSTATE_DSTOFFSET_SHIFT)
		| (HLSQ_DIRECT << CP_LOADSTATE_STATESRC_SHIFT)
		| (HLSQ_BLOCK_ID_SP_VS << CP_LOADSTATE_STATEBLOCKID_SHIFT)
		| (1 << CP_LOADSTATE_NUMOFUNITS_SHIFT);
	*cmds++ = (HLSQ_SP_VS_INSTR << CP_LOADSTATE_STATETYPE_SHIFT)
		| (0 << CP_LOADSTATE_EXTSRCADDR_SHIFT);
	/* (sy)end; */
	*cmds++ = 0x00000000; *cmds++ = 0x13001000;
	/* nop; */
	*cmds++ = 0x00000000; *cmds++ = 0x00000000;
	/* nop; */
	*cmds++ = 0x00000000; *cmds++ = 0x00000000;
	/* nop; */
	*cmds++ = 0x00000000; *cmds++ = 0x00000000;

	*cmds++ = cp_type0_packet(A3XX_VFD_PERFCOUNTER0_SELECT, 1);
	*cmds++ = 0x00000000;

	*cmds++ = cp_type3_packet(CP_WAIT_FOR_IDLE, 1);
	*cmds++ = 0x00000000;


	/* Load fragment shader */
	*cmds++ = cp_type3_packet(CP_LOAD_STATE, 18);
	*cmds++ = (0 << CP_LOADSTATE_DSTOFFSET_SHIFT)
		| (HLSQ_DIRECT << CP_LOADSTATE_STATESRC_SHIFT)
		| (HLSQ_BLOCK_ID_SP_FS << CP_LOADSTATE_STATEBLOCKID_SHIFT)
		| (2 << CP_LOADSTATE_NUMOFUNITS_SHIFT);
	*cmds++ = (HLSQ_SP_FS_INSTR << CP_LOADSTATE_STATETYPE_SHIFT)
		| (0 << CP_LOADSTATE_EXTSRCADDR_SHIFT);
	/* (sy)(rpt1)bary.f (ei)r0.z, (r)0, r0.x; */
	*cmds++ = 0x00002000; *cmds++ = 0x57309902;
	/* (rpt5)nop; */
	*cmds++ = 0x00000000; *cmds++ = 0x00000500;
	/* sam (f32)r0.xyzw, r0.z, s#0, t#0; */
	*cmds++ = 0x00000005; *cmds++ = 0xa0c01f00;
	/* (sy)mov.f32f32 r1.x, r0.x; */
	*cmds++ = 0x00000000; *cmds++ = 0x30040b00;
	/* mov.f32f32 r1.y, r0.y; */
	*cmds++ = 0x00000000; *cmds++ = 0x03000000;
	/* mov.f32f32 r1.z, r0.z; */
	*cmds++ = 0x00000000; *cmds++ = 0x00000000;
	/* mov.f32f32 r1.w, r0.w; */
	*cmds++ = 0x00000000; *cmds++ = 0x00000000;
	/* end; */
	*cmds++ = 0x00000000; *cmds++ = 0x00000000;

	*cmds++ = cp_type0_packet(A3XX_VFD_PERFCOUNTER0_SELECT, 1);
	*cmds++ = 0x00000000;

	*cmds++ = cp_type3_packet(CP_WAIT_FOR_IDLE, 1);
	*cmds++ = 0x00000000;

	*cmds++ = cp_type3_packet(CP_SET_CONSTANT, 3);
	*cmds++ = CP_REG(A3XX_VFD_CONTROL_0);
	/* VFD_CONTROL_0 */
	*cmds++ = _SET(VFD_CTRLREG0_TOTALATTRTOVS, 8) |
		_SET(VFD_CTRLREG0_PACKETSIZE, 2) |
		_SET(VFD_CTRLREG0_STRMDECINSTRCNT, 2) |
		_SET(VFD_CTRLREG0_STRMFETCHINSTRCNT, 2);
	/* VFD_CONTROL_1 */
	*cmds++ =  _SET(VFD_CTRLREG1_MAXSTORAGE, 2) |
		_SET(VFD_CTRLREG1_REGID4VTX, 252) |
		_SET(VFD_CTRLREG1_REGID4INST, 252);

	*cmds++ = cp_type3_packet(CP_SET_CONSTANT, 5);
	*cmds++ = CP_REG(A3XX_VFD_FETCH_INSTR_0_0);
	/* VFD_FETCH_INSTR_0_0 */
	*cmds++ = _SET(VFD_FETCHINSTRUCTIONS_FETCHSIZE, 7) |
		_SET(VFD_FETCHINSTRUCTIONS_BUFSTRIDE, 8) |
		_SET(VFD_FETCHINSTRUCTIONS_SWITCHNEXT, 1) |
		_SET(VFD_FETCHINSTRUCTIONS_STEPRATE, 1);
	/* VFD_FETCH_INSTR_1_0 */
	*cmds++ = _SET(VFD_BASEADDR_BASEADDR,
		shadow->quad_vertices_restore.gpuaddr);
	/* VFD_FETCH_INSTR_0_1 */
	*cmds++ = _SET(VFD_FETCHINSTRUCTIONS_FETCHSIZE, 11) |
		_SET(VFD_FETCHINSTRUCTIONS_BUFSTRIDE, 12) |
		_SET(VFD_FETCHINSTRUCTIONS_INDEXDECODE, 1) |
		_SET(VFD_FETCHINSTRUCTIONS_STEPRATE, 1);
	/* VFD_FETCH_INSTR_1_1 */
	*cmds++ = _SET(VFD_BASEADDR_BASEADDR,
		shadow->quad_vertices_restore.gpuaddr + 16);

	*cmds++ = cp_type3_packet(CP_SET_CONSTANT, 3);
	*cmds++ = CP_REG(A3XX_VFD_DECODE_INSTR_0);
	/* VFD_DECODE_INSTR_0 */
	*cmds++ = _SET(VFD_DECODEINSTRUCTIONS_WRITEMASK, 0x0F) |
		_SET(VFD_DECODEINSTRUCTIONS_CONSTFILL, 1) |
		_SET(VFD_DECODEINSTRUCTIONS_FORMAT, 1) |
		_SET(VFD_DECODEINSTRUCTIONS_SHIFTCNT, 8) |
		_SET(VFD_DECODEINSTRUCTIONS_LASTCOMPVALID, 1) |
		_SET(VFD_DECODEINSTRUCTIONS_SWITCHNEXT, 1);
	/* VFD_DECODE_INSTR_1 */
	*cmds++ = _SET(VFD_DECODEINSTRUCTIONS_WRITEMASK, 0x0F) |
		_SET(VFD_DECODEINSTRUCTIONS_CONSTFILL, 1) |
		_SET(VFD_DECODEINSTRUCTIONS_FORMAT, 2) |
		_SET(VFD_DECODEINSTRUCTIONS_REGID, 4) |
		_SET(VFD_DECODEINSTRUCTIONS_SHIFTCNT, 12) |
		_SET(VFD_DECODEINSTRUCTIONS_LASTCOMPVALID, 1);

	*cmds++ = cp_type3_packet(CP_SET_CONSTANT, 2);
	*cmds++ = CP_REG(A3XX_RB_DEPTH_CONTROL);
	/* RB_DEPTH_CONTROL */
	*cmds++ = _SET(RB_DEPTHCONTROL_Z_TEST_FUNC, RB_FRAG_LESS);

	*cmds++ = cp_type3_packet(CP_SET_CONSTANT, 2);
	*cmds++ = CP_REG(A3XX_RB_STENCIL_CONTROL);
	/* RB_STENCIL_CONTROL */
	*cmds++ = _SET(RB_STENCILCONTROL_STENCIL_FUNC, RB_REF_ALWAYS) |
		_SET(RB_STENCILCONTROL_STENCIL_FAIL, RB_STENCIL_KEEP) |
		_SET(RB_STENCILCONTROL_STENCIL_ZPASS, RB_STENCIL_KEEP) |
		_SET(RB_STENCILCONTROL_STENCIL_ZFAIL, RB_STENCIL_KEEP) |
		_SET(RB_STENCILCONTROL_STENCIL_FUNC_BF, RB_REF_ALWAYS) |
		_SET(RB_STENCILCONTROL_STENCIL_FAIL_BF, RB_STENCIL_KEEP) |
		_SET(RB_STENCILCONTROL_STENCIL_ZPASS_BF, RB_STENCIL_KEEP) |
		_SET(RB_STENCILCONTROL_STENCIL_ZFAIL_BF, RB_STENCIL_KEEP);

	*cmds++ = cp_type3_packet(CP_SET_CONSTANT, 2);
	*cmds++ = CP_REG(A3XX_RB_MODE_CONTROL);
	/* RB_MODE_CONTROL */
	*cmds++ = _SET(RB_MODECONTROL_RENDER_MODE, RB_RENDERING_PASS) |
		_SET(RB_MODECONTROL_MARB_CACHE_SPLIT_MODE, 1);

	*cmds++ = cp_type3_packet(CP_SET_CONSTANT, 2);
	*cmds++ = CP_REG(A3XX_RB_RENDER_CONTROL);
	/* RB_RENDER_CONTROL */
	*cmds++ = _SET(RB_RENDERCONTROL_BIN_WIDTH, shadow->width >> 5) |
		_SET(RB_RENDERCONTROL_ALPHA_TEST_FUNC, 7);

	*cmds++ = cp_type3_packet(CP_SET_CONSTANT, 2);
	*cmds++ = CP_REG(A3XX_RB_MSAA_CONTROL);
	/* RB_MSAA_CONTROL */
	*cmds++ = _SET(RB_MSAACONTROL_MSAA_DISABLE, 1) |
		_SET(RB_MSAACONTROL_SAMPLE_MASK, 0xFFFF);

	*cmds++ = cp_type3_packet(CP_SET_CONSTANT, 2);
	*cmds++ = CP_REG(A3XX_RB_MRT_CONTROL0);
	/* RB_MRT_CONTROL0 */
	*cmds++ = _SET(RB_MRTCONTROL_ROP_CODE, 12) |
		_SET(RB_MRTCONTROL_DITHER_MODE, RB_DITHER_DISABLE) |
		_SET(RB_MRTCONTROL_COMPONENT_ENABLE, 0xF);

	*cmds++ = cp_type3_packet(CP_SET_CONSTANT, 3);
	*cmds++ = CP_REG(A3XX_RB_MRT_BLEND_CONTROL0);
	/* RB_MRT_BLENDCONTROL0 */
	*cmds++ = _SET(RB_MRTBLENDCONTROL_RGB_SRC_FACTOR, RB_FACTOR_ONE) |
		_SET(RB_MRTBLENDCONTROL_RGB_BLEND_OPCODE, RB_BLEND_OP_ADD) |
		_SET(RB_MRTBLENDCONTROL_RGB_DEST_FACTOR, RB_FACTOR_ZERO) |
		_SET(RB_MRTBLENDCONTROL_ALPHA_SRC_FACTOR, RB_FACTOR_ONE) |
		_SET(RB_MRTBLENDCONTROL_ALPHA_BLEND_OPCODE, RB_BLEND_OP_ADD) |
		_SET(RB_MRTBLENDCONTROL_ALPHA_DEST_FACTOR, RB_FACTOR_ZERO) |
		_SET(RB_MRTBLENDCONTROL_CLAMP_ENABLE, 1);
	/* RB_MRT_CONTROL1 */
	*cmds++ = _SET(RB_MRTCONTROL_READ_DEST_ENABLE, 1) |
		_SET(RB_MRTCONTROL_ROP_CODE, 12) |
		_SET(RB_MRTCONTROL_DITHER_MODE, RB_DITHER_ALWAYS) |
		_SET(RB_MRTCONTROL_COMPONENT_ENABLE, 0xF);

	*cmds++ = cp_type3_packet(CP_SET_CONSTANT, 3);
	*cmds++ = CP_REG(A3XX_RB_MRT_BLEND_CONTROL1);
	/* RB_MRT_BLENDCONTROL1 */
	*cmds++ = _SET(RB_MRTBLENDCONTROL_RGB_SRC_FACTOR, RB_FACTOR_ONE) |
		_SET(RB_MRTBLENDCONTROL_RGB_BLEND_OPCODE, RB_BLEND_OP_ADD) |
		_SET(RB_MRTBLENDCONTROL_RGB_DEST_FACTOR, RB_FACTOR_ZERO) |
		_SET(RB_MRTBLENDCONTROL_ALPHA_SRC_FACTOR, RB_FACTOR_ONE) |
		_SET(RB_MRTBLENDCONTROL_ALPHA_BLEND_OPCODE, RB_BLEND_OP_ADD) |
		_SET(RB_MRTBLENDCONTROL_ALPHA_DEST_FACTOR, RB_FACTOR_ZERO) |
		_SET(RB_MRTBLENDCONTROL_CLAMP_ENABLE, 1);
	/* RB_MRT_CONTROL2 */
	*cmds++ = _SET(RB_MRTCONTROL_READ_DEST_ENABLE, 1) |
		_SET(RB_MRTCONTROL_ROP_CODE, 12) |
		_SET(RB_MRTCONTROL_DITHER_MODE, RB_DITHER_ALWAYS) |
		_SET(RB_MRTCONTROL_COMPONENT_ENABLE, 0xF);

	*cmds++ = cp_type3_packet(CP_SET_CONSTANT, 3);
	*cmds++ = CP_REG(A3XX_RB_MRT_BLEND_CONTROL2);
	/* RB_MRT_BLENDCONTROL2 */
	*cmds++ = _SET(RB_MRTBLENDCONTROL_RGB_SRC_FACTOR, RB_FACTOR_ONE) |
		_SET(RB_MRTBLENDCONTROL_RGB_BLEND_OPCODE, RB_BLEND_OP_ADD) |
		_SET(RB_MRTBLENDCONTROL_RGB_DEST_FACTOR, RB_FACTOR_ZERO) |
		_SET(RB_MRTBLENDCONTROL_ALPHA_SRC_FACTOR, RB_FACTOR_ONE) |
		_SET(RB_MRTBLENDCONTROL_ALPHA_BLEND_OPCODE, RB_BLEND_OP_ADD) |
		_SET(RB_MRTBLENDCONTROL_ALPHA_DEST_FACTOR, RB_FACTOR_ZERO) |
		_SET(RB_MRTBLENDCONTROL_CLAMP_ENABLE, 1);
	/* RB_MRT_CONTROL3 */
	*cmds++ = _SET(RB_MRTCONTROL_READ_DEST_ENABLE, 1) |
		_SET(RB_MRTCONTROL_ROP_CODE, 12) |
		_SET(RB_MRTCONTROL_DITHER_MODE, RB_DITHER_ALWAYS) |
		_SET(RB_MRTCONTROL_COMPONENT_ENABLE, 0xF);

	*cmds++ = cp_type3_packet(CP_SET_CONSTANT, 2);
	*cmds++ = CP_REG(A3XX_RB_MRT_BLEND_CONTROL3);
	/* RB_MRT_BLENDCONTROL3 */
	*cmds++ = _SET(RB_MRTBLENDCONTROL_RGB_SRC_FACTOR, RB_FACTOR_ONE) |
		_SET(RB_MRTBLENDCONTROL_RGB_BLEND_OPCODE, RB_BLEND_OP_ADD) |
		_SET(RB_MRTBLENDCONTROL_RGB_DEST_FACTOR, RB_FACTOR_ZERO) |
		_SET(RB_MRTBLENDCONTROL_ALPHA_SRC_FACTOR, RB_FACTOR_ONE) |
		_SET(RB_MRTBLENDCONTROL_ALPHA_BLEND_OPCODE, RB_BLEND_OP_ADD) |
		_SET(RB_MRTBLENDCONTROL_ALPHA_DEST_FACTOR, RB_FACTOR_ZERO) |
		_SET(RB_MRTBLENDCONTROL_CLAMP_ENABLE, 1);

	*cmds++ = cp_type3_packet(CP_SET_CONSTANT, 5);
	*cmds++ = CP_REG(A3XX_VFD_INDEX_MIN);
	/* VFD_INDEX_MIN */
	*cmds++ = 0x00000000;
	/* VFD_INDEX_MAX */
	*cmds++ = 340;
	/* VFD_INDEX_OFFSET */
	*cmds++ = 0x00000000;
	/* TPL1_TP_VS_TEX_OFFSET */
	*cmds++ = 0x00000000;

	*cmds++ = cp_type3_packet(CP_SET_CONSTANT, 2);
	*cmds++ = CP_REG(A3XX_VFD_VS_THREADING_THRESHOLD);
	/* VFD_VS_THREADING_THRESHOLD */
	*cmds++ = _SET(VFD_THREADINGTHRESHOLD_REGID_THRESHOLD, 15) |
		_SET(VFD_THREADINGTHRESHOLD_REGID_VTXCNT, 252);

	*cmds++ = cp_type3_packet(CP_SET_CONSTANT, 2);
	*cmds++ = CP_REG(A3XX_TPL1_TP_VS_TEX_OFFSET);
	/* TPL1_TP_VS_TEX_OFFSET */
	*cmds++ = 0x00000000;

	*cmds++ = cp_type3_packet(CP_SET_CONSTANT, 2);
	*cmds++ = CP_REG(A3XX_TPL1_TP_FS_TEX_OFFSET);
	/* TPL1_TP_FS_TEX_OFFSET */
	*cmds++ = _SET(TPL1_TPTEXOFFSETREG_SAMPLEROFFSET, 16) |
		_SET(TPL1_TPTEXOFFSETREG_MEMOBJOFFSET, 16) |
		_SET(TPL1_TPTEXOFFSETREG_BASETABLEPTR, 224);

	*cmds++ = cp_type3_packet(CP_SET_CONSTANT, 2);
	*cmds++ = CP_REG(A3XX_GRAS_SC_CONTROL);
	/* GRAS_SC_CONTROL */
	/*cmds++ = _SET(GRAS_SC_CONTROL_RASTER_MODE, 1);
		*cmds++ = _SET(GRAS_SC_CONTROL_RASTER_MODE, 1) |*/
	*cmds++ = 0x04001000;

	*cmds++ = cp_type3_packet(CP_SET_CONSTANT, 2);
	*cmds++ = CP_REG(A3XX_GRAS_SU_MODE_CONTROL);
	/* GRAS_SU_MODE_CONTROL */
	*cmds++ = _SET(GRAS_SU_CTRLMODE_LINEHALFWIDTH, 2);

	*cmds++ = cp_type3_packet(CP_SET_CONSTANT, 3);
	*cmds++ = CP_REG(A3XX_GRAS_SC_WINDOW_SCISSOR_TL);
	/* GRAS_SC_WINDOW_SCISSOR_TL */
	*cmds++ = 0x00000000;
	/* GRAS_SC_WINDOW_SCISSOR_BR */
	*cmds++ = _SET(GRAS_SC_WINDOW_SCISSOR_BR_BR_X, shadow->width - 1) |
		_SET(GRAS_SC_WINDOW_SCISSOR_BR_BR_Y, shadow->height - 1);

	*cmds++ = cp_type3_packet(CP_SET_CONSTANT, 3);
	*cmds++ = CP_REG(A3XX_GRAS_SC_SCREEN_SCISSOR_TL);
	/* GRAS_SC_SCREEN_SCISSOR_TL */
	*cmds++ = 0x00000000;
	/* GRAS_SC_SCREEN_SCISSOR_BR */
	*cmds++ = _SET(GRAS_SC_SCREEN_SCISSOR_BR_BR_X, shadow->width - 1) |
		_SET(GRAS_SC_SCREEN_SCISSOR_BR_BR_Y, shadow->height - 1);

	*cmds++ = cp_type3_packet(CP_SET_CONSTANT, 5);
	*cmds++ = CP_REG(A3XX_GRAS_CL_VPORT_XOFFSET);
	/* GRAS_CL_VPORT_XOFFSET */
	*cmds++ = 0x00000000;
	/* GRAS_CL_VPORT_XSCALE */
	*cmds++ = _SET(GRAS_CL_VPORT_XSCALE_VPORT_XSCALE, 0x3F800000);
	/* GRAS_CL_VPORT_YOFFSET */
	*cmds++ = 0x00000000;
	/* GRAS_CL_VPORT_YSCALE */
	*cmds++ = _SET(GRAS_CL_VPORT_YSCALE_VPORT_YSCALE, 0x3F800000);

	*cmds++ = cp_type3_packet(CP_SET_CONSTANT, 3);
	*cmds++ = CP_REG(A3XX_GRAS_CL_VPORT_ZOFFSET);
	/* GRAS_CL_VPORT_ZOFFSET */
	*cmds++ = 0x00000000;
	/* GRAS_CL_VPORT_ZSCALE */
	*cmds++ = _SET(GRAS_CL_VPORT_ZSCALE_VPORT_ZSCALE, 0x3F800000);

	*cmds++ = cp_type3_packet(CP_SET_CONSTANT, 2);
	*cmds++ = CP_REG(A3XX_GRAS_CL_CLIP_CNTL);
	/* GRAS_CL_CLIP_CNTL */
	*cmds++ = _SET(GRAS_CL_CLIP_CNTL_IJ_PERSP_CENTER, 1);

	*cmds++ = cp_type3_packet(CP_SET_CONSTANT, 2);
	*cmds++ = CP_REG(A3XX_SP_FS_IMAGE_OUTPUT_REG_0);
	/* SP_FS_IMAGE_OUTPUT_REG_0 */
	*cmds++ = _SET(SP_IMAGEOUTPUTREG_MRTFORMAT, SP_R8G8B8A8_UNORM);

	*cmds++ = cp_type3_packet(CP_SET_CONSTANT, 2);
	*cmds++ = CP_REG(A3XX_PC_PRIM_VTX_CNTL);
	/* PC_PRIM_VTX_CONTROL */
	*cmds++ = _SET(PC_PRIM_VTX_CONTROL_STRIDE_IN_VPC, 2) |
		_SET(PC_PRIM_VTX_CONTROL_POLYMODE_FRONT_PTYPE,
		PC_DRAW_TRIANGLES) |
		_SET(PC_PRIM_VTX_CONTROL_POLYMODE_BACK_PTYPE,
		PC_DRAW_TRIANGLES) |
		_SET(PC_PRIM_VTX_CONTROL_PROVOKING_VTX_LAST, 1);


	/* oxili_generate_context_roll_packets */
	*cmds++ = cp_type0_packet(A3XX_SP_VS_CTRL_REG0, 1);
	*cmds++ = 0x00000400;

	*cmds++ = cp_type0_packet(A3XX_SP_FS_CTRL_REG0, 1);
	*cmds++ = 0x00000400;

	*cmds++ = cp_type0_packet(A3XX_SP_VS_PVT_MEM_SIZE_REG, 1);
	*cmds++ = 0x00008000; /* SP_VS_MEM_SIZE_REG */

	*cmds++ = cp_type0_packet(A3XX_SP_FS_PVT_MEM_SIZE_REG, 1);
	*cmds++ = 0x00008000; /* SP_FS_MEM_SIZE_REG */

	/* Clear cache invalidate bit when re-loading the shader control regs */
	*cmds++ = cp_type0_packet(A3XX_SP_VS_CTRL_REG0, 1);
	*cmds++ = _SET(SP_VSCTRLREG0_VSTHREADMODE, SP_MULTI) |
		_SET(SP_VSCTRLREG0_VSINSTRBUFFERMODE, SP_BUFFER_MODE) |
		_SET(SP_VSCTRLREG0_VSFULLREGFOOTPRINT, 2) |
		_SET(SP_VSCTRLREG0_VSTHREADSIZE, SP_TWO_VTX_QUADS) |
		_SET(SP_VSCTRLREG0_VSLENGTH, 1);

	*cmds++ = cp_type0_packet(A3XX_SP_FS_CTRL_REG0, 1);
	*cmds++ = _SET(SP_FSCTRLREG0_FSTHREADMODE, SP_MULTI) |
		_SET(SP_FSCTRLREG0_FSINSTRBUFFERMODE, SP_BUFFER_MODE) |
		_SET(SP_FSCTRLREG0_FSHALFREGFOOTPRINT, 1) |
		_SET(SP_FSCTRLREG0_FSFULLREGFOOTPRINT, 1) |
		_SET(SP_FSCTRLREG0_FSINOUTREGOVERLAP, 1) |
		_SET(SP_FSCTRLREG0_FSTHREADSIZE, SP_FOUR_PIX_QUADS) |
		_SET(SP_FSCTRLREG0_FSSUPERTHREADMODE, 1) |
		_SET(SP_FSCTRLREG0_FSLENGTH, 2);

	*cmds++ = cp_type0_packet(A3XX_SP_VS_PVT_MEM_SIZE_REG, 1);
	*cmds++ = 0x00000000;		 /* SP_VS_MEM_SIZE_REG */

	*cmds++ = cp_type0_packet(A3XX_SP_FS_PVT_MEM_SIZE_REG, 1);
	*cmds++ = 0x00000000;		 /* SP_FS_MEM_SIZE_REG */

	/* end oxili_generate_context_roll_packets */

	*cmds++ = cp_type3_packet(CP_DRAW_INDX, 3);
	*cmds++ = 0x00000000; /* Viz query info */
	*cmds++ = BUILD_PC_DRAW_INITIATOR(PC_DI_PT_RECTLIST,
					  PC_DI_SRC_SEL_AUTO_INDEX,
					  PC_DI_INDEX_SIZE_16_BIT,
					  PC_DI_IGNORE_VISIBILITY);
	*cmds++ = 0x00000002; /* Num indices */

	/* Create indirect buffer command for above command sequence */
	create_ib1(drawctxt, shadow->gmem_restore, start, cmds);

	return cmds;
}


static void build_regrestore_cmds(struct adreno_device *adreno_dev,
				  struct adreno_context *drawctxt)
{
	unsigned int *start = tmp_ctx.cmd;
	unsigned int *cmd = start;
	unsigned int *lcc_start;

	int i;

	/* Flush HLSQ lazy updates */
	*cmd++ = cp_type3_packet(CP_EVENT_WRITE, 1);
	*cmd++ = 0x7;		/* HLSQ_FLUSH */
	*cmd++ = cp_type3_packet(CP_WAIT_FOR_IDLE, 1);
	*cmd++ = 0;

	*cmd++ = cp_type0_packet(A3XX_UCHE_CACHE_INVALIDATE0_REG, 2);
	*cmd++ = 0x00000000;    /* No start addr for full invalidate */
	*cmd++ = (unsigned int)
		UCHE_ENTIRE_CACHE << UCHE_INVALIDATE1REG_ALLORPORTION |
		UCHE_OP_INVALIDATE << UCHE_INVALIDATE1REG_OPCODE |
		0;  /* No end addr for full invalidate */

	lcc_start = cmd;

	/* deferred cp_type3_packet(CP_LOAD_CONSTANT_CONTEXT, ???); */
	cmd++;

#ifdef CONFIG_MSM_KGSL_DISABLE_SHADOW_WRITES
	/* Force mismatch */
	*cmd++ = ((drawctxt->gpustate.gpuaddr + REG_OFFSET) & 0xFFFFE000) | 1;
#else
	*cmd++ = (drawctxt->gpustate.gpuaddr + REG_OFFSET) & 0xFFFFE000;
#endif

	for (i = 0; i < ARRAY_SIZE(context_register_ranges) / 2; i++) {
		cmd = reg_range(cmd, context_register_ranges[i * 2],
				context_register_ranges[i * 2 + 1]);
	}

	lcc_start[0] = cp_type3_packet(CP_LOAD_CONSTANT_CONTEXT,
					(cmd - lcc_start) - 1);

#ifdef CONFIG_MSM_KGSL_DISABLE_SHADOW_WRITES
	lcc_start[2] |= (0 << 24) | (4 << 16);	/* Disable shadowing. */
#else
	lcc_start[2] |= (1 << 24) | (4 << 16);
#endif

	for (i = 0; i < ARRAY_SIZE(global_registers); i++) {
		*cmd++ = cp_type0_packet(global_registers[i], 1);
		tmp_ctx.reg_values[i] = virt2gpu(cmd, &drawctxt->gpustate);
		*cmd++ = 0x00000000;
	}

	create_ib1(drawctxt, drawctxt->reg_restore, start, cmd);
	tmp_ctx.cmd = cmd;
}

static void build_constantrestore_cmds(struct adreno_device *adreno_dev,
				       struct adreno_context *drawctxt)
{
	unsigned int *cmd = tmp_ctx.cmd;
	unsigned int *start = cmd;
	unsigned int mode = 4;	/* Indirect mode */
	unsigned int stateblock;
	unsigned int numunits;
	unsigned int statetype;

	drawctxt->cond_execs[2].hostptr = cmd;
	drawctxt->cond_execs[2].gpuaddr = virt2gpu(cmd, &drawctxt->gpustate);
	*cmd++ = 0;
	drawctxt->cond_execs[3].hostptr = cmd;
	drawctxt->cond_execs[3].gpuaddr = virt2gpu(cmd, &drawctxt->gpustate);
	*cmd++ = 0;

#ifndef CONFIG_MSM_KGSL_DISABLE_SHADOW_WRITES
	*cmd++ = cp_type3_packet(CP_LOAD_CONSTANT_CONTEXT, 3);
	*cmd++ = (drawctxt->gpustate.gpuaddr + REG_OFFSET) & 0xFFFFE000;
	*cmd++ = 4 << 16;
	*cmd++ = 0x0;
#endif
	/* HLSQ full update */
	*cmd++ = cp_type3_packet(CP_SET_CONSTANT, 2);
	*cmd++ = CP_REG(A3XX_HLSQ_CONTROL_0_REG);
	*cmd++ = 0x68000240;	/* A3XX_HLSQ_CONTROL_0_REG */

#ifndef CONFIG_MSM_KGSL_DISABLE_SHADOW_WRITES
	/* Re-enable shadowing */
	*cmd++ = cp_type3_packet(CP_LOAD_CONSTANT_CONTEXT, 3);
	*cmd++ = (drawctxt->gpustate.gpuaddr + REG_OFFSET) & 0xFFFFE000;
	*cmd++ = (4 << 16) | (1 << 24);
	*cmd++ = 0x0;
#endif

	/* Load vertex shader constants */
	*cmd++ = cp_type3_packet(CP_COND_EXEC, 4);
	*cmd++ = drawctxt->cond_execs[2].gpuaddr >> 2;
	*cmd++ = drawctxt->cond_execs[2].gpuaddr >> 2;
	*cmd++ = 0x0000ffff;
	*cmd++ = 3; /* EXEC_COUNT */
	*cmd++ = cp_type3_packet(CP_LOAD_STATE, 2);
	drawctxt->constant_load_commands[0].hostptr = cmd;
	drawctxt->constant_load_commands[0].gpuaddr = virt2gpu(cmd,
		&drawctxt->gpustate);

	/*
	   From fixup:

	   mode = 4 (indirect)
	   stateblock = 4 (Vertex constants)
	   numunits = SP_VS_CTRL_REG1.VSCONSTLENGTH * 2; (256bit units)

	   From register spec:
	   SP_VS_CTRL_REG1.VSCONSTLENGTH [09:00]: 0-512, unit = 128bits.

	   ord1 = (numunits<<22) | (stateblock<<19) | (mode<<16);
	 */

	*cmd++ = 0;		/* ord1 */
	*cmd++ = ((drawctxt->gpustate.gpuaddr) & 0xfffffffc) | 1;

	/* Load fragment shader constants */
	*cmd++ = cp_type3_packet(CP_COND_EXEC, 4);
	*cmd++ = drawctxt->cond_execs[3].gpuaddr >> 2;
	*cmd++ = drawctxt->cond_execs[3].gpuaddr >> 2;
	*cmd++ = 0x0000ffff;
	*cmd++ = 3; /* EXEC_COUNT */
	*cmd++ = cp_type3_packet(CP_LOAD_STATE, 2);
	drawctxt->constant_load_commands[1].hostptr = cmd;
	drawctxt->constant_load_commands[1].gpuaddr =
	    virt2gpu(cmd, &drawctxt->gpustate);
	/*
	   From fixup:

	   mode = 4 (indirect)
	   stateblock = 6 (Fragment constants)
	   numunits = SP_FS_CTRL_REG1.FSCONSTLENGTH * 2; (256bit units)

	   From register spec:
	   SP_FS_CTRL_REG1.FSCONSTLENGTH [09:00]: 0-512, unit = 128bits.

	   ord1 = (numunits<<22) | (stateblock<<19) | (mode<<16);
	 */

	*cmd++ = 0;		/* ord1 */
	drawctxt->constant_load_commands[2].hostptr = cmd;
	drawctxt->constant_load_commands[2].gpuaddr =
	    virt2gpu(cmd, &drawctxt->gpustate);
	/*
	   From fixup:
	   base = drawctxt->gpustate.gpuaddr (ALU constant shadow base)
	   offset = SP_FS_OBJ_OFFSET_REG.CONSTOBJECTSTARTOFFSET

	   From register spec:
	   SP_FS_OBJ_OFFSET_REG.CONSTOBJECTSTARTOFFSET [16:24]: Constant object
	   start offset in on chip RAM,
	   128bit aligned

	   ord2 = base + offset | 1
	   Because of the base alignment we can use
	   ord2 = base | offset | 1
	 */
	*cmd++ = 0;		/* ord2 */

	/* Restore VS texture memory objects */
	stateblock = 0;
	statetype = 1;
	numunits = (TEX_SIZE_MEM_OBJECTS / 7) / 4;

	*cmd++ = cp_type3_packet(CP_LOAD_STATE, 2);
	*cmd++ = (numunits << 22) | (stateblock << 19) | (mode << 16);
	*cmd++ = ((drawctxt->gpustate.gpuaddr + VS_TEX_OFFSET_MEM_OBJECTS)
	    & 0xfffffffc) | statetype;

	/* Restore VS texture mipmap addresses */
	stateblock = 1;
	statetype = 1;
	numunits = TEX_SIZE_MIPMAP / 4;
	*cmd++ = cp_type3_packet(CP_LOAD_STATE, 2);
	*cmd++ = (numunits << 22) | (stateblock << 19) | (mode << 16);
	*cmd++ = ((drawctxt->gpustate.gpuaddr + VS_TEX_OFFSET_MIPMAP)
	    & 0xfffffffc) | statetype;

	/* Restore VS texture sampler objects */
	stateblock = 0;
	statetype = 0;
	numunits = (TEX_SIZE_SAMPLER_OBJ / 2) / 4;
	*cmd++ = cp_type3_packet(CP_LOAD_STATE, 2);
	*cmd++ = (numunits << 22) | (stateblock << 19) | (mode << 16);
	*cmd++ = ((drawctxt->gpustate.gpuaddr + VS_TEX_OFFSET_SAMPLER_OBJ)
	    & 0xfffffffc) | statetype;

	/* Restore FS texture memory objects */
	stateblock = 2;
	statetype = 1;
	numunits = (TEX_SIZE_MEM_OBJECTS / 7) / 4;
	*cmd++ = cp_type3_packet(CP_LOAD_STATE, 2);
	*cmd++ = (numunits << 22) | (stateblock << 19) | (mode << 16);
	*cmd++ = ((drawctxt->gpustate.gpuaddr + FS_TEX_OFFSET_MEM_OBJECTS)
	    & 0xfffffffc) | statetype;

	/* Restore FS texture mipmap addresses */
	stateblock = 3;
	statetype = 1;
	numunits = TEX_SIZE_MIPMAP / 4;
	*cmd++ = cp_type3_packet(CP_LOAD_STATE, 2);
	*cmd++ = (numunits << 22) | (stateblock << 19) | (mode << 16);
	*cmd++ = ((drawctxt->gpustate.gpuaddr + FS_TEX_OFFSET_MIPMAP)
	    & 0xfffffffc) | statetype;

	/* Restore FS texture sampler objects */
	stateblock = 2;
	statetype = 0;
	numunits = (TEX_SIZE_SAMPLER_OBJ / 2) / 4;
	*cmd++ = cp_type3_packet(CP_LOAD_STATE, 2);
	*cmd++ = (numunits << 22) | (stateblock << 19) | (mode << 16);
	*cmd++ = ((drawctxt->gpustate.gpuaddr + FS_TEX_OFFSET_SAMPLER_OBJ)
	    & 0xfffffffc) | statetype;

	create_ib1(drawctxt, drawctxt->constant_restore, start, cmd);
	tmp_ctx.cmd = cmd;
}

static void build_shader_restore_cmds(struct adreno_device *adreno_dev,
				      struct adreno_context *drawctxt)
{
	unsigned int *cmd = tmp_ctx.cmd;
	unsigned int *start = cmd;

	/* Vertex shader */
	*cmd++ = cp_type3_packet(CP_COND_EXEC, 4);
	*cmd++ = drawctxt->cond_execs[0].gpuaddr >> 2;
	*cmd++ = drawctxt->cond_execs[0].gpuaddr >> 2;
	*cmd++ = 1;
	*cmd++ = 3;		/* EXEC_COUNT */

	*cmd++ = cp_type3_packet(CP_LOAD_STATE, 2);
	drawctxt->shader_load_commands[0].hostptr = cmd;
	drawctxt->shader_load_commands[0].gpuaddr =
	    virt2gpu(cmd, &drawctxt->gpustate);
	/*
	   From fixup:

	   mode = 4 (indirect)
	   stateblock = 4 (Vertex shader)
	   numunits = SP_VS_CTRL_REG0.VS_LENGTH

	   From regspec:
	   SP_VS_CTRL_REG0.VS_LENGTH [31:24]: VS length, unit = 256bits.
	   If bit31 is 1, it means overflow
	   or any long shader.

	   ord1 = (numunits<<22) | (stateblock<<19) | (mode<<11)
	 */
	*cmd++ = 0;		/*ord1 */
	*cmd++ = (drawctxt->gpustate.gpuaddr + SHADER_OFFSET) & 0xfffffffc;

	/* Fragment shader */
	*cmd++ = cp_type3_packet(CP_COND_EXEC, 4);
	*cmd++ = drawctxt->cond_execs[1].gpuaddr >> 2;
	*cmd++ = drawctxt->cond_execs[1].gpuaddr >> 2;
	*cmd++ = 1;
	*cmd++ = 3;		/* EXEC_COUNT */

	*cmd++ = cp_type3_packet(CP_LOAD_STATE, 2);
	drawctxt->shader_load_commands[1].hostptr = cmd;
	drawctxt->shader_load_commands[1].gpuaddr =
	    virt2gpu(cmd, &drawctxt->gpustate);
	/*
	   From fixup:

	   mode = 4 (indirect)
	   stateblock = 6 (Fragment shader)
	   numunits = SP_FS_CTRL_REG0.FS_LENGTH

	   From regspec:
	   SP_FS_CTRL_REG0.FS_LENGTH [31:24]: FS length, unit = 256bits.
	   If bit31 is 1, it means overflow
	   or any long shader.

	   ord1 = (numunits<<22) | (stateblock<<19) | (mode<<11)
	 */
	*cmd++ = 0;		/*ord1 */
	*cmd++ = (drawctxt->gpustate.gpuaddr + SHADER_OFFSET
		  + (SHADER_SHADOW_SIZE / 2)) & 0xfffffffc;

	create_ib1(drawctxt, drawctxt->shader_restore, start, cmd);
	tmp_ctx.cmd = cmd;
}

static void build_hlsqcontrol_restore_cmds(struct adreno_device *adreno_dev,
					   struct adreno_context *drawctxt)
{
	unsigned int *cmd = tmp_ctx.cmd;
	unsigned int *start = cmd;

	*cmd++ = cp_type3_packet(CP_SET_CONSTANT, 2);
	*cmd++ = CP_REG(A3XX_HLSQ_CONTROL_0_REG);
	drawctxt->hlsqcontrol_restore_commands[0].hostptr = cmd;
	drawctxt->hlsqcontrol_restore_commands[0].gpuaddr
	    = virt2gpu(cmd, &drawctxt->gpustate);
	*cmd++ = 0;

	/* Create indirect buffer command for above command sequence */
	create_ib1(drawctxt, drawctxt->hlsqcontrol_restore, start, cmd);

	tmp_ctx.cmd = cmd;
}

/* IB that modifies the shader and constant sizes and offsets in restore IBs. */
static void build_restore_fixup_cmds(struct adreno_device *adreno_dev,
				     struct adreno_context *drawctxt)
{
	unsigned int *cmd = tmp_ctx.cmd;
	unsigned int *start = cmd;

#ifdef GSL_CONTEXT_SWITCH_CPU_SYNC
	/* Save shader sizes */
	*cmd++ = cp_type3_packet(CP_REG_TO_MEM, 2);
	*cmd++ = A3XX_SP_VS_CTRL_REG0;
	*cmd++ = drawctxt->shader_load_commands[0].gpuaddr;

	*cmd++ = cp_type3_packet(CP_REG_TO_MEM, 2);
	*cmd++ = A3XX_SP_FS_CTRL_REG0;
	*cmd++ = drawctxt->shader_load_commands[1].gpuaddr;

	/* Save constant sizes */
	*cmd++ = cp_type3_packet(CP_REG_TO_MEM, 2);
	*cmd++ = A3XX_SP_VS_CTRL_REG1;
	*cmd++ = drawctxt->constant_load_commands[0].gpuaddr;

	*cmd++ = cp_type3_packet(CP_REG_TO_MEM, 2);
	*cmd++ = A3XX_SP_FS_CTRL_REG1;
	*cmd++ = drawctxt->constant_load_commands[1].gpuaddr;

	/* Save constant offsets */
	*cmd++ = cp_type3_packet(CP_REG_TO_MEM, 2);
	*cmd++ = A3XX_SP_FS_OBJ_OFFSET_REG;
	*cmd++ = drawctxt->constant_load_commands[2].gpuaddr;
#else
	/* Save shader sizes */
	cmd = rmw_regtomem(cmd, A3XX_SP_VS_CTRL_REG0, 0x7f000000,
			   30, (4 << 19) | (4 << 16),
			   drawctxt->shader_load_commands[0].gpuaddr);

	cmd = rmw_regtomem(cmd, A3XX_SP_FS_CTRL_REG0, 0x7f000000,
			   30, (6 << 19) | (4 << 16),
			   drawctxt->shader_load_commands[1].gpuaddr);

	/* Save constant sizes */
	cmd = rmw_regtomem(cmd, A3XX_SP_VS_CTRL_REG1, 0x000003ff,
			   23, (4 << 19) | (4 << 16),
			   drawctxt->constant_load_commands[0].gpuaddr);

	cmd = rmw_regtomem(cmd, A3XX_SP_FS_CTRL_REG1, 0x000003ff,
			   23, (6 << 19) | (4 << 16),
			   drawctxt->constant_load_commands[1].gpuaddr);

	/* Modify constant restore conditionals */
	cmd = rmw_regtomem(cmd, A3XX_SP_VS_CTRL_REG1, 0x000003ff,
			0, 0, drawctxt->cond_execs[2].gpuaddr);

	cmd = rmw_regtomem(cmd, A3XX_SP_FS_CTRL_REG1, 0x000003ff,
			0, 0, drawctxt->cond_execs[3].gpuaddr);

	/* Save fragment constant shadow offset */
	cmd = rmw_regtomem(cmd, A3XX_SP_FS_OBJ_OFFSET_REG, 0x00ff0000,
			   18, (drawctxt->gpustate.gpuaddr & 0xfffffe00) | 1,
			   drawctxt->constant_load_commands[2].gpuaddr);
#endif

	/* Use mask value to avoid flushing HLSQ which would cause the HW to
	   discard all the shader data */

	cmd = rmw_regtomem(cmd,  A3XX_HLSQ_CONTROL_0_REG, 0x9ffffdff,
		0, 0, drawctxt->hlsqcontrol_restore_commands[0].gpuaddr);

	create_ib1(drawctxt, drawctxt->restore_fixup, start, cmd);

	tmp_ctx.cmd = cmd;
}

static int a3xx_create_gpustate_shadow(struct adreno_device *adreno_dev,
				     struct adreno_context *drawctxt)
{
	drawctxt->flags |= CTXT_FLAGS_STATE_SHADOW;

	build_regrestore_cmds(adreno_dev, drawctxt);
	build_constantrestore_cmds(adreno_dev, drawctxt);
	build_hlsqcontrol_restore_cmds(adreno_dev, drawctxt);
	build_regconstantsave_cmds(adreno_dev, drawctxt);
	build_shader_save_cmds(adreno_dev, drawctxt);
	build_shader_restore_cmds(adreno_dev, drawctxt);
	build_restore_fixup_cmds(adreno_dev, drawctxt);
	build_save_fixup_cmds(adreno_dev, drawctxt);

	return 0;
}

/* create buffers for saving/restoring registers, constants, & GMEM */
static int a3xx_create_gmem_shadow(struct adreno_device *adreno_dev,
				 struct adreno_context *drawctxt)
{
	int result;

	calc_gmemsize(&drawctxt->context_gmem_shadow, adreno_dev->gmem_size);
	tmp_ctx.gmem_base = adreno_dev->gmem_base;

	result = kgsl_allocate(&drawctxt->context_gmem_shadow.gmemshadow,
		drawctxt->pagetable, drawctxt->context_gmem_shadow.size);

	if (result)
		return result;

	build_quad_vtxbuff(drawctxt, &drawctxt->context_gmem_shadow,
		&tmp_ctx.cmd);

	tmp_ctx.cmd = build_gmem2sys_cmds(adreno_dev, drawctxt,
		&drawctxt->context_gmem_shadow);
	tmp_ctx.cmd = build_sys2gmem_cmds(adreno_dev, drawctxt,
		&drawctxt->context_gmem_shadow);

	kgsl_cache_range_op(&drawctxt->context_gmem_shadow.gmemshadow,
		KGSL_CACHE_OP_FLUSH);

	drawctxt->flags |= CTXT_FLAGS_GMEM_SHADOW;

	return 0;
}

static int a3xx_drawctxt_create(struct adreno_device *adreno_dev,
	struct adreno_context *drawctxt)
{
	int ret;

	/*
	 * Allocate memory for the GPU state and the context commands.
	 * Despite the name, this is much more then just storage for
	 * the gpustate.  This contains command space for gmem save
	 * and texture and vertex buffer storage too
	 */

	ret = kgsl_allocate(&drawctxt->gpustate,
		drawctxt->pagetable, CONTEXT_SIZE);

	if (ret)
		return ret;

	kgsl_sharedmem_set(&drawctxt->gpustate, 0, 0, CONTEXT_SIZE);
	tmp_ctx.cmd = drawctxt->gpustate.hostptr + CMD_OFFSET;

	if (!(drawctxt->flags & CTXT_FLAGS_PREAMBLE)) {
		ret = a3xx_create_gpustate_shadow(adreno_dev, drawctxt);
		if (ret)
			goto done;

		drawctxt->flags |= CTXT_FLAGS_SHADER_SAVE;
	}

	if (!(drawctxt->flags & CTXT_FLAGS_NOGMEMALLOC))
		ret = a3xx_create_gmem_shadow(adreno_dev, drawctxt);

done:
	if (ret)
		kgsl_sharedmem_free(&drawctxt->gpustate);

	return ret;
}

static void a3xx_drawctxt_save(struct adreno_device *adreno_dev,
			   struct adreno_context *context)
{
	struct kgsl_device *device = &adreno_dev->dev;

	if (context == NULL || (context->flags & CTXT_FLAGS_BEING_DESTROYED))
		return;

	if (context->flags & CTXT_FLAGS_GPU_HANG)
		KGSL_CTXT_WARN(device,
			       "Current active context has caused gpu hang\n");

	if (!(context->flags & CTXT_FLAGS_PREAMBLE)) {
		/* Fixup self modifying IBs for save operations */
		adreno_ringbuffer_issuecmds(device, context,
			KGSL_CMD_FLAGS_NONE, context->save_fixup, 3);

		/* save registers and constants. */
		adreno_ringbuffer_issuecmds(device, context,
			KGSL_CMD_FLAGS_NONE,
			context->regconstant_save, 3);

		if (context->flags & CTXT_FLAGS_SHADER_SAVE) {
			/* Save shader instructions */
			adreno_ringbuffer_issuecmds(device, context,
				KGSL_CMD_FLAGS_PMODE, context->shader_save, 3);

			context->flags |= CTXT_FLAGS_SHADER_RESTORE;
		}
	}

	if ((context->flags & CTXT_FLAGS_GMEM_SAVE) &&
	    (context->flags & CTXT_FLAGS_GMEM_SHADOW)) {
		/*
		 * Save GMEM (note: changes shader. shader must
		 * already be saved.)
		 */

		kgsl_cffdump_syncmem(NULL,
			&context->gpustate,
			context->context_gmem_shadow.gmem_save[1],
			context->context_gmem_shadow.gmem_save[2] << 2, true);

		adreno_ringbuffer_issuecmds(device, context,
					KGSL_CMD_FLAGS_PMODE,
					    context->context_gmem_shadow.
					    gmem_save, 3);
		context->flags |= CTXT_FLAGS_GMEM_RESTORE;
	}
}

static void a3xx_drawctxt_restore(struct adreno_device *adreno_dev,
			      struct adreno_context *context)
{
	struct kgsl_device *device = &adreno_dev->dev;
	unsigned int cmds[5];

	if (context == NULL) {
		/* No context - set the default pagetable and thats it */
		kgsl_mmu_setstate(&device->mmu, device->mmu.defaultpagetable,
				adreno_dev->drawctxt_active->id);
		return;
	}

	KGSL_CTXT_INFO(device, "context flags %08x\n", context->flags);

	cmds[0] = cp_nop_packet(1);
	cmds[1] = KGSL_CONTEXT_TO_MEM_IDENTIFIER;
	cmds[2] = cp_type3_packet(CP_MEM_WRITE, 2);
	cmds[3] = device->memstore.gpuaddr +
		KGSL_MEMSTORE_OFFSET(KGSL_MEMSTORE_GLOBAL, current_context);
	cmds[4] = context->id;
	adreno_ringbuffer_issuecmds(device, context, KGSL_CMD_FLAGS_NONE,
					cmds, 5);
	kgsl_mmu_setstate(&device->mmu, context->pagetable, context->id);

	/*
	 * Restore GMEM.  (note: changes shader.
	 * Shader must not already be restored.)
	 */

	if (context->flags & CTXT_FLAGS_GMEM_RESTORE) {
		kgsl_cffdump_syncmem(NULL,
			&context->gpustate,
			context->context_gmem_shadow.gmem_restore[1],
			context->context_gmem_shadow.gmem_restore[2] << 2,
			true);

		adreno_ringbuffer_issuecmds(device, context,
					KGSL_CMD_FLAGS_PMODE,
					    context->context_gmem_shadow.
					    gmem_restore, 3);
		context->flags &= ~CTXT_FLAGS_GMEM_RESTORE;
	}

	if (!(context->flags & CTXT_FLAGS_PREAMBLE)) {
		adreno_ringbuffer_issuecmds(device, context,
			KGSL_CMD_FLAGS_NONE, context->reg_restore, 3);

		/* Fixup self modifying IBs for restore operations */
		adreno_ringbuffer_issuecmds(device, context,
			KGSL_CMD_FLAGS_NONE,
			context->restore_fixup, 3);

		adreno_ringbuffer_issuecmds(device, context,
			KGSL_CMD_FLAGS_NONE,
			context->constant_restore, 3);

		if (context->flags & CTXT_FLAGS_SHADER_RESTORE)
			adreno_ringbuffer_issuecmds(device, context,
				KGSL_CMD_FLAGS_NONE,
				context->shader_restore, 3);

		/* Restore HLSQ_CONTROL_0 register */
		adreno_ringbuffer_issuecmds(device, context,
			KGSL_CMD_FLAGS_NONE,
			context->hlsqcontrol_restore, 3);
	}
}

static int a3xx_rb_init(struct adreno_device *adreno_dev,
			 struct adreno_ringbuffer *rb)
{
	unsigned int *cmds, cmds_gpu;
	cmds = adreno_ringbuffer_allocspace(rb, NULL, 18);
	if (cmds == NULL)
		return -ENOMEM;

	cmds_gpu = rb->buffer_desc.gpuaddr + sizeof(uint) * (rb->wptr - 18);

	GSL_RB_WRITE(cmds, cmds_gpu, cp_type3_packet(CP_ME_INIT, 17));
	GSL_RB_WRITE(cmds, cmds_gpu, 0x000003f7);
	GSL_RB_WRITE(cmds, cmds_gpu, 0x00000000);
	GSL_RB_WRITE(cmds, cmds_gpu, 0x00000000);
	GSL_RB_WRITE(cmds, cmds_gpu, 0x00000000);
	GSL_RB_WRITE(cmds, cmds_gpu, 0x00000080);
	GSL_RB_WRITE(cmds, cmds_gpu, 0x00000100);
	GSL_RB_WRITE(cmds, cmds_gpu, 0x00000180);
	GSL_RB_WRITE(cmds, cmds_gpu, 0x00006600);
	GSL_RB_WRITE(cmds, cmds_gpu, 0x00000150);
	GSL_RB_WRITE(cmds, cmds_gpu, 0x0000014e);
	GSL_RB_WRITE(cmds, cmds_gpu, 0x00000154);
	GSL_RB_WRITE(cmds, cmds_gpu, 0x00000001);
	GSL_RB_WRITE(cmds, cmds_gpu, 0x00000000);
	GSL_RB_WRITE(cmds, cmds_gpu, 0x00000000);
	/* Enable protected mode registers for A3XX */
	GSL_RB_WRITE(cmds, cmds_gpu, 0x20000000);
	GSL_RB_WRITE(cmds, cmds_gpu, 0x00000000);
	GSL_RB_WRITE(cmds, cmds_gpu, 0x00000000);

	adreno_ringbuffer_submit(rb);

	return 0;
}

static void a3xx_err_callback(struct adreno_device *adreno_dev, int bit)
{
	struct kgsl_device *device = &adreno_dev->dev;
	const char *err = "";

	switch (bit) {
	case A3XX_INT_RBBM_AHB_ERROR: {
		unsigned int reg;

		adreno_regread(device, A3XX_RBBM_AHB_ERROR_STATUS, &reg);

		/*
		 * Return the word address of the erroring register so that it
		 * matches the register specification
		 */

		KGSL_DRV_CRIT(device,
			"RBBM | AHB bus error | %s | addr=%x | ports=%x:%x\n",
			reg & (1 << 28) ? "WRITE" : "READ",
			(reg & 0xFFFFF) >> 2, (reg >> 20) & 0x3,
			(reg >> 24) & 0x3);

		/* Clear the error */
		adreno_regwrite(device, A3XX_RBBM_AHB_CMD, (1 << 3));
		return;
	}
	case A3XX_INT_RBBM_REG_TIMEOUT:
		err = "RBBM: AHB register timeout";
		break;
	case A3XX_INT_RBBM_ME_MS_TIMEOUT:
		err = "RBBM: ME master split timeout";
		break;
	case A3XX_INT_RBBM_PFP_MS_TIMEOUT:
		err = "RBBM: PFP master split timeout";
		break;
	case A3XX_INT_RBBM_ATB_BUS_OVERFLOW:
		err = "RBBM: ATB bus oveflow";
		break;
	case A3XX_INT_VFD_ERROR:
		err = "VFD: Out of bounds access";
		break;
	case A3XX_INT_CP_T0_PACKET_IN_IB:
		err = "ringbuffer TO packet in IB interrupt";
		break;
	case A3XX_INT_CP_OPCODE_ERROR:
		err = "ringbuffer opcode error interrupt";
		break;
	case A3XX_INT_CP_RESERVED_BIT_ERROR:
		err = "ringbuffer reserved bit error interrupt";
		break;
	case A3XX_INT_CP_HW_FAULT:
		err = "ringbuffer hardware fault";
		break;
	case A3XX_INT_CP_REG_PROTECT_FAULT: {
		unsigned int reg;
		kgsl_regread(device, A3XX_CP_PROTECT_STATUS, &reg);

		KGSL_DRV_CRIT(device,
			"CP | Protected mode error| %s | addr=%x\n",
			reg & (1 << 24) ? "WRITE" : "READ",
			(reg & 0x1FFFF) >> 2);
		return;
	}
	case A3XX_INT_CP_AHB_ERROR_HALT:
		err = "ringbuffer AHB error interrupt";
		break;
	case A3XX_INT_MISC_HANG_DETECT:
		err = "MISC: GPU hang detected";
		break;
	case A3XX_INT_UCHE_OOB_ACCESS:
		err = "UCHE:  Out of bounds access";
		break;
	}

	KGSL_DRV_CRIT(device, "%s\n", err);
	kgsl_pwrctrl_irq(device, KGSL_PWRFLAGS_OFF);
}

static void a3xx_cp_callback(struct adreno_device *adreno_dev, int irq)
{
	struct kgsl_device *device = &adreno_dev->dev;

	/* Wake up everybody waiting for the interrupt */
	wake_up_interruptible_all(&device->wait_queue);

	/* Schedule work to free mem and issue ibs */
	queue_work(device->work_queue, &device->ts_expired_ws);
}

/**
 * struct a3xx_perfcounter_register - Define a performance counter register
 * @load_bit: the bit to set in RBBM_LOAD_CMD0/RBBM_LOAD_CMD1 to force the RBBM
 * to load the reset value into the appropriate counter
 * @select: The dword offset of the register to write the selected
 * countable into
 */

struct a3xx_perfcounter_register {
	unsigned int load_bit;
	unsigned int select;
};

static struct a3xx_perfcounter_register a3xx_perfcounter_reg_cp[] = {
	{ 0, A3XX_CP_PERFCOUNTER_SELECT },
};

static struct a3xx_perfcounter_register a3xx_perfcounter_reg_rbbm[] = {
	{ 1, A3XX_RBBM_PERFCOUNTER0_SELECT },
	{ 2, A3XX_RBBM_PERFCOUNTER1_SELECT },
};

static struct a3xx_perfcounter_register a3xx_perfcounter_reg_pc[] = {
	{ 3, A3XX_PC_PERFCOUNTER0_SELECT },
	{ 4, A3XX_PC_PERFCOUNTER1_SELECT },
	{ 5, A3XX_PC_PERFCOUNTER2_SELECT },
	{ 6, A3XX_PC_PERFCOUNTER3_SELECT },
};

static struct a3xx_perfcounter_register a3xx_perfcounter_reg_vfd[] = {
	{ 7, A3XX_VFD_PERFCOUNTER0_SELECT },
	{ 8, A3XX_VFD_PERFCOUNTER1_SELECT },
};

static struct a3xx_perfcounter_register a3xx_perfcounter_reg_hlsq[] = {
	{ 9, A3XX_HLSQ_PERFCOUNTER0_SELECT },
	{ 10, A3XX_HLSQ_PERFCOUNTER1_SELECT },
	{ 11, A3XX_HLSQ_PERFCOUNTER2_SELECT },
	{ 12, A3XX_HLSQ_PERFCOUNTER3_SELECT },
	{ 13, A3XX_HLSQ_PERFCOUNTER4_SELECT },
	{ 14, A3XX_HLSQ_PERFCOUNTER5_SELECT },
};

static struct a3xx_perfcounter_register a3xx_perfcounter_reg_vpc[] = {
	{ 15, A3XX_VPC_PERFCOUNTER0_SELECT },
	{ 16, A3XX_VPC_PERFCOUNTER1_SELECT },
};

static struct a3xx_perfcounter_register a3xx_perfcounter_reg_tse[] = {
	{ 17, A3XX_GRAS_PERFCOUNTER0_SELECT },
	{ 18, A3XX_GRAS_PERFCOUNTER1_SELECT },
};

static struct a3xx_perfcounter_register a3xx_perfcounter_reg_ras[] = {
	{ 19, A3XX_GRAS_PERFCOUNTER2_SELECT },
	{ 20, A3XX_GRAS_PERFCOUNTER3_SELECT },
};

static struct a3xx_perfcounter_register a3xx_perfcounter_reg_uche[] = {
	{ 21, A3XX_UCHE_PERFCOUNTER0_SELECT },
	{ 22, A3XX_UCHE_PERFCOUNTER1_SELECT },
	{ 23, A3XX_UCHE_PERFCOUNTER2_SELECT },
	{ 24, A3XX_UCHE_PERFCOUNTER3_SELECT },
	{ 25, A3XX_UCHE_PERFCOUNTER4_SELECT },
	{ 26, A3XX_UCHE_PERFCOUNTER5_SELECT },
};

static struct a3xx_perfcounter_register a3xx_perfcounter_reg_tp[] = {
	{ 27, A3XX_TP_PERFCOUNTER0_SELECT },
	{ 28, A3XX_TP_PERFCOUNTER1_SELECT },
	{ 29, A3XX_TP_PERFCOUNTER2_SELECT },
	{ 30, A3XX_TP_PERFCOUNTER3_SELECT },
	{ 31, A3XX_TP_PERFCOUNTER4_SELECT },
	{ 32, A3XX_TP_PERFCOUNTER5_SELECT },
};

static struct a3xx_perfcounter_register a3xx_perfcounter_reg_sp[] = {
	{ 33, A3XX_SP_PERFCOUNTER0_SELECT },
	{ 34, A3XX_SP_PERFCOUNTER1_SELECT },
	{ 35, A3XX_SP_PERFCOUNTER2_SELECT },
	{ 36, A3XX_SP_PERFCOUNTER3_SELECT },
	{ 37, A3XX_SP_PERFCOUNTER4_SELECT },
	{ 38, A3XX_SP_PERFCOUNTER5_SELECT },
	{ 39, A3XX_SP_PERFCOUNTER6_SELECT },
	{ 40, A3XX_SP_PERFCOUNTER7_SELECT },
};

static struct a3xx_perfcounter_register a3xx_perfcounter_reg_rb[] = {
	{ 41, A3XX_RB_PERFCOUNTER0_SELECT },
	{ 42, A3XX_RB_PERFCOUNTER1_SELECT },
};

#define REGCOUNTER_GROUP(_x) { (_x), ARRAY_SIZE((_x)) }

static struct {
	struct a3xx_perfcounter_register *regs;
	int count;
} a3xx_perfcounter_reglist[] = {
	REGCOUNTER_GROUP(a3xx_perfcounter_reg_cp),
	REGCOUNTER_GROUP(a3xx_perfcounter_reg_rbbm),
	REGCOUNTER_GROUP(a3xx_perfcounter_reg_pc),
	REGCOUNTER_GROUP(a3xx_perfcounter_reg_vfd),
	REGCOUNTER_GROUP(a3xx_perfcounter_reg_hlsq),
	REGCOUNTER_GROUP(a3xx_perfcounter_reg_vpc),
	REGCOUNTER_GROUP(a3xx_perfcounter_reg_tse),
	REGCOUNTER_GROUP(a3xx_perfcounter_reg_ras),
	REGCOUNTER_GROUP(a3xx_perfcounter_reg_uche),
	REGCOUNTER_GROUP(a3xx_perfcounter_reg_tp),
	REGCOUNTER_GROUP(a3xx_perfcounter_reg_sp),
	REGCOUNTER_GROUP(a3xx_perfcounter_reg_rb),
};

static void a3xx_perfcounter_enable_pwr(struct kgsl_device *device,
	unsigned int countable)
{
	unsigned int in, out;

	adreno_regread(device, A3XX_RBBM_RBBM_CTL, &in);

	if (countable == 0)
		out = in | RBBM_RBBM_CTL_RESET_PWR_CTR0;
	else
		out = in | RBBM_RBBM_CTL_RESET_PWR_CTR1;

	adreno_regwrite(device, A3XX_RBBM_RBBM_CTL, out);

	if (countable == 0)
		out = in | RBBM_RBBM_CTL_ENABLE_PWR_CTR0;
	else
		out = in | RBBM_RBBM_CTL_ENABLE_PWR_CTR1;

	adreno_regwrite(device, A3XX_RBBM_RBBM_CTL, out);

	return;
}

static void a3xx_perfcounter_enable_vbif(struct kgsl_device *device,
					 unsigned int counter,
					 unsigned int countable)
{
	unsigned int in, out, bit, sel;

	if (counter > 1 || countable > 0x7f)
		return;

	adreno_regread(device, A3XX_VBIF_PERF_CNT_EN, &in);
	adreno_regread(device, A3XX_VBIF_PERF_CNT_SEL, &sel);

	if (counter == 0) {
		bit = VBIF_PERF_CNT_0;
		sel = (sel & ~VBIF_PERF_CNT_0_SEL_MASK) | countable;
	} else {
		bit = VBIF_PERF_CNT_1;
		sel = (sel & ~VBIF_PERF_CNT_1_SEL_MASK)
			| (countable << VBIF_PERF_CNT_1_SEL);
	}

	out = in | bit;

	adreno_regwrite(device, A3XX_VBIF_PERF_CNT_SEL, sel);

	adreno_regwrite(device, A3XX_VBIF_PERF_CNT_CLR, bit);
	adreno_regwrite(device, A3XX_VBIF_PERF_CNT_CLR, 0);

	adreno_regwrite(device, A3XX_VBIF_PERF_CNT_EN, out);
}

static void a3xx_perfcounter_enable_vbif_pwr(struct kgsl_device *device,
					     unsigned int countable)
{
	unsigned int in, out, bit;

	adreno_regread(device, A3XX_VBIF_PERF_CNT_EN, &in);
	if (countable == 0)
		bit = VBIF_PERF_PWR_CNT_0;
	else if (countable == 1)
		bit = VBIF_PERF_PWR_CNT_1;
	else
		bit = VBIF_PERF_PWR_CNT_2;

	out = in | bit;

	adreno_regwrite(device, A3XX_VBIF_PERF_CNT_CLR, bit);
	adreno_regwrite(device, A3XX_VBIF_PERF_CNT_CLR, 0);

	adreno_regwrite(device, A3XX_VBIF_PERF_CNT_EN, out);
}

/*
 * a3xx_perfcounter_enable - Configure a performance counter for a countable
 * @adreno_dev -  Adreno device to configure
 * @group - Desired performance counter group
 * @counter - Desired performance counter in the group
 * @countable - Desired countable
 *
 * Physically set up a counter within a group with the desired countable
 */

static void a3xx_perfcounter_enable(struct adreno_device *adreno_dev,
	unsigned int group, unsigned int counter, unsigned int countable)
{
	struct kgsl_device *device = &adreno_dev->dev;
	unsigned int val = 0;
	struct a3xx_perfcounter_register *reg;

	if (group >= ARRAY_SIZE(a3xx_perfcounter_reglist))
		return;

	if (counter >= a3xx_perfcounter_reglist[group].count)
		return;

	/* Special cases */
	if (group == KGSL_PERFCOUNTER_GROUP_PWR)
		return a3xx_perfcounter_enable_pwr(device, countable);
	else if (group == KGSL_PERFCOUNTER_GROUP_VBIF)
		return a3xx_perfcounter_enable_vbif(device, counter, countable);
	else if (group == KGSL_PERFCOUNTER_GROUP_VBIF_PWR)
		return a3xx_perfcounter_enable_vbif_pwr(device, countable);

	reg = &(a3xx_perfcounter_reglist[group].regs[counter]);

	/* Select the desired perfcounter */
	adreno_regwrite(device, reg->select, countable);

	if (reg->load_bit < 32) {
		val = 1 << reg->load_bit;
		adreno_regwrite(device, A3XX_RBBM_PERFCTR_LOAD_CMD0, val);
	} else {
		val  = 1 << (reg->load_bit - 32);
		adreno_regwrite(device, A3XX_RBBM_PERFCTR_LOAD_CMD1, val);
	}
}

static uint64_t a3xx_perfcounter_read(struct adreno_device *adreno_dev,
	unsigned int group, unsigned int counter,
	unsigned int offset)
{
	struct kgsl_device *device = &adreno_dev->dev;
	struct a3xx_perfcounter_register *reg = NULL;
	unsigned int lo = 0, hi = 0;
	unsigned int val;

	if (group >= ARRAY_SIZE(a3xx_perfcounter_reglist))
		return 0;

	if (counter >= a3xx_perfcounter_reglist[group].count)
		return 0;

	reg = &(a3xx_perfcounter_reglist[group].regs[counter]);

	/* Freeze the counter */
	adreno_regread(device, A3XX_RBBM_PERFCTR_CTL, &val);
	val &= ~reg->load_bit;
	adreno_regwrite(device, A3XX_RBBM_PERFCTR_CTL, val);

	/* Read the values */
	adreno_regread(device, offset, &lo);
	adreno_regread(device, offset + 1, &hi);

	/* Re-Enable the counter */
	val |= reg->load_bit;
	adreno_regwrite(device, A3XX_RBBM_PERFCTR_CTL, val);

	return (((uint64_t) hi) << 32) | lo;
}

#define A3XX_IRQ_CALLBACK(_c) { .func = _c }

#define A3XX_INT_MASK \
	((1 << A3XX_INT_RBBM_AHB_ERROR) |        \
	 (1 << A3XX_INT_RBBM_ATB_BUS_OVERFLOW) | \
	 (1 << A3XX_INT_CP_T0_PACKET_IN_IB) |    \
	 (1 << A3XX_INT_CP_OPCODE_ERROR) |       \
	 (1 << A3XX_INT_CP_RESERVED_BIT_ERROR) | \
	 (1 << A3XX_INT_CP_HW_FAULT) |           \
	 (1 << A3XX_INT_CP_IB1_INT) |            \
	 (1 << A3XX_INT_CP_IB2_INT) |            \
	 (1 << A3XX_INT_CP_RB_INT) |             \
	 (1 << A3XX_INT_CP_REG_PROTECT_FAULT) |  \
	 (1 << A3XX_INT_CP_AHB_ERROR_HALT) |     \
	 (1 << A3XX_INT_UCHE_OOB_ACCESS))

static struct {
	void (*func)(struct adreno_device *, int);
} a3xx_irq_funcs[] = {
	A3XX_IRQ_CALLBACK(NULL),               /* 0 - RBBM_GPU_IDLE */
	A3XX_IRQ_CALLBACK(a3xx_err_callback),  /* 1 - RBBM_AHB_ERROR */
	A3XX_IRQ_CALLBACK(a3xx_err_callback),  /* 2 - RBBM_REG_TIMEOUT */
	A3XX_IRQ_CALLBACK(a3xx_err_callback),  /* 3 - RBBM_ME_MS_TIMEOUT */
	A3XX_IRQ_CALLBACK(a3xx_err_callback),  /* 4 - RBBM_PFP_MS_TIMEOUT */
	A3XX_IRQ_CALLBACK(a3xx_err_callback),  /* 5 - RBBM_ATB_BUS_OVERFLOW */
	A3XX_IRQ_CALLBACK(a3xx_err_callback),  /* 6 - RBBM_VFD_ERROR */
	A3XX_IRQ_CALLBACK(NULL),	       /* 7 - CP_SW */
	A3XX_IRQ_CALLBACK(a3xx_err_callback),  /* 8 - CP_T0_PACKET_IN_IB */
	A3XX_IRQ_CALLBACK(a3xx_err_callback),  /* 9 - CP_OPCODE_ERROR */
	A3XX_IRQ_CALLBACK(a3xx_err_callback),  /* 10 - CP_RESERVED_BIT_ERROR */
	A3XX_IRQ_CALLBACK(a3xx_err_callback),  /* 11 - CP_HW_FAULT */
	A3XX_IRQ_CALLBACK(NULL),	       /* 12 - CP_DMA */
	A3XX_IRQ_CALLBACK(a3xx_cp_callback),   /* 13 - CP_IB2_INT */
	A3XX_IRQ_CALLBACK(a3xx_cp_callback),   /* 14 - CP_IB1_INT */
	A3XX_IRQ_CALLBACK(a3xx_cp_callback),   /* 15 - CP_RB_INT */
	A3XX_IRQ_CALLBACK(a3xx_err_callback),  /* 16 - CP_REG_PROTECT_FAULT */
	A3XX_IRQ_CALLBACK(NULL),	       /* 17 - CP_RB_DONE_TS */
	A3XX_IRQ_CALLBACK(NULL),	       /* 18 - CP_VS_DONE_TS */
	A3XX_IRQ_CALLBACK(NULL),	       /* 19 - CP_PS_DONE_TS */
	A3XX_IRQ_CALLBACK(NULL),	       /* 20 - CP_CACHE_FLUSH_TS */
	A3XX_IRQ_CALLBACK(a3xx_err_callback),  /* 21 - CP_AHB_ERROR_FAULT */
	A3XX_IRQ_CALLBACK(NULL),	       /* 22 - Unused */
	A3XX_IRQ_CALLBACK(NULL),	       /* 23 - Unused */
	A3XX_IRQ_CALLBACK(NULL),	       /* 24 - MISC_HANG_DETECT */
	A3XX_IRQ_CALLBACK(a3xx_err_callback),  /* 25 - UCHE_OOB_ACCESS */
	/* 26 to 31 - Unused */
};

static irqreturn_t a3xx_irq_handler(struct adreno_device *adreno_dev)
{
	struct kgsl_device *device = &adreno_dev->dev;
	irqreturn_t ret = IRQ_NONE;
	unsigned int status, tmp;
	int i;

	adreno_regread(&adreno_dev->dev, A3XX_RBBM_INT_0_STATUS, &status);

	for (tmp = status, i = 0; tmp && i < ARRAY_SIZE(a3xx_irq_funcs); i++) {
		if (tmp & 1) {
			if (a3xx_irq_funcs[i].func != NULL) {
				a3xx_irq_funcs[i].func(adreno_dev, i);
				ret = IRQ_HANDLED;
			} else {
				KGSL_DRV_CRIT(device,
					"Unhandled interrupt bit %x\n", i);
			}
		}

		tmp >>= 1;
	}

	trace_kgsl_a3xx_irq_status(device, status);

	if (status)
		adreno_regwrite(&adreno_dev->dev, A3XX_RBBM_INT_CLEAR_CMD,
			status);
	return ret;
}

static void a3xx_irq_control(struct adreno_device *adreno_dev, int state)
{
	struct kgsl_device *device = &adreno_dev->dev;

	if (state)
		adreno_regwrite(device, A3XX_RBBM_INT_0_MASK, A3XX_INT_MASK);
	else
		adreno_regwrite(device, A3XX_RBBM_INT_0_MASK, 0);
}

static unsigned int a3xx_irq_pending(struct adreno_device *adreno_dev)
{
	unsigned int status;

	adreno_regread(&adreno_dev->dev, A3XX_RBBM_INT_0_STATUS, &status);

	return (status & A3XX_INT_MASK) ? 1 : 0;
}

static unsigned int a3xx_busy_cycles(struct adreno_device *adreno_dev)
{
	struct kgsl_device *device = &adreno_dev->dev;
	unsigned int val;
	unsigned int ret = 0;

	/* Read the value */
	adreno_regread(device, A3XX_RBBM_PERFCTR_PWR_1_LO, &val);

	/* Return 0 for the first read */
	if (adreno_dev->gpu_cycles != 0) {
		if (val < adreno_dev->gpu_cycles)
			ret = (0xFFFFFFFF - adreno_dev->gpu_cycles) + val;
		else
			ret = val - adreno_dev->gpu_cycles;
	}

	adreno_dev->gpu_cycles = val;
	return ret;
}

struct a3xx_vbif_data {
	unsigned int reg;
	unsigned int val;
};

/* VBIF registers start after 0x3000 so use 0x0 as end of list marker */
static struct a3xx_vbif_data a305_vbif[] = {
	/* Set up 16 deep read/write request queues */
	{ A3XX_VBIF_IN_RD_LIM_CONF0, 0x10101010 },
	{ A3XX_VBIF_IN_RD_LIM_CONF1, 0x10101010 },
	{ A3XX_VBIF_OUT_RD_LIM_CONF0, 0x10101010 },
	{ A3XX_VBIF_OUT_WR_LIM_CONF0, 0x10101010 },
	{ A3XX_VBIF_DDR_OUT_MAX_BURST, 0x0000303 },
	{ A3XX_VBIF_IN_WR_LIM_CONF0, 0x10101010 },
	{ A3XX_VBIF_IN_WR_LIM_CONF1, 0x10101010 },
	/* Enable WR-REQ */
	{ A3XX_VBIF_GATE_OFF_WRREQ_EN, 0x0000FF },
	/* Set up round robin arbitration between both AXI ports */
	{ A3XX_VBIF_ARB_CTL, 0x00000030 },
	/* Set up AOOO */
	{ A3XX_VBIF_OUT_AXI_AOOO_EN, 0x0000003C },
	{ A3XX_VBIF_OUT_AXI_AOOO, 0x003C003C },
	{0, 0},
};

static struct a3xx_vbif_data a320_vbif[] = {
	/* Set up 16 deep read/write request queues */
	{ A3XX_VBIF_IN_RD_LIM_CONF0, 0x10101010 },
	{ A3XX_VBIF_IN_RD_LIM_CONF1, 0x10101010 },
	{ A3XX_VBIF_OUT_RD_LIM_CONF0, 0x10101010 },
	{ A3XX_VBIF_OUT_WR_LIM_CONF0, 0x10101010 },
	{ A3XX_VBIF_DDR_OUT_MAX_BURST, 0x0000303 },
	{ A3XX_VBIF_IN_WR_LIM_CONF0, 0x10101010 },
	{ A3XX_VBIF_IN_WR_LIM_CONF1, 0x10101010 },
	/* Enable WR-REQ */
	{ A3XX_VBIF_GATE_OFF_WRREQ_EN, 0x0000FF },
	/* Set up round robin arbitration between both AXI ports */
	{ A3XX_VBIF_ARB_CTL, 0x00000030 },
	/* Set up AOOO */
	{ A3XX_VBIF_OUT_AXI_AOOO_EN, 0x0000003C },
	{ A3XX_VBIF_OUT_AXI_AOOO, 0x003C003C },
	/* Enable 1K sort */
	{ A3XX_VBIF_ABIT_SORT, 0x000000FF },
	{ A3XX_VBIF_ABIT_SORT_CONF, 0x000000A4 },
	{0, 0},
};

static struct a3xx_vbif_data a330_vbif[] = {
	/* Set up 16 deep read/write request queues */
	{ A3XX_VBIF_IN_RD_LIM_CONF0, 0x18181818 },
	{ A3XX_VBIF_IN_RD_LIM_CONF1, 0x00001818 },
	{ A3XX_VBIF_OUT_RD_LIM_CONF0, 0x00001818 },
	{ A3XX_VBIF_OUT_WR_LIM_CONF0, 0x00001818 },
	{ A3XX_VBIF_DDR_OUT_MAX_BURST, 0x0000303 },
	{ A3XX_VBIF_IN_WR_LIM_CONF0, 0x18181818 },
	{ A3XX_VBIF_IN_WR_LIM_CONF1, 0x00001818 },
	/* Enable WR-REQ */
	{ A3XX_VBIF_GATE_OFF_WRREQ_EN, 0x00003F },
	/* Set up round robin arbitration between both AXI ports */
	{ A3XX_VBIF_ARB_CTL, 0x00000030 },
	/* Set up VBIF_ROUND_ROBIN_QOS_ARB */
	{ A3XX_VBIF_ROUND_ROBIN_QOS_ARB, 0x0001 },
	/* Set up AOOO */
	{ A3XX_VBIF_OUT_AXI_AOOO_EN, 0x0000003F },
	{ A3XX_VBIF_OUT_AXI_AOOO, 0x003F003F },
	/* Enable 1K sort */
	{ A3XX_VBIF_ABIT_SORT, 0x0001003F },
	{ A3XX_VBIF_ABIT_SORT_CONF, 0x000000A4 },
	/* Disable VBIF clock gating. This is to enable AXI running
	 * higher frequency than GPU.
	 */
	{ A3XX_VBIF_CLKON, 1 },
	{0, 0},
};

/*
 * Most of the VBIF registers on 8974v2 have the correct values at power on, so
 * we won't modify those if we don't need to
 */
static struct a3xx_vbif_data a330v2_vbif[] = {
	/* Enable 1k sort */
	{ A3XX_VBIF_ABIT_SORT, 0x0001003F },
	{ A3XX_VBIF_ABIT_SORT_CONF, 0x000000A4 },
	/* Enable WR-REQ */
	{ A3XX_VBIF_GATE_OFF_WRREQ_EN, 0x00003F },
	{ A3XX_VBIF_DDR_OUT_MAX_BURST, 0x0000303 },
	/* Set up VBIF_ROUND_ROBIN_QOS_ARB */
	{ A3XX_VBIF_ROUND_ROBIN_QOS_ARB, 0x0003 },
	/* Disable VBIF clock gating. This is to enable AXI running
	 * higher frequency than GPU.
	 */
	{ A3XX_VBIF_CLKON, 1 },
	{0, 0},
};

static struct {
	int(*devfunc)(struct adreno_device *);
	struct a3xx_vbif_data *vbif;
} a3xx_vbif_platforms[] = {
	{ adreno_is_a305, a305_vbif },
	{ adreno_is_a320, a320_vbif },
	/* A330v2 needs to be ahead of A330 so the right device matches */
	{ adreno_is_a330v2, a330v2_vbif },
	{ adreno_is_a330, a330_vbif },
};

static void a3xx_perfcounter_init(struct adreno_device *adreno_dev)
{
	/*
	 * Set SP to count SP_ALU_ACTIVE_CYCLES, it includes
	 * all ALU instruction execution regardless precision or shader ID.
	 * Set SP to count SP0_ICL1_MISSES, It counts
	 * USP L1 instruction miss request.
	 * Set SP to count SP_FS_FULL_ALU_INSTRUCTIONS, it
	 * counts USP flow control instruction execution.
	 * we will use this to augment our hang detection
	 */
	if (adreno_dev->fast_hang_detect) {
		adreno_perfcounter_get(adreno_dev, KGSL_PERFCOUNTER_GROUP_SP,
			SP_ALU_ACTIVE_CYCLES, &ft_detect_regs[6],
			PERFCOUNTER_FLAG_KERNEL);
		ft_detect_regs[7] = ft_detect_regs[6] + 1;
		adreno_perfcounter_get(adreno_dev, KGSL_PERFCOUNTER_GROUP_SP,
			SP0_ICL1_MISSES, &ft_detect_regs[8],
			PERFCOUNTER_FLAG_KERNEL);
		ft_detect_regs[9] = ft_detect_regs[8] + 1;
		adreno_perfcounter_get(adreno_dev, KGSL_PERFCOUNTER_GROUP_SP,
			SP_FS_CFLOW_INSTRUCTIONS, &ft_detect_regs[10],
			PERFCOUNTER_FLAG_KERNEL);
		ft_detect_regs[11] = ft_detect_regs[10] + 1;
	}

	adreno_perfcounter_get(adreno_dev, KGSL_PERFCOUNTER_GROUP_SP,
		SP_FS_FULL_ALU_INSTRUCTIONS, NULL, PERFCOUNTER_FLAG_KERNEL);

	/* Reserve and start countable 1 in the PWR perfcounter group */
	adreno_perfcounter_get(adreno_dev, KGSL_PERFCOUNTER_GROUP_PWR, 1,
			NULL, PERFCOUNTER_FLAG_KERNEL);
}

/**
 * a3xx_protect_init() - Initializes register protection on a3xx
 * @device: Pointer to the device structure
 * Performs register writes to enable protected access to sensitive
 * registers
 */
static void a3xx_protect_init(struct kgsl_device *device)
{
	int index = 0;

	/* enable access protection to privileged registers */
	kgsl_regwrite(device, A3XX_CP_PROTECT_CTRL, 0x00000007);

	/* RBBM registers */
	adreno_set_protected_registers(device, &index, 0x18, 0);
	adreno_set_protected_registers(device, &index, 0x20, 2);
	adreno_set_protected_registers(device, &index, 0x33, 0);
	adreno_set_protected_registers(device, &index, 0x42, 0);
	adreno_set_protected_registers(device, &index, 0x50, 4);
	adreno_set_protected_registers(device, &index, 0x63, 0);
	adreno_set_protected_registers(device, &index, 0x100, 4);

	/* CP registers */
	adreno_set_protected_registers(device, &index, 0x1C0, 5);
	adreno_set_protected_registers(device, &index, 0x1EC, 1);
	adreno_set_protected_registers(device, &index, 0x1F6, 1);
	adreno_set_protected_registers(device, &index, 0x1F8, 2);
	adreno_set_protected_registers(device, &index, 0x45E, 2);
	adreno_set_protected_registers(device, &index, 0x460, 4);

	/* RB registers */
	adreno_set_protected_registers(device, &index, 0xCC0, 0);

	/* VBIF registers */
	adreno_set_protected_registers(device, &index, 0x3000, 6);

	/* SMMU registers */
	adreno_set_protected_registers(device, &index, 0x4000, 14);
}

static void a3xx_start(struct adreno_device *adreno_dev)
{
	struct kgsl_device *device = &adreno_dev->dev;
	struct a3xx_vbif_data *vbif = NULL;
	int i;

	for (i = 0; i < ARRAY_SIZE(a3xx_vbif_platforms); i++) {
		if (a3xx_vbif_platforms[i].devfunc(adreno_dev)) {
			vbif = a3xx_vbif_platforms[i].vbif;
			break;
		}
	}

	BUG_ON(vbif == NULL);

	while (vbif->reg != 0) {
		adreno_regwrite(device, vbif->reg, vbif->val);
		vbif++;
	}

	/* Make all blocks contribute to the GPU BUSY perf counter */
	adreno_regwrite(device, A3XX_RBBM_GPU_BUSY_MASKED, 0xFFFFFFFF);

	/* Tune the hystersis counters for SP and CP idle detection */
	adreno_regwrite(device, A3XX_RBBM_SP_HYST_CNT, 0x10);
	adreno_regwrite(device, A3XX_RBBM_WAIT_IDLE_CLOCKS_CTL, 0x10);

	/* Enable the RBBM error reporting bits.  This lets us get
	   useful information on failure */

	adreno_regwrite(device, A3XX_RBBM_AHB_CTL0, 0x00000001);

	/* Enable AHB error reporting */
	adreno_regwrite(device, A3XX_RBBM_AHB_CTL1, 0xA6FFFFFF);

	/* Turn on the power counters */
	adreno_regwrite(device, A3XX_RBBM_RBBM_CTL, 0x00030000);

	/* Turn on hang detection - this spews a lot of useful information
	 * into the RBBM registers on a hang */

	adreno_regwrite(device, A3XX_RBBM_INTERFACE_HANG_INT_CTL,
			(1 << 16) | 0xFFF);

	/* Enable 64-byte cacheline size. HW Default is 32-byte (0x000000E0). */
	adreno_regwrite(device, A3XX_UCHE_CACHE_MODE_CONTROL_REG, 0x00000001);

	/* Enable Clock gating */
	adreno_regwrite(device, A3XX_RBBM_CLOCK_CTL,
		adreno_a3xx_rbbm_clock_ctl_default(adreno_dev));

	if (adreno_is_a330v2(adreno_dev))
		adreno_regwrite(device, A3XX_RBBM_GPR0_CTL,
			A330v2_RBBM_GPR0_CTL_DEFAULT);
	else if (adreno_is_a330(adreno_dev))
		adreno_regwrite(device, A3XX_RBBM_GPR0_CTL,
			A330_RBBM_GPR0_CTL_DEFAULT);

	/* Set the OCMEM base address for A330 */
	if (adreno_is_a330(adreno_dev)) {
		adreno_regwrite(device, A3XX_RB_GMEM_BASE_ADDR,
			(unsigned int)(adreno_dev->ocmem_base >> 14));
	}
	/* Turn on protection */
	a3xx_protect_init(device);

	/* Turn on performance counters */
	adreno_regwrite(device, A3XX_RBBM_PERFCTR_CTL, 0x01);

	/* Turn on the GPU busy counter and let it run free */

	adreno_dev->gpu_cycles = 0;
}

/*
 * Define the available perfcounter groups - these get used by
 * adreno_perfcounter_get and adreno_perfcounter_put
 */

static struct adreno_perfcount_register a3xx_perfcounters_cp[] = {
	{ KGSL_PERFCOUNTER_NOT_USED, 0, A3XX_RBBM_PERFCTR_CP_0_LO, 0 },
};

static struct adreno_perfcount_register a3xx_perfcounters_rbbm[] = {
	{ KGSL_PERFCOUNTER_NOT_USED, 0, A3XX_RBBM_PERFCTR_RBBM_0_LO, 0 },
	{ KGSL_PERFCOUNTER_NOT_USED, 0, A3XX_RBBM_PERFCTR_RBBM_1_LO, 0 },
};

static struct adreno_perfcount_register a3xx_perfcounters_pc[] = {
	{ KGSL_PERFCOUNTER_NOT_USED, 0, A3XX_RBBM_PERFCTR_PC_0_LO, 0 },
	{ KGSL_PERFCOUNTER_NOT_USED, 0, A3XX_RBBM_PERFCTR_PC_1_LO, 0 },
	{ KGSL_PERFCOUNTER_NOT_USED, 0, A3XX_RBBM_PERFCTR_PC_2_LO, 0 },
	{ KGSL_PERFCOUNTER_NOT_USED, 0, A3XX_RBBM_PERFCTR_PC_3_LO, 0 },
};

static struct adreno_perfcount_register a3xx_perfcounters_vfd[] = {
	{ KGSL_PERFCOUNTER_NOT_USED, 0, A3XX_RBBM_PERFCTR_VFD_0_LO, 0 },
	{ KGSL_PERFCOUNTER_NOT_USED, 0, A3XX_RBBM_PERFCTR_VFD_1_LO, 0 },
};

static struct adreno_perfcount_register a3xx_perfcounters_hlsq[] = {
	{ KGSL_PERFCOUNTER_NOT_USED, 0, A3XX_RBBM_PERFCTR_HLSQ_0_LO, 0 },
	{ KGSL_PERFCOUNTER_NOT_USED, 0, A3XX_RBBM_PERFCTR_HLSQ_1_LO, 0 },
	{ KGSL_PERFCOUNTER_NOT_USED, 0, A3XX_RBBM_PERFCTR_HLSQ_2_LO, 0 },
	{ KGSL_PERFCOUNTER_NOT_USED, 0, A3XX_RBBM_PERFCTR_HLSQ_3_LO, 0 },
	{ KGSL_PERFCOUNTER_NOT_USED, 0, A3XX_RBBM_PERFCTR_HLSQ_4_LO, 0 },
	{ KGSL_PERFCOUNTER_NOT_USED, 0, A3XX_RBBM_PERFCTR_HLSQ_5_LO, 0 },
};

static struct adreno_perfcount_register a3xx_perfcounters_vpc[] = {
	{ KGSL_PERFCOUNTER_NOT_USED, 0, A3XX_RBBM_PERFCTR_VPC_0_LO, 0 },
	{ KGSL_PERFCOUNTER_NOT_USED, 0, A3XX_RBBM_PERFCTR_VPC_1_LO, 0 },
};

static struct adreno_perfcount_register a3xx_perfcounters_tse[] = {
	{ KGSL_PERFCOUNTER_NOT_USED, 0, A3XX_RBBM_PERFCTR_TSE_0_LO, 0 },
	{ KGSL_PERFCOUNTER_NOT_USED, 0, A3XX_RBBM_PERFCTR_TSE_1_LO, 0 },
};

static struct adreno_perfcount_register a3xx_perfcounters_ras[] = {
	{ KGSL_PERFCOUNTER_NOT_USED, 0, A3XX_RBBM_PERFCTR_RAS_0_LO, 0 },
	{ KGSL_PERFCOUNTER_NOT_USED, 0, A3XX_RBBM_PERFCTR_RAS_1_LO, 0 },
};

static struct adreno_perfcount_register a3xx_perfcounters_uche[] = {
	{ KGSL_PERFCOUNTER_NOT_USED, 0, A3XX_RBBM_PERFCTR_UCHE_0_LO, 0 },
	{ KGSL_PERFCOUNTER_NOT_USED, 0, A3XX_RBBM_PERFCTR_UCHE_1_LO, 0 },
	{ KGSL_PERFCOUNTER_NOT_USED, 0, A3XX_RBBM_PERFCTR_UCHE_2_LO, 0 },
	{ KGSL_PERFCOUNTER_NOT_USED, 0, A3XX_RBBM_PERFCTR_UCHE_3_LO, 0 },
	{ KGSL_PERFCOUNTER_NOT_USED, 0, A3XX_RBBM_PERFCTR_UCHE_4_LO, 0 },
	{ KGSL_PERFCOUNTER_NOT_USED, 0, A3XX_RBBM_PERFCTR_UCHE_5_LO, 0 },
};

static struct adreno_perfcount_register a3xx_perfcounters_tp[] = {
	{ KGSL_PERFCOUNTER_NOT_USED, 0, A3XX_RBBM_PERFCTR_TP_0_LO, 0 },
	{ KGSL_PERFCOUNTER_NOT_USED, 0, A3XX_RBBM_PERFCTR_TP_1_LO, 0 },
	{ KGSL_PERFCOUNTER_NOT_USED, 0, A3XX_RBBM_PERFCTR_TP_2_LO, 0 },
	{ KGSL_PERFCOUNTER_NOT_USED, 0, A3XX_RBBM_PERFCTR_TP_3_LO, 0 },
	{ KGSL_PERFCOUNTER_NOT_USED, 0, A3XX_RBBM_PERFCTR_TP_4_LO, 0 },
	{ KGSL_PERFCOUNTER_NOT_USED, 0, A3XX_RBBM_PERFCTR_TP_5_LO, 0 },
};

static struct adreno_perfcount_register a3xx_perfcounters_sp[] = {
	{ KGSL_PERFCOUNTER_NOT_USED, 0, A3XX_RBBM_PERFCTR_SP_0_LO, 0 },
	{ KGSL_PERFCOUNTER_NOT_USED, 0, A3XX_RBBM_PERFCTR_SP_1_LO, 0 },
	{ KGSL_PERFCOUNTER_NOT_USED, 0, A3XX_RBBM_PERFCTR_SP_2_LO, 0 },
	{ KGSL_PERFCOUNTER_NOT_USED, 0, A3XX_RBBM_PERFCTR_SP_3_LO, 0 },
	{ KGSL_PERFCOUNTER_NOT_USED, 0, A3XX_RBBM_PERFCTR_SP_4_LO, 0 },
	{ KGSL_PERFCOUNTER_NOT_USED, 0, A3XX_RBBM_PERFCTR_SP_5_LO, 0 },
	{ KGSL_PERFCOUNTER_NOT_USED, 0, A3XX_RBBM_PERFCTR_SP_6_LO, 0 },
	{ KGSL_PERFCOUNTER_NOT_USED, 0, A3XX_RBBM_PERFCTR_SP_7_LO, 0 },
};

static struct adreno_perfcount_register a3xx_perfcounters_rb[] = {
	{ KGSL_PERFCOUNTER_NOT_USED, 0, A3XX_RBBM_PERFCTR_RB_0_LO, 0 },
	{ KGSL_PERFCOUNTER_NOT_USED, 0, A3XX_RBBM_PERFCTR_RB_1_LO, 0 },
};

static struct adreno_perfcount_register a3xx_perfcounters_pwr[] = {
	{ KGSL_PERFCOUNTER_NOT_USED, 0, A3XX_RBBM_PERFCTR_PWR_0_LO, 0 },
	{ KGSL_PERFCOUNTER_NOT_USED, 0, A3XX_RBBM_PERFCTR_PWR_1_LO, 0 },
};

static struct adreno_perfcount_register a3xx_perfcounters_vbif[] = {
	{ KGSL_PERFCOUNTER_NOT_USED, 0, A3XX_VBIF_PERF_CNT0_LO },
	{ KGSL_PERFCOUNTER_NOT_USED, 0, A3XX_VBIF_PERF_CNT1_LO },
};
static struct adreno_perfcount_register a3xx_perfcounters_vbif_pwr[] = {
	{ KGSL_PERFCOUNTER_NOT_USED, 0, A3XX_VBIF_PERF_PWR_CNT0_LO },
	{ KGSL_PERFCOUNTER_NOT_USED, 0, A3XX_VBIF_PERF_PWR_CNT1_LO },
	{ KGSL_PERFCOUNTER_NOT_USED, 0, A3XX_VBIF_PERF_PWR_CNT2_LO },
};

static struct adreno_perfcount_group a3xx_perfcounter_groups[] = {
	{ a3xx_perfcounters_cp, ARRAY_SIZE(a3xx_perfcounters_cp) },
	{ a3xx_perfcounters_rbbm, ARRAY_SIZE(a3xx_perfcounters_rbbm) },
	{ a3xx_perfcounters_pc, ARRAY_SIZE(a3xx_perfcounters_pc) },
	{ a3xx_perfcounters_vfd, ARRAY_SIZE(a3xx_perfcounters_vfd) },
	{ a3xx_perfcounters_hlsq, ARRAY_SIZE(a3xx_perfcounters_hlsq) },
	{ a3xx_perfcounters_vpc, ARRAY_SIZE(a3xx_perfcounters_vpc) },
	{ a3xx_perfcounters_tse, ARRAY_SIZE(a3xx_perfcounters_tse) },
	{ a3xx_perfcounters_ras, ARRAY_SIZE(a3xx_perfcounters_ras) },
	{ a3xx_perfcounters_uche, ARRAY_SIZE(a3xx_perfcounters_uche) },
	{ a3xx_perfcounters_tp, ARRAY_SIZE(a3xx_perfcounters_tp) },
	{ a3xx_perfcounters_sp, ARRAY_SIZE(a3xx_perfcounters_sp) },
	{ a3xx_perfcounters_rb, ARRAY_SIZE(a3xx_perfcounters_rb) },
	{ a3xx_perfcounters_pwr, ARRAY_SIZE(a3xx_perfcounters_pwr) },
	{ a3xx_perfcounters_vbif, ARRAY_SIZE(a3xx_perfcounters_vbif) },
	{ a3xx_perfcounters_vbif_pwr, ARRAY_SIZE(a3xx_perfcounters_vbif_pwr) },
};

static struct adreno_perfcounters a3xx_perfcounters = {
	a3xx_perfcounter_groups,
	ARRAY_SIZE(a3xx_perfcounter_groups),
};

/* Defined in adreno_a3xx_snapshot.c */
void *a3xx_snapshot(struct adreno_device *adreno_dev, void *snapshot,
	int *remain, int hang);

struct adreno_gpudev adreno_a3xx_gpudev = {
	.reg_rbbm_status = A3XX_RBBM_STATUS,
	.reg_cp_pfp_ucode_addr = A3XX_CP_PFP_UCODE_ADDR,
	.reg_cp_pfp_ucode_data = A3XX_CP_PFP_UCODE_DATA,
	.perfcounters = &a3xx_perfcounters,

	.ctxt_create = a3xx_drawctxt_create,
	.ctxt_save = a3xx_drawctxt_save,
	.ctxt_restore = a3xx_drawctxt_restore,
	.ctxt_draw_workaround = NULL,
	.rb_init = a3xx_rb_init,
	.perfcounter_init = a3xx_perfcounter_init,
	.irq_control = a3xx_irq_control,
	.irq_handler = a3xx_irq_handler,
	.irq_pending = a3xx_irq_pending,
	.busy_cycles = a3xx_busy_cycles,
	.start = a3xx_start,
	.snapshot = a3xx_snapshot,
	.perfcounter_enable = a3xx_perfcounter_enable,
	.perfcounter_read = a3xx_perfcounter_read,
};

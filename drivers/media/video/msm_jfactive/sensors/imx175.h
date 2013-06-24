/* Copyright (c) 2011, Code Aurora Forum. All rights reserved.        
 *                                                                    
 * This program is free software; you can redistribute it and/or modif
 * it under the terms of the GNU General Public License version 2 and 
 * only version 2 as published by the Free Software Foundation.       
 *                                                                    
 * This program is distributed in the hope that it will be useful,    
 * but WITHOUT ANY WARRANTY; without even the implied warranty of     
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the      
 * GNU General Public License for more details.                       
 *                                                                    
 */                                                                   
                                                                      
#include <mach/board.h>                                               
extern struct platform_driver imx175_driver;                          
                                                                      
static struct msm_camera_i2c_reg_conf imx175_start_settings[] = {     
	{0x0100, 0x01},                                                      
};                                                                    
                                                                      
static struct msm_camera_i2c_reg_conf imx175_stop_settings[] = {      
	{0x0100, 0x00},                                                      
};                                                                    
                                                                      
static struct msm_camera_i2c_reg_conf imx175_groupon_settings[] = {   
	{0x104, 0x01},                                                       
};                                                                    
                                                                      
static struct msm_camera_i2c_reg_conf imx175_groupoff_settings[] = {  
	{0x104, 0x00},                                                       
};                                                                    
                                                                      
static struct msm_camera_i2c_reg_conf imx175_snap_settings[] = {      


{0x0100, 0x00},

{0x0103, 0x01},

{0x0101, 0x03}, /*Orientation V-F Flip*/

{0x0202, 0x09},/* shutter*/
{0x0203, 0xDC},/* shutter*/
{0x0301, 0x05},/* vt_pix_clk_div*/
{0x0303, 0x01},/* vt_sys_clk_div*/
{0x0305, 0x08},/* pre_pll_clk_di*/
{0x0309, 0x05},/* OPPXCK_DIV*/
{0x030B, 0x01},/* OPSYCK_DIV*/
{0x030C, 0x00},/* PLL_OP_MPY*/
{0x030D, 0xD8},/* PLL_OP_MPY*/
{0x0340, 0x09},/*frame_length_lines [15:8]*/
{0x0341, 0xBE},/*frame_length_lines [7:0]*/
{0x0342, 0x0D},/*line_length_pck [15:8]*/
{0x0343, 0x54},/*line_length_pck [7:0]*/
{0x0344, 0x00},
{0x0345, 0x06},
{0x0346, 0x00},
{0x0347, 0x06},
{0x0348, 0x0C},
{0x0349, 0xC9},
{0x034A, 0x09},
{0x034B, 0x9B},
{0x034C, 0x0C},
{0x034D, 0xC4},
{0x034E, 0x09},
{0x034F, 0x96},
{0x0390, 0x00},
{0x3020, 0x10},
{0x302D, 0x03},
{0x302F, 0x80},
{0x3032, 0xA3},
{0x3033, 0x20},
{0x3034, 0x24},

{0x3036, 0x00}, /*AD Boost*/

{0x3041, 0x15},
{0x3042, 0x87},
{0x3050, 0x35},
{0x3056, 0x57},
{0x305D, 0x41},
{0x3097, 0x69},
{0x3109, 0x41},
{0x3148, 0x3F},
{0x330F, 0x07},
{0x3344, 0x57},
{0x3345, 0x1F},
{0x3364, 0x00},
{0x3368, 0x18},
{0x3369, 0x00},
{0x3370, 0x77},
{0x3371, 0x2F},
{0x3372, 0x4F},
{0x3373, 0x2F},
{0x3374, 0x2F},
{0x3375, 0x37},
{0x3376, 0x9F},
{0x3377, 0x37},
{0x33C8, 0x00},
{0x33D4, 0x0C},
{0x33D5, 0xC4},
{0x33D6, 0x09},
{0x33D7, 0x96},
{0x4100, 0x06},
{0x4104, 0x32},
{0x4105, 0x32},
{0x4108, 0x01},
{0x4109, 0x7C},
{0x410A, 0x00},
{0x410B, 0x00},
{0x3302, 0x01},
{0x0100, 0x01},
};                                                                    
                                                                      
static struct msm_camera_i2c_reg_conf imx175_prev_settings[] = {      


{0x0100, 0x00},

{0x0103, 0x01},

{0x0101, 0x03}, /*Orientation V-F Flip*/

{0x0202, 0x09},/* shutter*/
{0x0203, 0xDC},/* shutter*/
{0x0301, 0x05},/* vt_pix_clk_div*/
{0x0303, 0x01},/* vt_sys_clk_div*/
{0x0305, 0x08},/* pre_pll_clk_di*/
{0x0309, 0x05},/* OPPXCK_DIV*/
{0x030B, 0x01},/* OPSYCK_DIV*/
{0x030C, 0x00},/* PLL_OP_MPY*/
{0x030D, 0xD8},/* PLL_OP_MPY*/
{0x0340, 0x09},/*frame_length_lines [15:8]*/
{0x0341, 0xBE},/*frame_length_lines [7:0]*/
{0x0342, 0x0D},/*line_length_pck [15:8]*/
{0x0343, 0x54},/*line_length_pck [7:0]*/
{0x0344, 0x00},
{0x0345, 0x06},
{0x0346, 0x00},
{0x0347, 0x06},
{0x0348, 0x0C},
{0x0349, 0xC9},
{0x034A, 0x09},
{0x034B, 0x9B},
{0x034C, 0x0C},
{0x034D, 0xC4},
{0x034E, 0x09},
{0x034F, 0x96},
{0x0390, 0x00},
{0x3020, 0x10},
{0x302D, 0x03},
{0x302F, 0x80},
{0x3032, 0xA3},
{0x3033, 0x20},
{0x3034, 0x24},

{0x3036, 0x00}, /*AD Boost*/

{0x3041, 0x15},
{0x3042, 0x87},
{0x3050, 0x35},
{0x3056, 0x57},
{0x305D, 0x41},
{0x3097, 0x69},
{0x3109, 0x41},
{0x3148, 0x3F},
{0x330F, 0x07},
{0x3344, 0x57},
{0x3345, 0x1F},
{0x3364, 0x00},
{0x3368, 0x18},
{0x3369, 0x00},
{0x3370, 0x77},
{0x3371, 0x2F},
{0x3372, 0x4F},
{0x3373, 0x2F},
{0x3374, 0x2F},
{0x3375, 0x37},
{0x3376, 0x9F},
{0x3377, 0x37},
{0x33C8, 0x00},
{0x33D4, 0x0C},
{0x33D5, 0xC4},
{0x33D6, 0x09},
{0x33D7, 0x96},
{0x4100, 0x06},
{0x4104, 0x32},
{0x4105, 0x32},
{0x4108, 0x01},
{0x4109, 0x7C},
{0x410A, 0x00},
{0x410B, 0x00},
{0x3302, 0x01},
{0x0100, 0x01},                                            
};                                                                    
                                                                      
static struct msm_camera_i2c_reg_conf imx175_recommend_settings[] = { 
};                                                                    

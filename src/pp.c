/*
 * Copyright (c) 2015 savoca <savoca@codefi.re>
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include "kcal.h"

int write_pp(struct msmfb_mdp_pp *pp)
{
    int ret = 0;
    int fd;

    fd = open(TARGET_FB, O_WRONLY);
    if (fd < 0) {
        printf("Failed to open %s\n", TARGET_FB);
        return 1;
    }

    ret = ioctl(fd, MSMFB_MDP_PP, pp);

    close(fd);

    if (ret != 0)
        printf("Failed to apply post-processing: %d\n", ret);

    return ret;
}

int write_pcc(int red, int green, int blue, int invert)
{
	struct msmfb_mdp_pp pp;

	memset(&pp, 0, sizeof(struct msmfb_mdp_pp));

	pp.op = mdp_op_pcc_cfg;
	pp.data.pcc_cfg_data.block = MDP_LOGICAL_BLOCK_DISP_0;
	pp.data.pcc_cfg_data.ops = MDP_PP_OPS_WRITE | MDP_PP_OPS_ENABLE;
	pp.data.pcc_cfg_data.r.r = red * 128;
	pp.data.pcc_cfg_data.g.g = green * 128;
	pp.data.pcc_cfg_data.b.b = blue * 128;

	if (invert) {
		pp.data.pcc_cfg_data.r.c = pp.data.pcc_cfg_data.g.c =
			pp.data.pcc_cfg_data.b.c = 0x7ff8;
		pp.data.pcc_cfg_data.r.r |= (0xffff << 16);
		pp.data.pcc_cfg_data.g.g |= (0xffff << 16);
		pp.data.pcc_cfg_data.b.b |= (0xffff << 16);
	}

	write_pp(&pp);

	return 0;
}

int write_pa(int hue, int sat, int val, int cont)
{
	struct msmfb_mdp_pp pp;

	memset(&pp, 0, sizeof(struct msmfb_mdp_pp));

	pp.op = mdp_op_pa_cfg;
	pp.data.pa_cfg_data.block = MDP_LOGICAL_BLOCK_DISP_0;
	pp.data.pa_cfg_data.pa_data.flags = MDP_PP_OPS_WRITE | MDP_PP_OPS_ENABLE;
	pp.data.pa_cfg_data.pa_data.hue_adj = hue;
	pp.data.pa_cfg_data.pa_data.sat_adj = sat;
	pp.data.pa_cfg_data.pa_data.val_adj = val;
	pp.data.pa_cfg_data.pa_data.cont_adj = cont;

	write_pp(&pp);

	return 0;
}

int write_pa_v2(int hue, int sat, int val, int cont)
{
	struct msmfb_mdp_pp pp;

	memset(&pp, 0, sizeof(struct msmfb_mdp_pp));

	pp.op = mdp_op_pa_v2_cfg;
	pp.data.pa_v2_cfg_data.block = MDP_LOGICAL_BLOCK_DISP_0;
	pp.data.pa_v2_cfg_data.pa_v2_data.flags =
		MDP_PP_OPS_WRITE | MDP_PP_OPS_ENABLE;
	pp.data.pa_v2_cfg_data.pa_v2_data.flags |= MDP_PP_PA_HUE_ENABLE;
	pp.data.pa_v2_cfg_data.pa_v2_data.flags |= MDP_PP_PA_HUE_MASK;
	pp.data.pa_v2_cfg_data.pa_v2_data.flags |= MDP_PP_PA_SAT_ENABLE;
	pp.data.pa_v2_cfg_data.pa_v2_data.flags |= MDP_PP_PA_SAT_MASK;
	pp.data.pa_v2_cfg_data.pa_v2_data.flags |= MDP_PP_PA_VAL_ENABLE;
	pp.data.pa_v2_cfg_data.pa_v2_data.flags |= MDP_PP_PA_VAL_MASK;
	pp.data.pa_v2_cfg_data.pa_v2_data.flags |= MDP_PP_PA_CONT_ENABLE;
	pp.data.pa_v2_cfg_data.pa_v2_data.flags |= MDP_PP_PA_CONT_MASK;
	pp.data.pa_v2_cfg_data.pa_v2_data.global_hue_adj = hue;
	pp.data.pa_v2_cfg_data.pa_v2_data.global_sat_adj = sat;
	pp.data.pa_v2_cfg_data.pa_v2_data.global_val_adj = val;
	pp.data.pa_v2_cfg_data.pa_v2_data.global_cont_adj = cont;

	write_pp(&pp);

	return 0;
}

int write_lut(int red, int green, int blue)
{
	int i;
	int fd;
	int ret = 0;
	struct fb_cmap cmap;
	uint16_t lut_r[MDP_LUT_SIZE];
	uint16_t lut_g[MDP_LUT_SIZE];
	uint16_t lut_b[MDP_LUT_SIZE];

	for (i = 0; i < MDP_LUT_SIZE; i++) {
		lut_r[i] = (i * red) / (MDP_LUT_SIZE - 1);
		lut_g[i] = (i * green) / (MDP_LUT_SIZE - 1);
		lut_b[i] = (i * blue) / (MDP_LUT_SIZE - 1);
	}

	cmap.start = 0;
	cmap.len = MDP_LUT_SIZE;

	/* Qualcomm's R and G registers where accidently swapped in linux,
	 * and later fixed. Until OEMs begin to merge in the upstream fixes,
	 * we must swap our own R and G assignments to apply correct LUT.
	 */
	cmap.red = &lut_g[0];
	cmap.green = &lut_r[0];
	cmap.blue = &lut_b[0];

	fd = open(TARGET_FB, O_WRONLY);
	if (fd < 0) {
		printf("Failed to open %s\n", TARGET_FB);
		return 1;
	}

	ret = ioctl(fd, MSMFB_SET_LUT, &cmap);

	close(fd);

	if (ret != 0)
		printf("Failed to apply post-processing: %d\n", ret);

	return ret;
}

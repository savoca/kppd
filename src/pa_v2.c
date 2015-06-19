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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "kcal.h"
#include "msm_mdp.h"

int write_pa_v2(int hue, int sat, int val, int cont)
{
	int fd;
	struct msmfb_mdp_pp pp;

	memset(&pp, 0, sizeof(struct msmfb_mdp_pp));

	pp.op = mdp_op_pa_v2_cfg;
	pp.data.pa_v2_cfg_data.block = block;
	pp.data.pa_v2_cfg_data.pa_v2_data.flags = ops_enable;
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

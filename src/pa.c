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

int write_pa(int hue, int sat, int val, int cont)
{
	struct msmfb_mdp_pp pp;

	memset(&pp, 0, sizeof(struct msmfb_mdp_pp));

	pp.op = mdp_op_pa_cfg;
	pp.data.pa_cfg_data.block = block;
	pp.data.pa_cfg_data.pa_data.flags = ops_enable;
	pp.data.pa_cfg_data.pa_data.hue_adj = hue;
	pp.data.pa_cfg_data.pa_data.sat_adj = sat;
	pp.data.pa_cfg_data.pa_data.val_adj = val;
	pp.data.pa_cfg_data.pa_data.cont_adj = cont;

	write_pp(&pp);

	return 0;
}

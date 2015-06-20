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

int write_pcc(int red, int green, int blue)
{
	struct msmfb_mdp_pp pp;

	memset(&pp, 0, sizeof(struct msmfb_mdp_pp));

	pp.op = mdp_op_pcc_cfg;
	pp.data.pcc_cfg_data.block = block;
	pp.data.pcc_cfg_data.ops = ops_enable;
	pp.data.pcc_cfg_data.r.r = red * 128;
	pp.data.pcc_cfg_data.g.g = green * 128;
	pp.data.pcc_cfg_data.b.b = blue * 128;

	write_pp(&pp);

	return 0;
}

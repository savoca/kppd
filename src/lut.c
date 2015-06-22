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

	fd = open(TARGET_FB, O_RDWR);
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

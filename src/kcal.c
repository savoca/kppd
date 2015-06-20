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
#include <fcntl.h>
#include <sys/ioctl.h>
#include <stdbool.h>

#include "kcal.h"

int usage(char *self, bool extended)
{
	printf("%s: <target> <data...>\n", self);

	if (extended == true) {
		printf("target: pcc pa pa_v2 igc lut\n");
		printf("\n");
		printf("pcc: red, green, blue\n");
		printf("pa: hue, saturation, value, contrast\n");
		printf("pa_v2: hue, saturation, value, contrast\n");
		printf("igc: invert\n");
		printf("lut: red, green, blue\n");
		printf("\n");
		printf("Example: %s pcc 256 232 246\n", self);
		return 0;
	}

	return 1;
}

int write_pp(struct msmfb_mdp_pp *pp)
{
	int ret = 0;
	int fd;

	fd = open(TARGET_FB, O_RDWR);
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

int main(int argc, char **argv)
{
	int i;
	int data[MAX_KCAL_DATA];

	block = MDP_LOGICAL_BLOCK_DISP_0;
	ops_enable = MDP_PP_OPS_WRITE | MDP_PP_OPS_ENABLE;
	ops_disable = MDP_PP_OPS_WRITE | MDP_PP_OPS_DISABLE;

	if (argc == 1)
		return usage(argv[0], true);

	if (!strcmp(argv[1], "PCC") || !strcmp(argv[1], "pcc")) {
		if (argc != 5)
			return usage(argv[0], false);

		for (i = 0; i < 3; i++) {
			if (sscanf(argv[i + 2], "%d", &data[i]) != 1)
				return usage(argv[0], false);
		}

		write_pcc(data[0], data[1], data[2]);
	} else if (!strcmp(argv[1], "PA") || !strcmp(argv[1], "pa")) {
		if (argc != 6)
			return usage(argv[0], false);

		for (i = 0; i < 4; i++) {
			if (sscanf(argv[i + 2], "%d", &data[i]) != 1)
				return usage(argv[0], false);
		}

		write_pa(data[0], data[1], data[2], data[3]);
	} else if (!strcmp(argv[1], "PA_V2") || !strcmp(argv[1], "pa_v2")) {
		if (argc != 6)
			return usage(argv[0], false);

		for (i = 0; i < 4; i++) {
			if (sscanf(argv[i + 2], "%d", &data[i]) != 1)
				return usage(argv[0], false);
		}

		write_pa_v2(data[0], data[1], data[2], data[3]);
	} else if (!strcmp(argv[1], "IGC") || !strcmp(argv[1], "igc")) {
		if (argc != 3)
			return usage(argv[0], false);

		if (sscanf(argv[2], "%d", &data[0]) != 1)
			return usage(argv[0], false);

		write_igc(data[0]);
	} else if (!strcmp(argv[1], "LUT") || !strcmp(argv[1], "lut")) {
		if (argc != 5)
			return usage(argv[0], false);

		for (i = 0; i < 3; i++) {
			if (sscanf(argv[i + 2], "%d", &data[i]) != 1)
				return usage(argv[0], false);
		}

		write_lut(data[0], data[1], data[2]);
	} else
		return usage(argv[0], false);

	return 0;
}

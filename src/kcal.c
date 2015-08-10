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

int usage(char *self, bool extended)
{
	printf("Usage: %s <target> <data...>\n", self);

	if (extended == true) {
		printf("\n");
		printf("kcal - Configure post-processing settings of Qualcomm MDSS\n");
		printf("Copyright (c) 2015 savoca <savoca@codefi.re>\n");
		printf("\n");
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

int main(int argc, char **argv)
{
	int i;
	int data[MAX_KCAL_DATA];

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

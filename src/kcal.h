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
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/ioctl.h>

#include "msm_mdp.h"

#define MAX_KCAL_DATA 4
#define TARGET_FB "/dev/graphics/fb0"
#define IGC_LUT_ENTRIES 256
#define MDP_LUT_SIZE 256

int write_pp(struct msmfb_mdp_pp *pp);
int write_pcc(int red, int green, int blue);
int write_pa(int hue, int sat, int val, int cont);
int write_pa_v2(int hue, int sat, int val, int cont);
int write_igc(int invert);
int write_lut(int red, int green, int blue);

struct rgb_data {
	int red;
	int green;
	int blue;
};

struct pa_data {
	int hue;
	int sat;
	int val;
	int cont;
};

struct igc_data {
	int invert;
};

struct kcal_data {
	int mdp_ver;
	int pa_ver;
	struct rgb_data rgb;
	struct pa_data pa;
	struct igc_data igc;
};

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
	printf("Usage: %s <config>\n", self);

	if (extended == true) {
		printf("\n");
		printf("kppd - Configure post-processing settings of Qualcomm MDSS\n");
		printf("Copyright (c) 2015 savoca <savoca@codefi.re>\n");
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

void init_daemon(void)
{
	int i;
	pid_t pid;

	pid = fork();
	if (pid != 0)
		exit(pid < 0 ? EXIT_FAILURE : EXIT_SUCCESS);

	umask(0);

	if (setsid() < 0)
		exit(EXIT_FAILURE);

	if (chdir("/") < 0)
		exit(EXIT_FAILURE);

	for (i = sysconf(_SC_OPEN_MAX); i > 0; i--)
		close(i);
}

void load_defaults(struct kcal_data *kcal)
{
	kcal->mdp_ver = 5;
	kcal->pa_ver = 2;
	kcal->rgb.red = 256;
	kcal->rgb.green = 256;
	kcal->rgb.blue = 256;
	kcal->pa.hue = 0;
	kcal->pa.sat = 255;
	kcal->pa.val = 255;
	kcal->pa.cont = 255;
	kcal->igc.invert = 0;
}

int detect_change(struct kcal_data *kd1, struct kcal_data *kd2)
{
	if (kd1->mdp_ver != kd2->mdp_ver || kd1->pa_ver != kd2->pa_ver ||
		kd1->rgb.red != kd2->rgb.red || kd1->rgb.green != kd2->rgb.green ||
		kd1->rgb.blue != kd2->rgb.blue || kd1->pa.hue != kd2->pa.hue ||
		kd1->pa.sat != kd2->pa.sat || kd1->pa.val != kd2->pa.val ||
		kd1->pa.cont != kd2->pa.cont || kd1->igc.invert != kd2->igc.invert)
		return 1;

	return 0;
}

void read_config(char *config, struct kcal_data *kcal_cfg)
{
	FILE *config_fd;

	config_fd = fopen(config, "r");
	if (config_fd == NULL)
		return;

	fscanf(config_fd, "[mdp_version]=%d\n", &kcal_cfg->mdp_ver);
	fscanf(config_fd, "[pa_version]=%d\n", &kcal_cfg->pa_ver);
	fscanf(config_fd, "[red]=%d\n", &kcal_cfg->rgb.red);
	fscanf(config_fd, "[green]=%d\n", &kcal_cfg->rgb.green);
	fscanf(config_fd, "[blue]=%d\n", &kcal_cfg->rgb.blue);
	fscanf(config_fd, "[hue]=%d\n", &kcal_cfg->pa.hue);
	fscanf(config_fd, "[saturation]=%d\n", &kcal_cfg->pa.sat);
	fscanf(config_fd, "[value]=%d\n", &kcal_cfg->pa.val);
	fscanf(config_fd, "[contrast]=%d\n", &kcal_cfg->pa.cont);
	fscanf(config_fd, "[invert]=%d\n", &kcal_cfg->igc.invert);

	fclose(config_fd);
}

void apply_kcal(struct kcal_data *kcal)
{
	if (kcal->mdp_ver == 5) {
		write_pcc(kcal->rgb.red, kcal->rgb.green, kcal->rgb.blue);

		if (kcal->pa_ver == 1) {
			write_pa(kcal->pa.hue, kcal->pa.sat,
				kcal->pa.val, kcal->pa.cont);
		} else if (kcal->pa_ver == 2) {
			write_pa_v2(kcal->pa.hue, kcal->pa.sat,
				kcal->pa.val, kcal->pa.cont);
		}

		write_igc(kcal->igc.invert);
	} else if (kcal->mdp_ver == 3)
		write_lut(kcal->rgb.red, kcal->rgb.green, kcal->rgb.blue);
}

int main(int argc, char **argv)
{
	int i;
	char config[MAX_CONFIG_PATH];
	char cwd[MAX_CONFIG_PATH];
	struct kcal_data kcal;
	struct kcal_data kcal_cfg;
	FILE *config_fd;

	block = MDP_LOGICAL_BLOCK_DISP_0;
	ops_enable = MDP_PP_OPS_WRITE | MDP_PP_OPS_ENABLE;
	ops_disable = MDP_PP_OPS_WRITE | MDP_PP_OPS_DISABLE;

	if (argc == 1)
		return usage(argv[0], true);

	if (argc != 2)
		return usage(argv[0], false);

	/* We will cd into root so grab the entire path of the config. */
	if (argv[1][0] != '/') {
		getcwd(cwd, sizeof(cwd));
		snprintf(config, sizeof(config), "%s/%s", cwd, argv[1]);
	} else
		snprintf(config, sizeof(config), "%s", argv[1]);

	if (access(config, R_OK) != 0) {
		printf("Could not read config!\n");
		return 1;
	}

	init_daemon();

	load_defaults(&kcal);

	for (;;) {
		read_config(config, &kcal_cfg);

		if (detect_change(&kcal, &kcal_cfg)) {
			kcal = kcal_cfg;
			apply_kcal(&kcal);
		}

		sleep(10);
	}

	exit(EXIT_SUCCESS);
}

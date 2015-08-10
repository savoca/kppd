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

#include <sys/inotify.h>
#include <limits.h>
#include "kcal.h"

#define KPPD_VER "1.1.0"
#define I_BUF_LEN (sizeof(struct inotify_event) + NAME_MAX + 1)

static int usage(char *self)
{
	printf("Usage: %s <config>\n", self);
	return 1;
}

static int version(void)
{
	printf("kppd version %s\n", KPPD_VER);
	printf("Copyright (c) 2015 savoca <savoca@codefi.re>\n");
	return 0;
}

static void init_daemon(void)
{
	int i;
	pid_t pid;

	pid = fork();
	if (pid)
		exit(pid < 0 ? EXIT_FAILURE : EXIT_SUCCESS);

	umask(0);

	if (setsid() < 0 || chdir("/") < 0)
		exit(EXIT_FAILURE);

	for (i = sysconf(_SC_OPEN_MAX); i > 0; i--)
		close(i);
}

static void read_config(char *config, struct kcal_data *kcal_cfg)
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

static void apply_kcal(struct kcal_data kcal)
{
	if (kcal.mdp_ver == 5) {
		write_pcc(kcal.rgb.red, kcal.rgb.green, kcal.rgb.blue);

		if (kcal.pa_ver == 1)
			write_pa(kcal.pa.hue, kcal.pa.sat, kcal.pa.val, kcal.pa.cont);
		else if (kcal.pa_ver == 2)
			write_pa_v2(kcal.pa.hue, kcal.pa.sat, kcal.pa.val, kcal.pa.cont);

		write_igc(kcal.igc.invert);
	} else if (kcal.mdp_ver == 3)
		write_lut(kcal.rgb.red, kcal.rgb.green, kcal.rgb.blue);
}

int main(int argc, char **argv)
{
	char *config;
	struct kcal_data kcal;
	struct inotify_event *event;
	int i_fd, i_wd, i_len;
	char i_buf[I_BUF_LEN];
	char i_path[PATH_MAX];
	char *p;

	if (argc != 2)
		return usage(argv[0]);

	if (!strcmp(argv[1], "--version") || !strcmp(argv[1], "-v"))
		return version();

	config = realpath(argv[1], NULL);
	if (access(config, R_OK)) {
		printf("Could not read config %s\n", config);
		return 1;
	}

	if (access(TARGET_FB, W_OK)) {
		printf("Could not open %s\n", TARGET_FB);
		return 1;
	}

	init_daemon();

	i_fd = inotify_init();
	if (i_fd < 0) {
		printf("Failed to initialize inotify\n");
		return 1;
	}

	i_wd = inotify_add_watch(i_fd, config,
		IN_MODIFY | IN_DELETE_SELF | IN_MOVE_SELF);
	if (i_wd < 0) {
		printf("Failed to add watch for %s\n", config);
		return 1;
	}

	read_config(config, &kcal);
	apply_kcal(kcal);

	for (;;) {
		i_len = read(i_fd, i_buf, I_BUF_LEN);
		if (i_len < 1) {
			printf("Failed to read from inotify fd.\n");
			return 1;
		}

		for (p = i_buf; p < i_buf + i_len;) {
			event = (struct inotify_event *)p;

			if (event->mask & IN_MODIFY) {
				read_config(config, &kcal);
				apply_kcal(kcal);
			}

			if (event->mask & IN_DELETE_SELF || event->mask & IN_MOVE_SELF)
				goto exit;

			p += sizeof(struct inotify_event) + event->len;
		}
	}

exit:
	inotify_rm_watch(i_fd, i_wd);
	close(i_fd);

	return 0;
}

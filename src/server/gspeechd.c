/* gspeechd.c
 *
 * Copyright (C) 2015 Brailcom, o.p.s.
 *
 * This file is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This file is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * Author: Andrei Kholodnyi <andrei.kholodnyi@gmail.com>
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <gio/gio.h>
#include <glib-unix.h>
#include <glib/gstdio.h>
#include <glib-object.h>

#include <signal.h>
#include <unistd.h>

#include "gspeechd-options.h"
#include "gspeechd-server.h"
#include "gspeechd-log.h"

static gboolean
on_interrupt (gpointer user_data)
{
	g_print ("%s\n", __FUNCTION__);
	return G_SOURCE_REMOVE;
}

static gboolean
load_configuration (gpointer user_data)
{
	g_print ("%s\n", __FUNCTION__);
	return G_SOURCE_CONTINUE;
}

static gboolean
pid_file_create (gchar *pidfile_name)
{
	FILE *pid_file;
	int pid_fd;
	struct flock lock;
	int ret;

	/* If the file exists, examine it's lock */
	pid_file = g_fopen (pidfile_name, "r");
	if (pid_file != NULL) {
		pid_fd = fileno (pid_file);

		lock.l_type = F_WRLCK;
		lock.l_whence = SEEK_SET;
		lock.l_start = 1;
		lock.l_len = 3;

		/* If there is a lock, exit, otherwise remove the old file */
		ret = fcntl (pid_fd, F_GETLK, &lock);
		if (ret == -1) {
			g_critical ("Can't check lock status of an existing pid file: %s.\n", pidfile_name);
			return FALSE;
		}

		fclose (pid_file);
		if (lock.l_type != F_UNLCK) {
			g_critical ("gspeechd already running.\n");
			return FALSE;
		}

		g_unlink (pidfile_name);
	}

	/* Create a new pid file and lock it */
	g_mkdir_with_parents (g_path_get_dirname(pidfile_name), S_IRWXU);

	pid_file = g_fopen (pidfile_name, "w");
	if (pid_file == NULL) {
		g_critical ("Can't create pid file in %s, wrong permissions?\n", pidfile_name);
		return FALSE;
	}
	g_fprintf (pid_file, "%d\n", getpid());
	fflush (pid_file);

	pid_fd = fileno (pid_file);
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = 1;
	lock.l_len = 3;

	ret = fcntl (pid_fd, F_SETLK, &lock);
	if (ret == -1) {
		g_critical ("Can't set lock on pid file: %s.\n", pidfile_name);
		return FALSE;
	}

	return TRUE;
}

static void
pid_file_remove (gchar *pidfile_name)
{
	gchar *pidfile_contents;

	if (g_file_get_contents (pidfile_name, &pidfile_contents, NULL, NULL)) {
		int pid = atoi (pidfile_contents);

		if (pid == getpid ()) {
			g_unlink (pidfile_name);
		}
		g_free (pidfile_contents);
	}
}

int main(int argc, char * argv[])
{
	const gspeechd_options *options;
	GSpeechdServer *server;
	GMainLoop *main_loop;
	gboolean parsed;
	GArray *server_parameters;

	parsed = gspeechd_options_parse (argc, argv);
	options = gspeechd_options_get ();

	gspeechd_log_init (options->log_level, options->log_dir);
	if (!parsed) {
		g_error ("speechd options not parsed\n");
	}

	if (options->mode == GSPEECHD_MODE_DAEMON) {
		if (daemon (0, 0)) {
			g_error ("Can't fork child process");
		}

		if (!pid_file_create (options->pid_file)) {
			exit (1);
		}
	}

	server_parameters = gspeechd_options_server_get ();
	server = gspeechd_server_new (server_parameters->len, (GParameter *) server_parameters->data);
	if (server == NULL) {
		exit (1);
	}
	if (!gspeechd_server_start (server)) {
		exit (1);
	}
	
	g_unix_signal_add(SIGINT, on_interrupt, NULL);
	g_unix_signal_add(SIGHUP, load_configuration, NULL);
	signal (SIGPIPE, SIG_IGN);

	main_loop = g_main_loop_new (NULL, FALSE);
	g_main_loop_run (main_loop);

	g_object_unref (server);
	g_main_loop_unref (main_loop);

	gspeechd_log_finalize ();

	return 0;
}

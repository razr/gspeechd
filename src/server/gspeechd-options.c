/* gspeechd-options.c
 *
 * Copyright (C) 2013 Brailcom, o.p.s.
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

#include <glib.h>
#include <glib/gstdio.h>

#include "gspeechd-options.h"

#define GSPEECHD_DEFAULT_OPTION_MODE GSPEECHD_MODE_DAEMON
#define GSPEECHD_DEFAULT_OPTION_COM_METHOD GSPEECHD_UNIX_SOCKET
#define GSPEECHD_DEFAULT_OPTION_PORT 6560
#define GSPEECHD_DEFAULT_OPTION_SOCKET_FILE "default"
#define GSPEECHD_DEFAULT_OPTION_PID_FILE "default"
#define GSPEECHD_DEFAULT_OPTION_LOG_LEVEL 1
#define GSPEECHD_DEFAULT_OPTION_LOG_DIR "/var/log/gspeechd"

static gspeechd_options cmd_options = 
{
	.mode = GSPEECHD_DEFAULT_OPTION_MODE,
	.method = GSPEECHD_DEFAULT_OPTION_COM_METHOD,
	.port = GSPEECHD_DEFAULT_OPTION_PORT,
	.socket_file = GSPEECHD_DEFAULT_OPTION_SOCKET_FILE,
	.pid_file = GSPEECHD_DEFAULT_OPTION_PID_FILE,
	.log_level = GSPEECHD_DEFAULT_OPTION_LOG_LEVEL,
	.log_dir = GSPEECHD_DEFAULT_OPTION_LOG_DIR
};

static gboolean
parse_com_method_cb (const gchar *option_name, const gchar *value,
		 gpointer data, GError **error)
{
	/* fallback to default if value is not correct */
	if (g_strcmp0 ("inet_socket", value) == 0) {
		cmd_options.method = GSPEECHD_INET_SOCKET;
	}
	return TRUE;
}

static GOptionEntry options[] =
{
	{ "run-single", 's', 0, G_OPTION_ARG_NONE, &cmd_options.mode, "Run as single application", NULL },
	{ "communication-method", 'c', 0, G_OPTION_ARG_CALLBACK, parse_com_method_cb, "Communication method to use ('unix_socket' or 'inet_socket')", NULL },
	{ "port", 		'p', 0, G_OPTION_ARG_INT, &cmd_options.port, "Local port to bind to", NULL },
	{ "socket-file", 'S', 0, G_OPTION_ARG_STRING, &cmd_options.socket_file, "Set path to socket file", NULL },
	{ "pid-file", 	'P', 0, G_OPTION_ARG_STRING, &cmd_options.pid_file, "Set path to pid file", NULL },
	{ "log-level",  'l', 0, G_OPTION_ARG_INT, &cmd_options.log_level, "Set log level (1...5)", NULL },
	{ "log-dir",	'L', 0, G_OPTION_ARG_STRING, &cmd_options.log_dir, "Set path to logging", NULL },
	{ NULL }
};

gboolean gspeechd_options_parse (int argc, char * argv[])
{
	GOptionContext *context;
	GError *error = NULL;
	gboolean parsed;

	context = g_option_context_new (" - common interface for speech synthesis");

	// g_option_context_set_summary (context, "gspeechd is a device independent layer for speech synthesis that provides a common easy to use interface for both client applications (programs that want to speak) and for software synthesizers (programs actually able to convert text to speech).");
	g_option_context_add_main_entries (context, options, argv[0]);

	parsed = g_option_context_parse (context, &argc, &argv, &error);
	g_option_context_free (context);

	/* check consistency now */
	if (parsed) {
		if (cmd_options.log_level < 1 || cmd_options.log_level > 5 ) {
			cmd_options.log_level = cmd_options.log_level;
		}
		if (g_strcmp0 (GSPEECHD_DEFAULT_OPTION_PID_FILE, cmd_options.pid_file) == 0) {
			cmd_options.pid_file = g_build_filename (g_get_user_runtime_dir(), "speech-dispatcher", "pid", "speechd.pid", NULL);
			/* make dir first, we'll create file later on */
			g_mkdir(g_path_get_dirname(cmd_options.pid_file), S_IRWXU);
		}
		if (g_strcmp0 (GSPEECHD_DEFAULT_OPTION_SOCKET_FILE, cmd_options.socket_file) == 0) {
			cmd_options.socket_file = g_build_filename (g_get_user_runtime_dir(), "speech-dispatcher", "speechd.sock", NULL);
		}
	}

	/* TRUE if the parsing was successful */
	return parsed;
}

const gspeechd_options * gspeechd_options_get (void)
{
	return (&cmd_options);
}


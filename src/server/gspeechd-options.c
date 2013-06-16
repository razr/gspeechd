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

#include "gspeechd-options.h"

#define GSPEECHD_DEFAULT_PORT 6560
#define GSPEECHD_DEFAULT_LOG_LEVEL 1
#define GSPEECHD_DEFAULT_LOG_DIR "/var/log/gspeechd"

static gspeechd_options cmd_options = 
{
	.port = GSPEECHD_DEFAULT_PORT,
	.log_level = GSPEECHD_DEFAULT_LOG_LEVEL,
	.log_dir = GSPEECHD_DEFAULT_LOG_DIR
};

static GOptionEntry options[] =
{
	{ "port", 		'p', 0, G_OPTION_ARG_INT, &cmd_options.port, "Local port to bind to", NULL },
	{ "log-level",  'l', 0, G_OPTION_ARG_INT, &cmd_options.log_level, "Set log level (1...5)", NULL },
	{ "log-dir",	'L', 0, G_OPTION_ARG_STRING, &cmd_options.log_dir, "Set path to logging", NULL },
	{ NULL }
};

const gspeechd_options * gspeechd_options_get (int argc, char * argv[])
{
	GOptionContext *context;
	GError *error = NULL;
	gboolean retval;

	context = g_option_context_new (" - common interface for speech synthesis");

	// g_option_context_set_summary (context, "gspeechd is a device independent layer for speech synthesis that provides a common easy to use interface for both client applications (programs that want to speak) and for software synthesizers (programs actually able to convert text to speech).");
	g_option_context_add_main_entries (context, options, argv[0]);

	retval = g_option_context_parse (context, &argc, &argv, &error);
	g_option_context_free (context);

	return (&cmd_options);
}


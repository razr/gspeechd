/* ssip.c
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
#include <glib/gprintf.h>

#include "ssip-enums.h"
#include "ssip.h"

struct _SsipCommand
{
	volatile gint   ref_count;

	SsipClientId	id;
	SsipCmd  		cmd;
	gchar			*param;
	gchar			*value;
};

SsipCommand *
ssip_command_new (gchar *line)
{
	SsipCommand *ssip_cmd;
	gchar **ps, *l_str;

	if (line == NULL)
		return NULL;

	/* parse command line */
	ps = g_strsplit (line, " ",  4);

	l_str = g_ascii_strdown (ps[0], -1);
	if (g_strcmp0 ("help", l_str) != 0) {
		
	}
	g_strfreev (ps);

	ssip_cmd = g_slice_new0 (SsipCommand);
	ssip_cmd->ref_count = 1;

	return ssip_cmd; 
}

static gchar*
parse_help(void)
{
    char *help;

    help = (char*) g_malloc(1024 * sizeof(char));

    sprintf(help, 
            C_OK_HELP"-  SPEAK           -- say text \r\n"
            C_OK_HELP"-  KEY             -- say a combination of keys \r\n"
            C_OK_HELP"-  CHAR            -- say a character \r\n"
            C_OK_HELP"-  SOUND_ICON      -- execute a sound icon \r\n"
            C_OK_HELP"-  SET             -- set a parameter \r\n"
            C_OK_HELP"-  GET             -- get a current parameter \r\n"
            C_OK_HELP"-  LIST            -- list available arguments \r\n"
            C_OK_HELP"-  HISTORY         -- commands related to history \r\n"
            C_OK_HELP"-  QUIT            -- close the connection \r\n"
            OK_HELP_SENT);

    return help;
}


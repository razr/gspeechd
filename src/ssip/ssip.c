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

#include "ssip.h"
#include "ssip-enum-types.h"

struct _SsipCommand
{
	volatile gint   ref_count;

	SsipClientId	id;
	SsipCmd  		cmd;
	gchar			*param;
	gchar			*value;
};

inline static SsipCmd
string_to_ssip_cmd (const gchar * cmd)
{
        GType type;
        GEnumClass *enum_class;
        GEnumValue *enum_value;

        type = ssip_cmd_get_type ();
        enum_class = G_ENUM_CLASS (g_type_class_peek (type));
        enum_value = g_enum_get_value_by_nick (enum_class, cmd);

        if (!enum_value) {
                return -1;
        }

        return enum_value->value;
}

static gchar*
parse_set (gchar** ps)
{
	return NULL;
}

static gchar*
parse_help (gchar** ps)
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

SsipCommand *
ssip_command_new (gchar *line)
{
	SsipCommand *ssip_cmd;
	gchar **ps, *s;
	SsipCmd cmd;

	if (line == NULL)
		return NULL;

	/* parse command line */
	ps = g_strsplit (line, " ",  4);

	s = g_ascii_strdown (ps[0], -1);

	cmd = string_to_ssip_cmd (s);
	if (cmd == -1)
		return NULL;

	/* TODO: generate it automatically */
	switch (cmd) {
		case SSIP_CMD_SET:
			parse_set (&ps[1]);
			break;
		case SSIP_CMD_HELP:
			parse_help (NULL);
			break;
		default:
			break;
	}

	g_free (s);
	g_strfreev (ps);

	ssip_cmd = g_slice_new0 (SsipCommand);
	ssip_cmd->ref_count = 1;

	return ssip_cmd; 
}

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
#include "ssip-status-enums.h"

struct _SsipCommand
{
	volatile gint   ref_count;

	SsipCmd  		cmd;
	SsipClientId	id;
	union {
        SsipSetParam set_param;
    } param;
	gchar			*value;
};

inline static SsipCmd
parse_cmd (const gchar * cmd, SsipStatus * status)
{
        GType type;
        GEnumClass *enum_class;
        GEnumValue *enum_value;

        type = ssip_cmd_get_type ();
        enum_class = G_ENUM_CLASS (g_type_class_peek (type));
        enum_value = g_enum_get_value_by_nick (enum_class, cmd);

        if (!enum_value) {
                *status = ERR_INVALID_COMMAND;
                return -1;
        }

        return enum_value->value;
}

inline static SsipClientId
parse_client_id (gchar * id, SsipStatus * status)
{
        GType type;
        GEnumClass *enum_class;
        GEnumValue *enum_value;

        type = ssip_client_id_get_type ();
        enum_class = G_ENUM_CLASS (g_type_class_peek (type));
        enum_value = g_enum_get_value_by_nick (enum_class, id);

        if (!enum_value) {
                *status = ERR_PARAMETER_INVALID;
                return -1;
        }

        return enum_value->value;
}

inline static SsipSetParam
parse_set (gchar * set_param, SsipStatus * status)
{
        GType type;
        GEnumClass *enum_class;
        GEnumValue *enum_value;

        type = ssip_set_param_get_type ();
        enum_class = G_ENUM_CLASS (g_type_class_peek (type));
        enum_value = g_enum_get_value_by_nick (enum_class, set_param);

        if (!enum_value) {
                *status = ERR_PARAMETER_INVALID;
                return -1;
        }

        return enum_value->value;
}

static gchar*
ssip_help_process ()
{
    gchar *help;

    help = g_malloc(1024 * sizeof(gchar));

    g_sprintf(help, 
            "-  SPEAK           -- say text \r\n"
            "-  KEY             -- say a combination of keys \r\n"
            "-  CHAR            -- say a character \r\n"
            "-  SOUND_ICON      -- execute a sound icon \r\n"
            "-  SET             -- set a parameter \r\n"
            "-  GET             -- get a current parameter \r\n"
            "-  LIST            -- list available arguments \r\n"
            "-  HISTORY         -- commands related to history \r\n"
            "-  QUIT            -- close the connection \r\n"
            "OK_HELP_SENT");

    return help;
}

SsipCommand *
ssip_command_new (gchar *line)
{
	SsipCommand *ssip_cmd;
	gchar **ps, *s;
	SsipCmd cmd;
	SsipStatus status;

	if (line == NULL)
		return NULL;

	/* split command line in 5 parts */
	ps = g_strsplit (line, " ",  4);
	s = g_ascii_strdown (ps[0], -1);

	cmd = parse_cmd (s, &status);
	g_free (s);
	/* TODO: check status instead */
	if (cmd == -1) {
		g_strfreev (ps);
		return NULL;
	}

	ssip_cmd = g_slice_new0 (SsipCommand);
	ssip_cmd->cmd = cmd;
	g_printf ("%d\n",ssip_cmd->cmd);
	switch (ssip_cmd->cmd) {
		case SSIP_CMD_SET:
			s = g_ascii_strdown (ps[1], -1);
			ssip_cmd->id = parse_client_id (s, &status);
	        g_free (s);

			s = g_ascii_strdown (ps[2], -1);
			ssip_cmd->param.set_param = parse_set (s, &status);
	        g_free (s);

			ssip_cmd->value = g_ascii_strdown (ps[3], -1);
			break;
		case SSIP_CMD_HELP:
			break;
		default:
			break;
	}

	g_strfreev (ps);

	ssip_cmd->ref_count = 1;

	return ssip_cmd;
}

inline SsipCmd
ssip_cmd_get (SsipCommand *ssip_cmd)
{
    return ssip_cmd->cmd;
}

inline SsipSetParam
ssip_set_param_get (SsipCommand *ssip_cmd)
{
    return ssip_cmd->param.set_param;
}

inline gchar *
ssip_set_param_value_get (SsipCommand *ssip_cmd)
{
    return ssip_cmd->value;
}

gchar * ssip_response (SsipStatus status)
{
        GType type;
        GEnumClass *enum_class;
        GEnumValue *enum_value;

        type = ssip_status_get_type ();
        enum_class = G_ENUM_CLASS (g_type_class_peek (type));
        enum_value = g_enum_get_value (enum_class, status);

        if (!enum_value) {
                return NULL;
        }

        return enum_value->value_name;
}

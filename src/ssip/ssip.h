/* ssip.h
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

#ifndef SSIP_H
#define SSIP_H

#include <glib-object.h>

G_BEGIN_DECLS

#define SSIP_COMMAND (ssip_command_get_type())

typedef struct _SsipCommand SsipCommand;

GType        ssip_command_get_type (void) G_GNUC_CONST;
SsipCommand *ssip_command_new (gchar *line);

/**
 * SsipCommands:
 * @SSIP_CMD_SPEAK: say text.
 * @SSIP_CMD_KEY: say a combination of keys.
 * @SSIP_CMD_CHAR: say a character.
 * @SSIP_CMD_SOUND_ICON: execute a sound icon.
 * @SSIP_CMD_SET: set a parameter.
 * @SSIP_CMD_GET: get a parameter.
 * @SSIP_CMD_LIST: list available arguments.
 * @SSIP_CMD_HISTORY: commands history.
 * @SSIP_CMD_QUIT: close the connection.
 * @SSIP_CMD_HELP: help.
 */


typedef enum {
	SSIP_LIST_PARAM_OUTPUT_MODULES,
	SSIP_LIST_PARAM_VOICES,
	SSIP_LIST_PARAM_SYNTHESIS_VOICES,
	SSIP_LIST_PARAM_AUDIO_MODULES
} SsipListParam;

typedef enum {
	SSIP_GET_PARAM_OUTPUT_MODULE,
	SSIP_LIST_PARAM_VOICE
} SsipGetParam;

typedef enum {
	SSIP_EVENT_BEGIN,
	SSIP_EVENT_END,
	SSIP_EVENT_CANCEL,
	SSIP_EVENT_PAUSE,
	SSIP_EVENT_RESUME,
	SSIP_EVENT_INDEX_MARK
} SsipEvent;

G_END_DECLS

#endif /* SSIP_H */

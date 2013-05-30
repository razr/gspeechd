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
 *
 * Flags used when adding a new directory to be indexed in the #TrackerIndexingTree.
 */
typedef enum {
	SSIP_CMD_SPEAK,
	SSIP_CMD_KEY,
	SSIP_CMD_CHAR,
	SSIP_CMD_SOUND_ICON,
	SSIP_CMD_SET,
	SSIP_CMD_GET,
	SSIP_CMD_LIST,
	SSIP_CMD_HISTORY,
	SSIP_CMD_QUIT,
	SSIP_CMD_HELP,

	SSIP_CMD_STOP,
	SSIP_CMD_CANCEL,
	SSIP_CMD_PAUSE,
	SSIP_CMD_RESUME,

	SSIP_CMD_NOTIFICATION,

	SSIP_CMD_DEBUG
} SsipCmd;


typedef enum {
	SSIP_CLIENT_ID_ID,
	SSIP_CLIENT_ID_ALL,
	SSIP_CLIENT_ID_SELF
} SsipClientId;


typedef enum {
	SSIP_SET_PARAM_CLIENT_NAME,
	SSIP_SET_PARAM_OUTPUT_MODULE,
	SSIP_SET_PARAM_LANGUAGE,
	SSIP_SET_PARAM_SSML_MODE,
	SSIP_SET_PARAM_PUNCTUATION,
	SSIP_SET_PARAM_SPELLING,
	SSIP_SET_PARAM_CAP_LET_RECOGN,
	SSIP_SET_PARAM_VOICE,
	SSIP_SET_PARAM_SYNTHESIS_VOICE,
	SSIP_SET_PARAM_RATE,
	SSIP_SET_PARAM_PITCH,
	SSIP_SET_PARAM_VOLUME,
	SSIP_SET_PARAM_PAUSE_CONTEXT,
	SSIP_SET_PARAM_HISTORY
} SsipSetParam;

typedef enum {
	SSIP_LIST_PARAM_OUTPUT_MOUDLES,
	SSIP_LIST_PARAM_VOICES,
	SSIP_LIST_PARAM_SYNTHESIS_VOICES
} SsipListParam;

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

/* ssip-enums.h
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

#ifndef SSIP_ENUMS_H
#define SSIP_ENUMS_H

G_BEGIN_DECLS

typedef enum {
	SSIP_CMD_SPEAK,
	SSIP_CMD_STOP,
	SSIP_CMD_CANCEL,
	SSIP_CMD_PAUSE,
	SSIP_CMD_RESUME,
	SSIP_CMD_KEY,
	SSIP_CMD_CHAR,
	SSIP_CMD_SOUND_ICON,
	SSIP_CMD_SET,
	SSIP_CMD_GET,
	SSIP_CMD_LIST,
	SSIP_CMD_HISTORY,
	SSIP_CMD_QUIT,
	SSIP_CMD_BYE,
	SSIP_CMD_HELP,
	SSIP_CMD_BLOCK,
} SsipCmd;

typedef enum {
	SSIP_CLIENT_ID_ID,
	SSIP_CLIENT_ID_ALL,
	SSIP_CLIENT_ID_SELF
} SsipClientId;

typedef enum {
	SSIP_SET_PARAM_CLIENT_NAME,
	SSIP_SET_PARAM_OUTPUT_MODULE,
	SSIP_SET_PARAM_AUDIO_MODULE,
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
	SSIP_SET_PARAM_HISTORY,
	SSIP_SET_PARAM_DEBUG,
	SSIP_SET_PARAM_NOTIFICATION,
} SsipSetParam;

G_END_DECLS

#endif /* SSIP_ENUMS_H */

/* gspeechd.h
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
 
#ifndef GSPEECHD_H
#define GSPEECHD_H

#define GSPEECHD_DEFAULT_OPTION_MODE GSPEECHD_MODE_DAEMON
#define GSPEECHD_DEFAULT_OPTION_PID_FILE "default"
#define GSPEECHD_DEFAULT_OPTION_LOG_LEVEL 1
#define GSPEECHD_DEFAULT_OPTION_LOG_DIR "/var/log/gspeechd"

/* Mode of gspeechd execution */
typedef enum {
	GSPEECHD_MODE_DAEMON,
	GSPEECHD_MODE_SINGLE
} gspeechd_mode;

#endif /* GSPEECHD_H */
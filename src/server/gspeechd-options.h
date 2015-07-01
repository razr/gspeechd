/* gspeechd-options.h
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

#ifndef GSPEECHD_OPTIONS_H
#define GSPEECHD_OPTIONS_H

#include <glib.h>

G_BEGIN_DECLS

typedef enum {
	GSPEECHD_UNIX_SOCKET,
	GSPEECHD_INET_SOCKET
} gspeechd_com_method;

/* Mode of speechd execution */
typedef enum {
	GSPEECHD_MODE_DAEMON,
	GSPEECHD_MODE_SINGLE
} gspeechd_mode;

typedef struct _gspeechd_options {
	gspeechd_mode mode;
	gspeechd_com_method method;
	gint	port;
	gchar  *socket_file;
	gchar  *pid_file;
	gint	log_level;
	gchar  *log_dir;
} gspeechd_options;

gboolean gspeechd_options_parse (int argc, char * argv[]);
const gspeechd_options * 
gspeechd_options_get (void);

G_END_DECLS

#endif /* GSPEECHD_OPTIONS_H */

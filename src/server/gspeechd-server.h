/* gspeechd-server.h
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

#ifndef GSPEECHD_SERVER_H
#define GSPEECHD_SERVER_H

#include <gio/gio.h>

G_BEGIN_DECLS

#define GSPEECHD_TYPE_SERVER            (gspeechd_server_get_type())
#define GSPEECHD_SERVER_TYPE_COM_METHOD (gspeechd_server_com_method_get_type ())

typedef struct _GSpeechdServer        GSpeechdServer;

typedef enum {
	GSPEECHD_SERVER_UNIX_SOCKET,
	GSPEECHD_SERVER_INET_SOCKET
} gspeechd_server_com_method;

GType gspeechd_server_get_type (void) G_GNUC_CONST;
GType gspeechd_server_com_method_get_type (void) G_GNUC_CONST;
GSpeechdServer *gspeechd_server_new              (guint n_parameters,
               									  GParameter *parameters);

G_END_DECLS

#endif /* GSPEECHD_SERVER_H */

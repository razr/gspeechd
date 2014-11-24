/* gspeechd-client.h
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

#ifndef GSPEECHD_CLIENT_H
#define GSPEECHD_CLIENT_H

#include <glib-object.h>
#include <gio/gio.h>

G_BEGIN_DECLS

#define GSPEECHD_TYPE_CLIENT (gspeechd_client_get_type())

typedef struct _GSpeechdClient GSpeechdClient;

GType           gspeechd_client_get_type (void) G_GNUC_CONST;
GSpeechdClient  *gspeechd_client_ref     (GSpeechdClient *client);
void            gspeechd_client_unref    (GSpeechdClient *client);
GSpeechdClient  *gspeechd_client_new     (GSocketConnection    *connection);
void            gspeechd_client_dispatch (GSpeechdClient *client);

void            gspeechd_client_fail     (GSpeechdClient *client);

G_END_DECLS

#endif /* GSPEECHD_CLIENT_H */


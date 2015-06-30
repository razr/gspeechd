/* gspeechd-server.h
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

#ifndef GSPEECHD_SERVER_H
#define GSPEECHD_SERVER_H

#include <gio/gio.h>
#include "gspeechd-options.h"

G_BEGIN_DECLS

#define GSPEECHD_TYPE_SERVER            (gspeechd_server_get_type())
#define GSPEECHD_SERVER(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GSPEECHD_TYPE_SERVER, GSpeechdServer))
#define GSPEECHD_SERVER_CONST(obj)      (G_TYPE_CHECK_INSTANCE_CAST ((obj), GSPEECHD_TYPE_SERVER, GSpeechdServer const))
#define GSPEECHD_SERVER_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass),  GSPEECHD_TYPE_SERVER, GSpeechdServerClass))
#define GSPEECHD_IS_SERVER(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GSPEECHD_TYPE_SERVER))
#define GSPEECHD_IS_SERVER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass),  GSPEECHD_TYPE_SERVER))
#define GSPEECHD_SERVER_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj),  GSPEECHD_TYPE_SERVER, GSpeechdServerClass))

typedef struct _GSpeechdServerPrivate GSpeechdServerPrivate;
typedef struct _GSpeechdServer        GSpeechdServer;
typedef struct _GSpeechdServerClass   GSpeechdServerClass;

struct _GSpeechdServer
{
   GThreadedSocketService parent;

   /*< private >*/
   GSpeechdServerPrivate *priv;
};

struct _GSpeechdServerClass
{
   GThreadedSocketServiceClass parent_class;

	/* signals */
	void (* client_added)   (GSpeechdServer *server,
	                         const gchar    *client_name);
	void (* client_removed) (GSpeechdServer *server,
	                         const gchar    *client_name);
};

GType    	    gspeechd_server_get_type         (void) G_GNUC_CONST;
GSpeechdServer *gspeechd_server_new              (gspeechd_com_method method);

G_END_DECLS

#endif /* GSPEECHD_SERVER_H */

/* gspeechd-server.c
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

#include "gspeechd-server.h"
#include "gspeechd-client-context.h"

G_DEFINE_TYPE(GSpeechdServer, gspeechd_server, G_TYPE_SOCKET_SERVICE)

struct _GSpeechdServerPrivate
{
   GHashTable *client_contexts;
};

enum
{
   CLIENT_ADDED,
   CLIENT_REMOVED,
   LAST_SIGNAL
};

static guint signals[LAST_SIGNAL];

GSpeechdServer *
gspeechd_server_new (int port)
{
	GError *error = NULL;

	GSpeechdServer *server = g_object_new (GSPEECHD_TYPE_SERVER, NULL);

	g_socket_listener_add_inet_port (G_SOCKET_LISTENER (server), port, NULL, &error);
	return server;
}

static gboolean
gspeechd_server_incoming (GSocketService    *service,
                          GSocketConnection *connection,
                          GObject           *source_object)
{
	GSpeechdServerPrivate *priv;
	GSpeechdClientContext *client;
	GSpeechdServer *server = (GSpeechdServer *)service;

	g_return_val_if_fail (GSPEECHD_IS_SERVER (server), FALSE);
	g_return_val_if_fail (G_IS_SOCKET_CONNECTION (connection), FALSE);

	g_printf ("%s\n", __FUNCTION__);

	priv = server->priv;

	/*
	 * Store the client context for tracking things
	 */
	client = gspeechd_client_context_new (connection);
	g_hash_table_insert (priv->client_contexts, connection, client);

	g_signal_emit (server, signals[CLIENT_ADDED], 0);

	return TRUE;
}

static void
gspeechd_server_finalize (GObject *object)
{
   GSpeechdServerPrivate *priv;

   priv = GSPEECHD_SERVER(object)->priv;

   g_hash_table_unref(priv->client_contexts);

   G_OBJECT_CLASS(gspeechd_server_parent_class)->finalize(object);

   return;
}

static void
gspeechd_server_class_init (GSpeechdServerClass *klass)
{
   GObjectClass *object_class;
   GSocketServiceClass *service_class;

   object_class = G_OBJECT_CLASS(klass);
   object_class->finalize = gspeechd_server_finalize;

	signals [CLIENT_ADDED] =
		g_signal_new ("client-added",
		              G_OBJECT_CLASS_TYPE (object_class),
		              G_SIGNAL_RUN_LAST,
		              G_STRUCT_OFFSET (GSpeechdServerClass, client_added),
		              NULL, NULL,
		              g_cclosure_marshal_VOID__STRING,
		              G_TYPE_NONE, 1,
		              G_TYPE_STRING);

	signals [CLIENT_ADDED] =
		g_signal_new ("client-removed",
		              G_OBJECT_CLASS_TYPE (object_class),
		              G_SIGNAL_RUN_LAST,
		              G_STRUCT_OFFSET (GSpeechdServerClass, client_removed),
		              NULL, NULL,
		              g_cclosure_marshal_VOID__STRING,
		              G_TYPE_NONE, 1,
		              G_TYPE_STRING);

   g_type_class_add_private(object_class, sizeof(GSpeechdServerPrivate));

   service_class = G_SOCKET_SERVICE_CLASS(klass);
   service_class->incoming = gspeechd_server_incoming;

}

static void
gspeechd_server_init (GSpeechdServer *server)
{
   server->priv = G_TYPE_INSTANCE_GET_PRIVATE(server,
                                              GSPEECHD_TYPE_SERVER,
                                              GSpeechdServerPrivate);
   server->priv->client_contexts =
      g_hash_table_new_full(g_direct_hash,
                            g_direct_equal,
                            NULL,
                            (GDestroyNotify)gspeechd_client_context_unref);
}


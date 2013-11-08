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
#include "ssip/ssip.h"

G_DEFINE_TYPE(GSpeechdServer, gspeechd_server, G_TYPE_SOCKET_SERVICE)

struct _GSpeechdServerPrivate
{
   GHashTable *client_contexts;
};

enum
{
   LAST_SIGNAL
};

static guint gSignals[LAST_SIGNAL];

static void
gspeechd_read_line_cb (GInputStream *stream,
                       GAsyncResult *res,
                       gpointer user_data);

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
	GInputStream *stream;
	GDataInputStream *data_stream;
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

	/*
	 * Start listening for the message
	 */
	stream = g_io_stream_get_input_stream(G_IO_STREAM(connection));
	data_stream = g_data_input_stream_new (stream);

	g_data_input_stream_read_line_async(data_stream,
							G_PRIORITY_DEFAULT,
							client->cancellable,
							(GAsyncReadyCallback)gspeechd_read_line_cb,
							gspeechd_client_context_ref (client));

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

static void
gspeechd_read_line_cb (GInputStream *stream,
                       GAsyncResult *res,
                       gpointer user_data)
{
	GSpeechdClientContext *context = user_data;
	char *s;
	gsize len;
	GError *error = NULL;
	SsipCommand *cmd;
	s = g_data_input_stream_read_line_finish (G_DATA_INPUT_STREAM(stream),
	                                          res,
                                              &len,
	                                          &error);
	g_printf ("%s\n",s);

	/* parse SSIP command */
	cmd = ssip_command_new (s);
	g_free(s);

	/* process SSIP command */
}

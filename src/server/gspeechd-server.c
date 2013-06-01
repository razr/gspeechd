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

#include "gspeechd-server.h"
#include "ssip/ssip.h"

G_DEFINE_TYPE(GSpeechdServer, gspeechd_server, G_TYPE_SOCKET_SERVICE)

struct _GSpeechdServerPrivate
{
   GHashTable *client_contexts;
};

struct _GSpeechdClientContext
{
   volatile gint      ref_count;

   GCancellable      *cancellable;
   GSocketConnection *connection;
   GDataOutputStream *output;
   GSpeechdServer    *server;
   guint8            *incoming;
   gboolean           failed;
};

enum
{
   LAST_SIGNAL
};

static guint gSignals[LAST_SIGNAL];

static GSpeechdClientContext *
gspeechd_client_context_new (GSpeechdServer       *server,
                             GSocketConnection    *connection);
static void
gspeechd_client_context_dispatch (GSpeechdClientContext *client);

static void
gspeechd_client_context_fail (GSpeechdClientContext *client);

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
	client = gspeechd_client_context_new (server, connection);
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
	
	cmd = ssip_command_new (s);
	g_free(s);
}


static void
gspeechd_client_context_dispose (GSpeechdClientContext *context)
{
   g_clear_object (&context->cancellable);
   g_clear_object (&context->connection);
   g_clear_object (&context->output);

   if (context->server) {
      g_object_remove_weak_pointer (G_OBJECT(context->server),
                                    (gpointer *)&context->server);
   }
}

static GSpeechdClientContext *
gspeechd_client_context_new (GSpeechdServer       *server,
                             GSocketConnection *connection)
{
	GSpeechdClientContext *context;
	GOutputStream *output_stream;

	context = g_slice_new0 (GSpeechdClientContext);
	context->ref_count = 1;
	context->cancellable = g_cancellable_new ();
	context->server = server;
	context->connection = g_object_ref (connection);
	context->failed = FALSE;

	output_stream = g_io_stream_get_output_stream (G_IO_STREAM(connection));
	context->output = g_data_output_stream_new (output_stream);
	g_object_add_weak_pointer(G_OBJECT(context->server),
                              (gpointer *)&context->server);
   return (context);
}

static void
gspeechd_client_context_fail (GSpeechdClientContext *client)
{
   g_assert(client);

   client->failed = TRUE;
   g_io_stream_close(G_IO_STREAM(client->connection),
                     client->cancellable,
                     NULL);
}

/**
 * gspeechd_client_context_ref:
 * @context: A #GSpeechdClientContext.
 *
 * Atomically increments the reference count of @context by one.
 *
 * Returns: (transfer full): A reference to @context.
 */
GSpeechdClientContext *
gspeechd_client_context_ref (GSpeechdClientContext *context)
{
   g_return_val_if_fail(context != NULL, NULL);
   g_return_val_if_fail(context->ref_count > 0, NULL);

   g_atomic_int_inc(&context->ref_count);
   return (context);
}

/**
 * gspeechd_client_context_unref:
 * @context: A GSpeechdClientContext.
 *
 * Atomically decrements the reference count of @context by one.  When the
 * reference count reaches zero, the structure will be destroyed and
 * freed.
 */
void
gspeechd_client_context_unref (GSpeechdClientContext *context)
{
   g_return_if_fail (context != NULL);
   g_return_if_fail (context->ref_count > 0);

   if (g_atomic_int_dec_and_test (&context->ref_count)) {
      gspeechd_client_context_dispose (context);
      g_slice_free (GSpeechdClientContext, context);
   }
}

GType
gspeechd_client_context_get_type (void)
{
   static gsize initialized;
   static GType type_id;

   if (g_once_init_enter(&initialized)) {
      type_id = g_boxed_type_register_static (
            "GSpeechDClientContext",
            (GBoxedCopyFunc)gspeechd_client_context_ref,
            (GBoxedFreeFunc)gspeechd_client_context_unref);
      g_once_init_leave (&initialized, TRUE);
   }

   return type_id;
}

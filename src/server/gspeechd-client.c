/* gspeechd-client.c
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

#include <glib-object.h>
#include <gio/gio.h>

#include "gspeechd-client.h"
#include "ssip/ssip.h"

struct _GSpeechdClient
{
   volatile gint      ref_count;

   GCancellable      *cancellable;
   GSocketConnection *connection;
	GDataInputStream  *input;
   GDataOutputStream *output;

   guint8            *incoming;
   gboolean           failed;

	/* msg settings */
	/* msg mode: DATA, SSML */
	/* msg prio */
	/* msg type = TEXT, CHAR, ICON etc. */
	/* notification */

	/* status: speaking, paused, stoped */

	/* output modules: espeak, etc */
	/* audio module: alsa, pulse, etc */
	/* log level */
	/* history */
};

static void
gspeechd_client_read_line_cb (GInputStream *stream,
                              GAsyncResult *res,
                              gpointer user_data);

void
gspeechd_client_dispose (GSpeechdClient *client)
{
   g_clear_object (&client->cancellable);
   g_clear_object (&client->connection);
   g_clear_object (&client->input);
   g_clear_object (&client->output);
}

GSpeechdClient *
gspeechd_client_new (GSocketConnection *connection)
{
	GSpeechdClient *client;
	GOutputStream *output_stream;
	GInputStream *input_stream;

	client = g_slice_new0 (GSpeechdClient);
	client->ref_count = 1;
	client->cancellable = g_cancellable_new ();
	client->connection = g_object_ref (connection);
	client->failed = FALSE;

	/*
	 * Start listening for the message
	 */
	input_stream = g_io_stream_get_input_stream(G_IO_STREAM(connection));
	client->input = g_data_input_stream_new (input_stream);

	g_data_input_stream_read_line_async(client->input,
							G_PRIORITY_DEFAULT,
							client->cancellable,
							(GAsyncReadyCallback)gspeechd_client_read_line_cb,
							gspeechd_client_ref (client));

	output_stream = g_io_stream_get_output_stream (G_IO_STREAM(connection));
	client->output = g_data_output_stream_new (output_stream);

	return (client);
}

void
gspeechd_client_fail (GSpeechdClient *client)
{
   g_assert(client);

   client->failed = TRUE;
   g_io_stream_close(G_IO_STREAM(client->connection),
                     client->cancellable,
                     NULL);
}

/**
 * gspeechd_client_ref:
 * @client: A #GSpeechdClient.
 *
 * Automically increments the reference count of @client by one.
 *
 * Returns: (transfer full): A reference to @client.
 */
GSpeechdClient *
gspeechd_client_ref (GSpeechdClient *client)
{
   g_return_val_if_fail(client != NULL, NULL);
   g_return_val_if_fail(client->ref_count > 0, NULL);

   g_atomic_int_inc(&client->ref_count);
   return (client);
}

/**
 * gspeechd_client_unref:
 * @client: A GSpeechdClient.
 *
 * Automically decrements the reference count of @client by one.  When the
 * reference count reaches zero, the structure will be destroyed and
 * freed.
 */
void
gspeechd_client_unref (GSpeechdClient *client)
{
   g_return_if_fail (client != NULL);
   g_return_if_fail (client->ref_count > 0);

   if (g_atomic_int_dec_and_test (&client->ref_count)) {
      gspeechd_client_dispose (client);
      g_slice_free (GSpeechdClient, client);
   }
}

GType
gspeechd_client_get_type (void)
{
   static gsize initialized;
   static GType type_id;

   if (g_once_init_enter(&initialized)) {
      type_id = g_boxed_type_register_static (
            "GSpeechDClient",
            (GBoxedCopyFunc)gspeechd_client_ref,
            (GBoxedFreeFunc)gspeechd_client_unref);
      g_once_init_leave (&initialized, TRUE);
   }

   return type_id;
}

static void
gspeechd_client_read_line_cb (GInputStream *stream,
                       GAsyncResult *res,
                       gpointer user_data)
{
	GSpeechdClient *client = user_data;
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
	/* when QUIT received, send an event connection_closed */
}
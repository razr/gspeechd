/* gspeechd-client-context.c
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

#include "gspeechd-client-context.h"

struct _GSpeechdClientContext
{
   volatile gint      ref_count;

   GCancellable      *cancellable;
   GSocketConnection *connection;
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


void
gspeechd_client_context_dispose (GSpeechdClientContext *context)
{
   g_clear_object (&context->cancellable);
   g_clear_object (&context->connection);
   g_clear_object (&context->output);
}

GSpeechdClientContext *
gspeechd_client_context_new (GSocketConnection *connection)
{
	GSpeechdClientContext *context;
	GOutputStream *output_stream;

	context = g_slice_new0 (GSpeechdClientContext);
	context->ref_count = 1;
	context->cancellable = g_cancellable_new ();
	context->connection = g_object_ref (connection);
	context->failed = FALSE;

	output_stream = g_io_stream_get_output_stream (G_IO_STREAM(connection));
	context->output = g_data_output_stream_new (output_stream);

	return (context);
}

void
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

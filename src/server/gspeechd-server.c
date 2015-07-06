/* gspeechd-server.c
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <glib.h>
#include <gio/gio.h>
#include <gio/gunixsocketaddress.h>
#include <glib/gstdio.h>

#include "gspeechd-server.h"
#include "gspeechd-client.h"

#define GSPEECHD_SERVER_DEFAULT_COM_METHOD GSPEECHD_SERVER_UNIX_SOCKET
#define GSPEECHD_SERVER_DEFAULT_PORT 6560
#define GSPEECHD_SERVER_DEFAULT_SOCKET_FILE "default"

#define GSPEECHD_SERVER(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GSPEECHD_TYPE_SERVER, GSpeechdServer))
#define GSPEECHD_SERVER_CONST(obj)      (G_TYPE_CHECK_INSTANCE_CAST ((obj), GSPEECHD_TYPE_SERVER, GSpeechdServer const))
#define GSPEECHD_SERVER_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass),  GSPEECHD_TYPE_SERVER, GSpeechdServerClass))
#define GSPEECHD_IS_SERVER(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GSPEECHD_TYPE_SERVER))
#define GSPEECHD_IS_SERVER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass),  GSPEECHD_TYPE_SERVER))
#define GSPEECHD_SERVER_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj),  GSPEECHD_TYPE_SERVER, GSpeechdServerClass))

typedef struct _GSpeechdServerPrivate GSpeechdServerPrivate;
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

G_DEFINE_TYPE(GSpeechdServer, gspeechd_server, G_TYPE_SOCKET_SERVICE)

struct _GSpeechdServerPrivate
{
   gspeechd_server_com_method method;
   guint16 port;
   gchar *socket_file;

   GHashTable *clients;
};

enum
{
   CLIENT_ADDED,
   CLIENT_REMOVED,
   LAST_SIGNAL
};
static guint signals[LAST_SIGNAL];

enum
{
  PROP_0,

  PROP_COMMUNICATION_METHOD,
  PROP_PORT,
  PROP_SOCKET_FILE,

  N_PROPERTIES
};
static GParamSpec *obj_properties[N_PROPERTIES] = { NULL, };

GType
gspeechd_server_com_method_get_type (void)
{
  static GType server_com_method_type = 0;

  if (!server_com_method_type) {
    static GEnumValue com_types[] = {
      { GSPEECHD_SERVER_UNIX_SOCKET, "UNIX Socket", "unix-socket" },
      { GSPEECHD_SERVER_INET_SOCKET, "INET Socket", "inet-socket"  },
      { 0, NULL, NULL },
    };

    server_com_method_type =
	g_enum_register_static ("GspeechdServerComMethod",
				com_types);
  }

  return server_com_method_type;
}

static void
gspeechd_get_property (GObject    *object,
                       guint       property_id,
                       GValue     *value,
                       GParamSpec *pspec)
{
  GSpeechdServer *self = GSPEECHD_SERVER (object);

  switch (property_id)
    {
    case PROP_COMMUNICATION_METHOD:
      g_value_set_enum (value, self->priv->method);
      break;

    case PROP_PORT:
      g_value_set_int (value, self->priv->port);
      break;

    case PROP_SOCKET_FILE:
      g_value_set_string (value, self->priv->socket_file);
      break;

    default:
      /* We don't have any other property... */
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void
gspeechd_set_property (GObject      *object,
                       guint         property_id,
                       const GValue *value,
                       GParamSpec   *pspec)
{
  GSpeechdServer *self = GSPEECHD_SERVER (object);

  switch (property_id)
    {
    case PROP_COMMUNICATION_METHOD:
      self->priv->method = g_value_get_enum (value);
      break;

    case PROP_PORT:
      self->priv->port = g_value_get_int (value);
      break;

    case PROP_SOCKET_FILE:
      g_free (self->priv->socket_file);
      self->priv->socket_file = g_value_dup_string (value);
      break;

    default:
      /* We don't have any other property... */
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

GSpeechdServer *
gspeechd_server_new (guint n_parameters, GParameter *parameters)
{
	GError *error = NULL;

	GSpeechdServer *server = g_object_newv (GSPEECHD_TYPE_SERVER, n_parameters, parameters);

	if (g_strcmp0 (GSPEECHD_SERVER_DEFAULT_SOCKET_FILE, server->priv->socket_file) == 0) {
	    g_free (server->priv->socket_file);
		server->priv->socket_file = g_build_filename (g_get_user_runtime_dir(), "speech-dispatcher", "speechd.sock", NULL);
	}

	if (server->priv->method == GSPEECHD_SERVER_INET_SOCKET) {
		if (!g_socket_listener_add_inet_port (G_SOCKET_LISTENER (server), server->priv->port, NULL, &error)) {
		    g_error ("Cannot add listener on port %d: %s", server->priv->port, error->message);
		}
		g_message ("server is listening on port %d\n", server->priv->port);
	}
	else {
		GSocket *socket;
		GSocketAddress *address;

		socket = g_socket_new (G_SOCKET_FAMILY_UNIX, G_SOCKET_TYPE_STREAM,
							   G_SOCKET_PROTOCOL_DEFAULT, &error);
 		if (!socket)
        	g_error ("Cannot create socket: %s", error->message);

		if (g_file_test (server->priv->socket_file, G_FILE_TEST_EXISTS)) {
			g_unlink (server->priv->socket_file);
		}
		address = g_unix_socket_address_new (server->priv->socket_file);

	    if (!g_socket_bind (socket, address, TRUE, &error))
    	    g_error ("Cannot bind socket: %s", error->message);

	    if (!g_socket_listen (socket, &error))
    	    g_error ("Cannot listen in socket: %s", error->message);

		g_socket_listener_add_socket (G_SOCKET_LISTENER (server), socket, NULL, &error);
		g_message ("server is listening on socket %s\n", server->priv->socket_file);

    	if (socket)
        	g_object_unref (socket);
    	if (address)
        	g_object_unref (address);
	}

	return server;
}

static gboolean
gspeechd_server_incoming (GSocketService    *service,
                          GSocketConnection *connection,
                          GObject           *source_object)
{
	GSpeechdServerPrivate *priv;
	GSpeechdClient *client;
	GSpeechdServer *server = (GSpeechdServer *)service;

	g_return_val_if_fail (GSPEECHD_IS_SERVER (server), FALSE);
	g_return_val_if_fail (G_IS_SOCKET_CONNECTION (connection), FALSE);
	priv = server->priv;

	/* Store the client for tracking things */
	client = gspeechd_client_new (connection);
	g_hash_table_insert (priv->clients, connection, client);

	/* TODO emit signal on client name change */
	g_signal_emit (server, signals[CLIENT_ADDED], 0);

	return TRUE;
}

static void
gspeechd_server_finalize (GObject *object)
{
    GSpeechdServerPrivate *priv;

    priv = GSPEECHD_SERVER(object)->priv;

    g_hash_table_unref(priv->clients);
    g_free (priv->socket_file);

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

	object_class->set_property = gspeechd_set_property;
	object_class->get_property = gspeechd_get_property;

	obj_properties[PROP_COMMUNICATION_METHOD] =
	g_param_spec_enum ("communication-method",
                       "communication method property",
                       "get/set server communication method",
                       GSPEECHD_SERVER_TYPE_COM_METHOD,
                       GSPEECHD_SERVER_DEFAULT_COM_METHOD,
                       G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);

	obj_properties[PROP_PORT] =
    g_param_spec_int ("port",
                      "communication port number property",
                      "get/set communication port method",
                      1024  /* minimum value */,
                      65535 /* maximum value */,
                      GSPEECHD_SERVER_DEFAULT_PORT,
                      G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);

	obj_properties[PROP_SOCKET_FILE] =
	g_param_spec_string ("socket-file",
                         "socket file property",
                         "socket file for the UNIX socket",
                         GSPEECHD_SERVER_DEFAULT_SOCKET_FILE,
                         G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);

	g_object_class_install_properties (object_class,
                                       N_PROPERTIES,
                                       obj_properties);

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
   server->priv->clients =
      g_hash_table_new_full(g_direct_hash,
                            g_direct_equal,
                            NULL,
                            (GDestroyNotify)gspeechd_client_unref);
}

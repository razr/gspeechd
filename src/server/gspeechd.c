#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <gio/gio.h>
#include <gio/gnio.h>

#include "options.h"

int main(int argc, char * argv[])
{
	GOptionContext *context;
	GSocketService *service;

	service = g_threaded_socket_service_new (max_threads);

	if (!g_socket_listener_add_inet_port (G_SOCKET_LISTENER (service),
	                                      port, NULL, &error))
	{
		g_printerr ("%s: %s\n", argv[0], error->message);
		return 1;
	}

	g_print ("speechd listening on port %d\n", port);

	g_signal_connect (service, "run", G_CALLBACK (handler), NULL);

	g_main_loop_run (g_main_loop_new (NULL, FALSE));
	g_assert_not_reached ();
}

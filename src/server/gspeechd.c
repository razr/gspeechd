#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <gio/gio.h>
#include <glib-unix.h>

#include <signal.h>

#include "gspeechd-options.h"
#include "gspeechd-server.h"
#include "gspeechd-log.h"

static gboolean
on_interrupt (gpointer user_data)
{
	g_print ("%s\n", __FUNCTION__);
	return G_SOURCE_REMOVE;
}

static gboolean
load_configuration (gpointer user_data)
{
	g_print ("%s\n", __FUNCTION__);
	return G_SOURCE_CONTINUE;
}

int main(int argc, char * argv[])
{
	const gspeechd_options *options;
	GSpeechdServer *server;
	GMainLoop *main_loop;
	gboolean ret;

	ret = gspeechd_options_parse (argc, argv);
	if (ret) {
		g_print ("speechd options parsed\n");
	}
	options = gspeechd_options_get ();

	gspeechd_log_init (options->log_level, options->log_dir);

	server = gspeechd_server_new (options->method);

	g_print ("speechd started in %s mode\n", 
		(options->method == GSPEECHD_UNIX_SOCKET)?"unix_socket":"inet_socket");

	g_unix_signal_add(SIGINT, on_interrupt, NULL);
	g_unix_signal_add(SIGHUP, load_configuration, NULL);
	signal (SIGPIPE, SIG_IGN);

	main_loop = g_main_loop_new (NULL, FALSE);
	g_main_loop_run (main_loop);

	g_object_unref (server);
	g_main_loop_unref (main_loop);

	gspeechd_log_finalize ();

	return 0;
}

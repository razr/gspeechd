#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <gio/gio.h>

#include "gspeechd-options.h"
#include "gspeechd-server.h"
#include "gspeechd-log.h"

int main(int argc, char * argv[])
{
	const gspeechd_options *options;
	GOptionContext *context;
	GSpeechdServer *server;
	GMainLoop *main_loop;

	options = gspeechd_options_get (argc, argv);

	gspeechd_log_init (options->log_level, options->log_dir);

	server = gspeechd_server_new (options->port);

	g_print ("speechd listening on port %d\n", options->port);

	main_loop = g_main_loop_new (NULL, FALSE);
	g_main_loop_run (main_loop);

	g_object_unref (server);
	g_main_loop_unref (main_loop);

	gspeechd_log_finalize ();

	return 0;
}

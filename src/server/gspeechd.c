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

	main_loop = g_main_loop_new (NULL, FALSE);
	g_main_loop_run (main_loop);

	g_object_unref (server);
	g_main_loop_unref (main_loop);

	gspeechd_log_finalize ();

	return 0;
}

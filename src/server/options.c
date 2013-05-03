#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <glib.h>

#include "options.h"

#define SPEECHD_DEFAULT_PORT 6560

struct options {
	gint port;
};

static struct options cmd_options = {.port = SPEECHD_DEFAULT_PORT};

static GOptionEntry options[] =
{
	{ "port",     'p', 0, G_OPTION_ARG_INT, &cmd_options.port, "Local port to bind to", NULL },
	{ NULL }
};

#ifndef __OPTIONS_H_
#define __OPTIONS_H_

#include <glib.h>

G_BEGIN_DECLS

struct options {
	gint port;
};

int
command_line (GApplication            *application,
              GApplicationCommandLine *cmdline);

G_END_DECLS

#endif /* __OPTIONS_H_ */

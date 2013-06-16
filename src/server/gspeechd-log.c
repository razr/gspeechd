/* gspeechd-log.c
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
#include <config.h>

#include <glib.h>

#include "gspeechd-log.h"

typedef struct _gspeechd_log_options {
	GMutex	    mutex;
	gboolean	initialized;

	gint		level;
	gchar	   *dirname;
} gspeechd_log_options;

static gspeechd_log_options log_options = 
{
	.initialized = FALSE
};


static inline void
log_output (const gchar    *domain,
            GLogLevelFlags  level,
            const gchar    *message)
{
	g_return_if_fail (message != NULL && message[0] != '\0');
}

static void
gspeechd_log_handler (const gchar    *domain,
                      GLogLevelFlags  level,
                      const gchar    *message,
                      gpointer        user_data)
{
	log_output (domain, level, message);

	/* Now show the message through stdout/stderr as usual */
	g_log_default_handler (domain, level, message, user_data);
}

gboolean
gspeechd_log_init (gint    level,
                   gchar * dirname)
{
	if (log_options.initialized) {
		return TRUE;
	}

	log_options.level = level;
	log_options.dirname = g_strdup (dirname);
	g_mutex_init (&log_options.mutex);

	g_log_set_default_handler (gspeechd_log_handler, NULL);

	log_options.initialized = TRUE;

	/* log binary name and version */
	g_message ("Starting %s %s", g_get_application_name (), PACKAGE_VERSION);
	return TRUE;
}


gboolean
gspeechd_log_finalize (void)
{
	if (!log_options.initialized) {
		return TRUE;
	}

	g_free (log_options.dirname);
	g_mutex_clear (&log_options.mutex);
	log_options.initialized = FALSE;

	return TRUE;
}

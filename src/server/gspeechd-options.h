/* gspeechd-options.c
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

#ifndef GSPEECHD_OPTIONS_H
#define GSPEECHD_OPTIONS_H

#include <glib.h>

G_BEGIN_DECLS

typedef struct _gspeechd_options {
	gint port;
	gint log_level;
} gspeechd_options;

const gspeechd_options * 
gspeechd_options_get (int argc, char * argv[]);

G_END_DECLS

#endif /* GSPEECHD_OPTIONS_H */

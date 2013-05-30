/* ssip.c
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

#include <glib.h>

#include "ssip.h"

struct _SsipCommand
{
	volatile gint   ref_count;

	SsipCmd  		cmd;
	gchar			param;
};

SsipCommand *
ssip_command_new (gchar *line)
{
	SsipCommand *command;

	command = g_slice_new0 (SsipCommand);
	command->ref_count = 1;

	return command; 
}

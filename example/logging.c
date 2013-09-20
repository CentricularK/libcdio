/*
  Copyright (C) 2013 Rocky Bernstein <rocky@gnu.org>

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/* Simple program to show how to set log verbosity. And write a custom
   log handler.
*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#define __CDIO_CONFIG_H__ 1
#endif

#ifdef HAVE_STDIO_H
#include <stdio.h>
#endif
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#ifdef HAVE_ERRNO_H
#include <errno.h>
#endif

#ifdef HAVE_LIMITS_H
#include <limits.h>
#endif

#include <cdio/cdio.h>
#include <cdio/cd_types.h>
#include <cdio/logging.h>

int
main(int argc, const char *argv[])
{
    char **ppsz_cd_drives=NULL, **c;
    /* Set the log level to the least verbosity. */
    cdio_loglevel_default = CDIO_LOG_WARN;

    /* Parse command arguemnts */
    if (argc > 2) {
	printf("Usage:\n%s <verbosity-level>\nShows cd drives with given verbosity level",
	       argv[0]);
	exit(EXIT_FAILURE);
    }

    if (argc == 2) {
        char *endptr;
        long loglevel;

        errno = 0;    /* To distinguish success/failure after call */
        loglevel = strtol(argv[1], &endptr, 10);

        /* Check for various possible errors, in parsing  the verbosity level. */
	if ((errno == ERANGE && (loglevel == LONG_MAX || loglevel == LONG_MIN))
	    || (errno != 0 && loglevel == 0)) {
            perror("strtol conversion error of loglevel");
	    exit(EXIT_FAILURE);
	}

        if (endptr == argv[1]) {
            fprintf(stderr, "No digits were found\n");
            exit(EXIT_FAILURE);
        }
	/* Finally! Set the verbosity level. */
	cdio_loglevel_default = loglevel;
    }

    /* Print out a list of CD-drives with the above set log level. */
    ppsz_cd_drives = cdio_get_devices(DRIVER_DEVICE);
    if (NULL != ppsz_cd_drives)
	for( c = ppsz_cd_drives; *c != NULL; c++ ) {
	    printf("-- Drive %s\n", *c);
	}
    cdio_free_device_list(ppsz_cd_drives);
    return 0;
}

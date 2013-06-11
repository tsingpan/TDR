/*
 * Portions Copyright (c) 1987, 1993, 1994
 * The Regents of the University of California.  All rights reserved.
 *
 * Portions Copyright (c) 2003-2007, PostgreSQL Global Development Group
 *
 * $PostgreSQL: pgsql/src/include/getopt_long.h,v 1.8 2007/01/05 22:19:50 momjian Exp $
 */
#ifndef _H_HP_GETOPT_LONG
#define _H_HP_GETOPT_LONG


#include "hotpot/hp_platform.h"

/* These are picked up from the system's getopt() facility. */
extern hpint32      hp_opterr;
extern hpint32      hp_optind;
extern hpint32      hp_optopt;
extern char *hp_optarg;

/* Some systems have this, otherwise you need to define it somewhere. */
extern hpint32      optreset;

#ifndef HAVE_STRUCT_OPTION

struct option
{
        const char *name;
        hpint32                     has_arg;
        hpint32                *flag;
        hpint32                     val;
};

#define no_argument 0
#define required_argument 1
#endif

#ifndef HAVE_GETOPT_LONG

HP_API hpint32 hp_getopt_long(hpint32 argc, char *const argv[],
                        const char *optstring,
                        const struct option * longopts, hpint32 *longindex);
#endif

#endif   /* GETOPT_LONG_H */


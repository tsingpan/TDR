#ifndef _H_HP_GETOPT
#define _H_HP_GETOPT

#include "hotpot/hp_platform.h"

HP_API hpint32 hp_getopt(hpint32 argc, char * const argv[], const char *optstring);
extern char *hp_optarg;
extern hpint32 hp_optind, hp_opterr, hp_optopt;
#include "hotpot/hp_getopt_long.h"

#endif   /* GETOPT_H */


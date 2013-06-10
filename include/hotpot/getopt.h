#ifndef GETOPT_H
#define GETOPT_H

#include "xbase.h"

XAPI int getopt(int argc, char * const argv[], const char *optstring);
extern char *optarg;
extern int optind, opterr, optopt;
#include "getopt_long.h"

#endif   /* GETOPT_H */


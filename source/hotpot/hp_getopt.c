#include "hotpot/hp_getopt.h"
#include "hotpot/hp_platform.h"

#include <stdio.h>
#include <string.h>


hpint32                 hp_opterr = 1,                     /* if error message should be printed */
                        hp_optind = 1,                     /* index into parent argv vector */
                        hp_optopt,                         /* character checked for validity */
                        optreset;                       /* reset getopt */
char       *hp_optarg;                             /* argument associated with option */

#define BADCH   (hpint32)'?'
#define BADARG  (hpint32)':'
#define EMSG    ""

/*
 * getopt
 *      Parse argc/argv argument vector.
 */
hpint32 hp_getopt(hpint32 nargc, char * const *nargv, const char *ostr)
{
        static char *place = EMSG;      /* option letter processing */
        const char       *oli;                        /* option letter list index */

        if (optreset || !*place)
        {                                                       /* update scanning pointer */
                optreset = 0;
                if (hp_optind >= nargc || *(place = nargv[hp_optind]) != '-')
                {
                        place = EMSG;
                        return -1;
                }
                if (place[1] && *++place == '-' && place[1] == '\0')
                {                                               /* found "--" */
                        ++hp_optind;
                        place = EMSG;
                        return -1;
                }
        }                                                       /* option letter okay? */
		//fix warning
        if ((hp_optopt = (hpint32) *place++) == (hpint32) ':' ||
                !(oli = strchr(ostr, hp_optopt)))				
        {
                /*
                 * if the user didn't specify '-' as an option, assume it means -1.
                 */
                if (hp_optopt == (hpint32) '-')
                        return -1;
                if (!*place)
                        ++hp_optind;
                if (hp_opterr && *ostr != ':')
                        (void) fprintf(stderr,
                                                   "illegal option -- %c\n", hp_optopt);
                return BADCH;
        }
        if (*++oli != ':')
        {                                                       /* don't need argument */
                hp_optarg = NULL;
                if (!*place)
                        ++hp_optind;
        }
        else
        {                                                       /* need an argument */
                if (*place)                             /* no white space */
                        hp_optarg = place;
                else if (nargc <= ++hp_optind)
                {                                               /* no arg */
                        place = EMSG;
                        if (*ostr == ':')
                                return BADARG;
                        if (hp_opterr)
                                (void) fprintf(stderr,
                                                           "option requires an argument -- %c\n",
                                                           hp_optopt);
                        return BADCH;
                }
                else
                        /* white space */
                        hp_optarg = nargv[hp_optind];
                place = EMSG;
                ++hp_optind;
        }
        return hp_optopt;                          /* dump back option letter */
}


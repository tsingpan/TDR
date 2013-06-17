#include <stdio.h>
#include <string.h>

#include "hotpot/hp_getopt_long.h"

#ifndef HAVE_INT_OPTRESET
hpint32                     optreset;
#endif

#define BADCH   '?'
#define BADARG  ':'
#define EMSG    ""


hpint32 hp_getopt_long(hpint32 argc, char *const argv[],
                        const char *optstring,
                        const struct option * longopts, hpint32 *longindex)
{
        static char *place = EMSG;      /* option letter processing */
        const char       *oli;                        /* option letter list index */

        if (optreset || !*place)
        {                                                       /* update scanning pointer */
                optreset = 0;

                if (hp_optind >= argc)
                {
                        place = EMSG;
                        return -1;
                }

                place = argv[hp_optind];

                if (place[0] != '-')
                {
                        place = EMSG;
                        return -1;
                }

                place++;

                if (place[0] && place[0] == '-' && place[1] == '\0')
                {                                               /* found "--" */
                        ++hp_optind;
                        place = EMSG;
                        return -1;
                }

                if (place[0] && place[0] == '-' && place[1])
                {
                        /* long option */
                        size_t          namelen;
                        hpint32                     i;

                        place++;

                        namelen = strcspn(place, "=");
                        for (i = 0; longopts[i].name != NULL; i++)
                        {
                                if (strlen(longopts[i].name) == namelen
                                        && strncmp(place, longopts[i].name, namelen) == 0)
                                {
                                        if (longopts[i].has_arg)
                                        {
                                                if (place[namelen] == '=')
                                                        hp_optarg = place + namelen + 1;
                                                else if (hp_optind < argc - 1)
                                                {
                                                        hp_optind++;
                                                        hp_optarg = argv[hp_optind];
                                                }
                                                else
                                                {
                                                        if (optstring[0] == ':')
                                                                return BADARG;
                                                        if (hp_opterr)
                                                                fprintf(stderr,
                                                                   "%s: option requires an argument -- %s\n",
                                                                                argv[0], place);
                                                        place = EMSG;
                                                        hp_optind++;
                                                        return BADCH;
                                                }
                                        }
                                        else
                                        {
                                                hp_optarg = NULL;
                                                if (place[namelen] != 0)
                                                {
                                                        /* XXX error? */
                                                }
                                        }

                                        hp_optind++;

                                        if (longindex)
                                                *longindex = i;

                                        place = EMSG;

                                        if (longopts[i].flag == NULL)
                                                return longopts[i].val;
                                        else
                                        {
                                                *longopts[i].flag = longopts[i].val;
                                                return 0;
                                        }
                                }
                        }

                        if (hp_opterr && optstring[0] != ':')
                                fprintf(stderr,
                                                "%s: illegal option -- %s\n", argv[0], place);
                        place = EMSG;
                        hp_optind++;
                        return BADCH;
                }
        }

        /* short option */
        hp_optopt = (hpint32) *place++;

        oli = strchr(optstring, hp_optopt);
        if (!oli)
        {
                if (!*place)
                        ++hp_optind;
                if (hp_opterr && *optstring != ':')
                        fprintf(stderr,
                                        "%s: illegal option -- %c\n", argv[0], hp_optopt);
                return BADCH;
        }

        if (oli[1] != ':')
        {                                                       /* don't need argument */
                hp_optarg = NULL;
                if (!*place)
                        ++hp_optind;
        }
        else
        {                                                       /* need an argument */
                if (*place)                             /* no white space */
                        hp_optarg = place;
                else if (argc <= ++hp_optind)
                {                                               /* no arg */
                        place = EMSG;
                        if (*optstring == ':')
                                return BADARG;
                        if (hp_opterr)
                                fprintf(stderr,
                                                "%s: option requires an argument -- %c\n",
                                                argv[0], hp_optopt);
                        return BADCH;
                }
                else
                        /* white space */
                        hp_optarg = argv[hp_optind];
                place = EMSG;
                ++hp_optind;
        }
        return hp_optopt;
}

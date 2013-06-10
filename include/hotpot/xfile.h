#ifndef _H_XFILE
#define _H_XFILE

#include "xbase.h"

#define XFILE_PATH_LENGTH 128
#define XFILE_MAX_PATH_COUNT 128

#ifdef WIN32
#define FILE_SEPARATOR '\\'
#else
#define FILE_SEPARATOR '/'
#endif


struct tagXFILE
{
	char includePath[XFILE_MAX_PATH_COUNT][XFILE_PATH_LENGTH];
	int itail;
};
#ifndef DEF_XFILE
typedef struct tagXFILE XFILE;
#define DEF_XFILE
#endif

XAPI int xfile_init(XFILE* xfile);

XAPI int xfile_add_path(XFILE* xfile, const char* path);

XAPI int xfile_fix_path(char* _path, int *_len);

XAPI int xfile_search_file(const XFILE* xfile, char* path, int *_len);

XAPI void xfile_cut_path(char* file_name);

XAPI void xfile_cut_suffix(char *file_name, const char* suffix);

#endif//_H_XFILE

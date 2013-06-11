#ifndef _H_HP_FILE
#define _H_HP_FILE

#include "hotpot/hp_platform.h"

#define HP_FILE_PATH_LENGTH 128
#define HP_FILE_MAX_PATH_COUNT 128

struct tagHP_FILE
{
	char includePath[HP_FILE_MAX_PATH_COUNT][HP_FILE_PATH_LENGTH];
	hpint32 itail;
};
#ifndef _DEF_HP_FILE
typedef struct tagHP_FILE HP_FILE;
#define _DEF_HP_FILE
#endif//_DEF_HP_FILE

HP_API hpint32 hp_file_init(HP_FILE* xfile);

HP_API hpint32 hp_file_add_path(HP_FILE* xfile, const char* path);

HP_API hpint32 hp_file_fix_path(char* _path, hpuint32 *_len);

HP_API hpint32 hp_file_search_file(const HP_FILE* xfile, char* path, hpuint32 *_len);

HP_API void hp_file_cut_path(char* file_name);

HP_API void hp_file_cut_suffix(char *file_name, const char* suffix);

#endif//_H_XFILE

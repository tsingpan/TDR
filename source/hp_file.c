#include "hotpot/hp_file.h"
#include "hotpot/hp_platform.h"
#include "hotpot/hp_error.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>


hpint32 hp_file_init(HP_FILE* xfile)
{
	if(xfile == NULL)
	{
		goto ERROR_RET;
	}
	xfile->itail = 0;

	return E_HP_NOERROR;
ERROR_RET:
	return E_HP_ERROR;
}

hpint32 hp_file_add_path(HP_FILE* xfile, const char* path)
{
	if((xfile == NULL) || (path == NULL))
	{
		return E_HP_ERROR;
	}

	if(xfile->itail < HP_FILE_MAX_PATH_COUNT)
	{
		snprintf(xfile->includePath[xfile->itail], HP_FILE_PATH_LENGTH, "%s", path);
		++xfile->itail;
		return E_HP_NOERROR;
	}
	return E_HP_ERROR;
}

hpint32 hp_file_fix_path(char* _path, hpuint32 *_len)
{	
	char path[HP_FILE_PATH_LENGTH];
	char *p[HP_FILE_PATH_LENGTH];
	hpuint32 ptail = 0;
	hpint32 first = 1;
	hpuint32 i = 0;
	hpuint32 j = 0;
	hpuint32 len = 0;
	hpuint32 tlen = 0;

	if((_path == NULL) || (_len == NULL))
	{
		return E_HP_ERROR;
	}
	len = (hpuint32)strlen(_path);

	snprintf(path, HP_FILE_PATH_LENGTH, "%s", _path);
#ifndef WIN32
	if(_path[0] == '/')
	{
		p[0] = "/";
		ptail = 1;
	}
#endif //WIN32

	for(i = 0; i < len; ++i)
	{
		if((path[i] == '/') || (path[i] == '\\'))
		{
			path[i] = 0;
			first = 1;
		}
		else if(first)
		{
			p[ptail++] = &path[i];
			first = 0;
		}
	}

	for(i = 0; i < ptail; ++i)
	{
		if(p[i][0] == '.')
		{
			tlen = (hpuint32)strlen(p[i]);			
			for(j = i - 1; (tlen > 1) && (j >= 0); --j)
			{
				if(p[j][0] == '.')
				{
					break;
				}
				else if(p[j][0] != 0)
				{
					p[j][0] = 0;
					--tlen;
				}
			}

			for(j = 0; j < tlen; ++j)
			{
				p[i][j] = '.';
			}
			p[i][tlen] = 0;

			if(tlen == 1)
			{
				p[i][0] = 0;
			}
		}
	}

	memset(_path, 0, len);
	len = 0;

	for(i = 0; i < ptail; ++i)
	{
		if((len > 0) && (strlen(p[i]) > 0) && (_path[len - 1] != HP_FILE_SEPARATOR))
		{
			if(len < *_len)
			{
				_path[len++] += HP_FILE_SEPARATOR;
			}
			else
			{
				*_len = len;
				return E_HP_ERROR;
			}
		}

		for(j = 0; j < (hpint32)strlen(p[i]); ++j)
		{
			if(len < *_len)
			{
				_path[len++] = p[i][j];
			}
			else
			{
				*_len = len;
				return E_HP_ERROR;
			}
		}
	}

	if(len < *_len)
	{
		_path[len++] = 0;
		*_len = len;
	}
	return E_HP_NOERROR;
}


hpint32 hp_file_search_file(const HP_FILE* xfile, char* path, hpuint32 *_len)
{
	hpint32 i = 0;
	hpuint32 len = 0;
	char realPath[HP_FILE_PATH_LENGTH];

	if((xfile == NULL) | (path == NULL) || (_len == NULL))
	{
		return E_HP_ERROR;
	}
	len = (hpuint32)strlen(path);

	if(fopen(path, "r") != NULL)
	{
		*_len = len;
		return E_HP_NOERROR;
	}
#ifndef WIN32
	if(path[0] == '/')
	{
		return E_HP_ERROR;
	}
#endif//WIN32

	for(i = 0; i < xfile->itail; ++i)
	{
		snprintf(realPath, HP_FILE_PATH_LENGTH, "%s%c%s", xfile->includePath[i], HP_FILE_SEPARATOR, path);
		len = HP_FILE_PATH_LENGTH;
		if(hp_file_fix_path(realPath, &len) != E_HP_NOERROR)
		{
			memset(path, 0, *_len);
			*_len = 0;
			return E_HP_ERROR;
		}
		if(fopen(realPath, "r") != NULL)
		{
			if(len < *_len)
			{
				*_len = len;
			}
			snprintf(path, *_len, "%s", realPath);
			return E_HP_NOERROR;
		}
	}
	memset(path, 0, *_len);
	*_len = 0;
	return E_HP_ERROR;
}

void hp_file_cut_path(char* file_name)
{
	hpuint32 i = 0;
	hpuint32 pos = 0;
	hpuint32 len = (hpuint32)strlen(file_name);
	
	for(i = 0; i < len; ++i)
	{
		if((file_name[i] == '/') || (file_name[i] == '\\'))
		{
			file_name[i] = 0;
			pos = i + 1;
		}
	}
	memmove(file_name, file_name + pos, len - pos);
	file_name[len - pos] = 0;
}

void hp_file_cut_suffix(char *file_name, const char* suffix)
{
	if(strlen(file_name) >= strlen(suffix))
	{
		if(strcmp(file_name + strlen(file_name) - strlen(suffix), suffix) == 0)
		{
			file_name[strlen(file_name) - strlen(suffix)] = 0;
		}
	}
}

#include "hotpot/xfile.h"
#include "hotpot/xbase.h"
#include "hotpot/xerror.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>


#ifdef WIN32
#pragma warning(disable:4996)
#endif


int xfile_init(XFILE* xfile)
{
	if(xfile == NULL)
	{
		goto ERROR_RET;
	}
	xfile->itail = 0;

	return XNOERROR;
ERROR_RET:
	return XERROR;
}

int xfile_add_path(XFILE* xfile, const char* path)
{
	if((xfile == NULL) || (path == NULL))
	{
		return XERROR;
	}

	if(xfile->itail < XFILE_MAX_PATH_COUNT)
	{
		SNPRINTF(xfile->includePath[xfile->itail], XFILE_PATH_LENGTH, "%s", path);
		++xfile->itail;
		return XNOERROR;
	}
	return XERROR;
}

int xfile_fix_path(char* _path, int *_len)
{	
	char path[XFILE_PATH_LENGTH];
	char *p[XFILE_PATH_LENGTH];
	int ptail = 0;
	int first = 1;
	int i = 0;
	int j = 0;
	int len = 0;
	int tlen = 0;

	if((_path == NULL) || (_len == NULL))
	{
		return XERROR;
	}
	len = strlen(_path);

	SNPRINTF(path, XFILE_PATH_LENGTH, "%s", _path);
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
			tlen = strlen(p[i]);			
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
		if((len > 0) && (strlen(p[i]) > 0) && (_path[len - 1] != FILE_SEPARATOR))
		{
			if(len < *_len)
			{
				_path[len++] += FILE_SEPARATOR;
			}
			else
			{
				*_len = len;
				return XERROR;
			}
		}

		for(j = 0; j < (int)strlen(p[i]); ++j)
		{
			if(len < *_len)
			{
				_path[len++] = p[i][j];
			}
			else
			{
				*_len = len;
				return XERROR;
			}
		}
	}

	if(len < *_len)
	{
		_path[len++] = 0;
		*_len = len;
	}
	return XNOERROR;
}


int xfile_search_file(const XFILE* xfile, char* path, int *_len)
{
	int i = 0;
	int len = 0;
	char realPath[XFILE_PATH_LENGTH];

	if((xfile == NULL) | (path == NULL) || (_len == NULL))
	{
		return XERROR;
	}
	len = strlen(path);

	if(fopen(path, "r") != NULL)
	{
		*_len = len;
		return XNOERROR;
	}
#ifndef WIN32
	if(path[0] == '/')
	{
		return XERROR;
	}
#endif//WIN32

	for(i = 0; i < xfile->itail; ++i)
	{
		SNPRINTF(realPath, XFILE_PATH_LENGTH, "%s%c%s", xfile->includePath[i], FILE_SEPARATOR, path);
		len = XFILE_PATH_LENGTH;
		if(xfile_fix_path(realPath, &len) != XNOERROR)
		{
			memset(path, 0, *_len);
			*_len = 0;
			return XERROR;
		}
		if(fopen(realPath, "r") != NULL)
		{
			if(len < *_len)
			{
				*_len = len;
			}
			SNPRINTF(path, *_len, "%s", realPath);
			return XNOERROR;
		}
	}
	memset(path, 0, *_len);
	*_len = 0;
	return XERROR;
}

void xfile_cut_path(char* file_name)
{
	int i = 0;
	int pos = 0;
	int len = strlen(file_name);
	
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

void xfile_cut_suffix(char *file_name, const char* suffix)
{
	if(strlen(file_name) >= strlen(suffix))
	{
		if(strcmp(file_name + strlen(file_name) - strlen(suffix), suffix) == 0)
		{
			file_name[strlen(file_name) - strlen(suffix)] = 0;
		}
	}
}

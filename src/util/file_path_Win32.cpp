#include <cstring>
#include <cstdlib>
#include <windows.h>
#include <util/file_path.h>

static const int buffer_size = 4096;

static wchar_t* to_utf16(const char* str)
{
    wchar_t* res;
    int res_size;

    res_size = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
    res = new wchar_t[res_size];
    MultiByteToWideChar(CP_UTF8, 0, str, -1, res, res_size);
    return res;
}

static char* to_utf8(const wchar_t* wstr)
{
    char* res;
    int res_size;

    res_size = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
    res = new char[res_size];
    WideCharToMultiByte(CP_UTF8, 0, wstr, -1, res, res_size, NULL, NULL);
    return res;
}

const char* root_path()
{
    static char root[buffer_size];
    wchar_t wroot[buffer_size];
    char* tmp;

    if (*root == 0)
    {
		size_t last = 0;

        GetModuleFileNameW(NULL, wroot, buffer_size);
        tmp = to_utf8(wroot);
        strncpy(root, tmp, buffer_size);
        delete [] tmp;
        for (size_t i = 0; root[i]; ++i)
        {
			if (root[i] == '\\')
			{
				root[i] = '/';
				last = i;
			}
        }
		if (last > 2)
		{
			root[last] = 0;
		}
    }
    return root;
}

const char* data_path(const char* rel_path)
{
    static char buffer[buffer_size];

    strcpy(buffer, root_path());
    strcat(buffer, "/data/");
    strcat(buffer, rel_path);

    return buffer;
}

#include <iostream>

const char* save_path(const char* rel_path)
{
    static char buffer[buffer_size];

    strcpy(buffer, root_path());
    strcat(buffer, "/save/");
    strcat(buffer, rel_path);

    return buffer;
}

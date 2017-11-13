#include <cstring>
#include <core/file_system.h>

bool FileSystem::mkpath(const char* path)
{
    char* buffer;
    size_t path_len;
    size_t i;
    DirStatus status;

    status = DirStatus::Ok;
    path_len = strlen(path);
    buffer = new char[path_len + 1];
    strcpy(buffer, path);
    i = 0;
    if (buffer[0] == '/')
        i++;

#if defined(WIN32)
	if (buffer[1] == ':')
		i = 3;
#endif

    while (buffer[i])
    {
        if (buffer[i] == '/')
        {
            buffer[i] = 0;
            FileSystem::mkdir(buffer);
            buffer[i] = '/';
        }
        i++;
    }
    status = FileSystem::mkdir(buffer);

    if (status == DirStatus::Error)
        return false;
    return true;
}

#include <cerrno>
#include <cstring>
#include <cstdlib>
#include <util/file_path.h>

#if defined(__APPLE__)
# include <mach-o/dyld.h>
# include <CoreServices/CoreServices.h>
#endif

#include <sys/stat.h>

#include <config.h>

static const int buffer_size = 4096;

const char* root_path()
{
    static char root[buffer_size];

    if (*root == 0)
    {
#if defined(__APPLE__)
        char buffer[buffer_size];
        uint32_t bufsize = buffer_size;
        _NSGetExecutablePath(buffer, &bufsize);
        for (size_t i = strlen(buffer) - 1; i > 0; --i)
        {
            if (buffer[i] == '/')
            {
                buffer[i] = 0;
                break;
            }
        }
        strcat(buffer, "/..");
        realpath(buffer, root);
#endif
    }
    return root;
}

const char* data_path(const char* rel_path)
{
    static char buffer[buffer_size];

    char tmp[buffer_size];

    strcpy(tmp, root_path());
    strcat(tmp, "/");
#if APP_BUNDLE
    strcat(tmp, "Resources");
#else
    strcat(tmp, SUPREMACY_DATA_DIR);
#endif
    strcat(tmp, "/");
    strcat(tmp, rel_path);
    realpath(tmp, buffer);

    return buffer;
}

const char* save_path(const char* rel_path)
{
    static char buffer[buffer_size];

    char tmp[buffer_size];

#if APP_BUNDLE
    FSRef ref;
    FSFindFolder(kUserDomain, kApplicationSupportFolderType, kCreateFolder, &ref);
    FSRefMakePath(&ref, (UInt8*)&tmp, buffer_size);
    strcat(tmp, "/Supremacy");
#else
    const char* xdg = getenv("XDG_DATA_HOME");
    const char* home = getenv("HOME");

    if (xdg && xdg[0])
        strcpy(tmp, xdg);
    else
    {
        strcpy(tmp, home);
        strcat(tmp, "/.local/share");
    }
    strcat(tmp, "/supremacy");
#endif
    strcat(tmp, "/");
    strcat(tmp, rel_path);
    strcpy(buffer, tmp);

    return buffer;
}

bool make_path(const char* path)
{
    char buffer[buffer_size];
    size_t i;
    int ret;

    strcpy(buffer, path);
    i = 0;
    if (buffer[0] == '/')
        i++;

    while (buffer[i])
    {
        if (buffer[i] == '/')
        {
            buffer[i] = 0;
            mkdir(buffer, S_IRWXU | S_IRWXG | S_IRWXO);
            buffer[i] = '/';
        }
        i++;
    }
    errno = 0;
    ret = mkdir(buffer, S_IRWXU | S_IRWXG | S_IRWXO);

    if (ret == -1 && errno != EEXIST)
        return false;
    return true;
}

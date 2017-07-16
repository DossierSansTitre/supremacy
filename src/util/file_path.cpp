#include <cstring>
#include <cstdlib>
#include <util/file_path.h>

#if defined(__APPLE__)
# include <mach-o/dyld.h>
#endif

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
    strcat(tmp, SUPREMACY_DATA_DIR);
    strcat(tmp, "/");
    strcat(tmp, rel_path);
    realpath(tmp, buffer);

    return buffer;
}

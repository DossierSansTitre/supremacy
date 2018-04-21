#include <cstring>
#include <windows.h>
#include <Core/file_system.h>

struct FileSystem::Directory
{
    HANDLE  handle;
    char*   name;
    bool    started;
};

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

bool FileSystem::file_exists(const char* path)
{
    wchar_t* wpath;
    DWORD attr;

    wpath = to_utf16(path);
    attr = GetFileAttributesW(wpath);
    delete [] wpath;
    if (attr == INVALID_FILE_ATTRIBUTES)
        return false;
    else
        return true;
}

FileSystem::DirStatus FileSystem::mkdir(const char* path)
{
    wchar_t* wpath;
    BOOL res;
    DWORD error;

    wpath = to_utf16(path);
    res = CreateDirectoryW(wpath, NULL);
    delete [] wpath;

    if (!res)
    {
        error = GetLastError();
        if (error == ERROR_ALREADY_EXISTS)
            return DirStatus::Exists;
        return DirStatus::Error;
    }
    return DirStatus::Ok;
}

FileSystem::Directory* FileSystem::open_directory(const char* path)
{
    char* buffer;
    size_t path_len;
    wchar_t* wpath;
    HANDLE handle;
    WIN32_FIND_DATAW find_data;
    Directory* dir;

    path_len = strlen(path);
    buffer = new char[path_len + 3];
    strcpy(buffer, path);
    strcat(buffer, "/*");
    wpath = to_utf16(buffer);
    delete [] buffer;
    dir = new Directory;
    handle = FindFirstFileW(wpath, &find_data);
    delete [] wpath;
    if (handle == INVALID_HANDLE_VALUE)
    {
        dir->handle = NULL;
        dir->name = NULL;
    }
    else
    {
        dir->handle = handle;
        dir->name = to_utf8(find_data.cFileName);
    }
    dir->started = false;
    return dir;
}

const char* FileSystem::read_directory(FileSystem::Directory* dir)
{
    WIN32_FIND_DATAW find_data;

    if (!dir->handle)
        return nullptr;
    if (!dir->started)
    {
        dir->started = true;
        return dir->name;
    }
    delete [] dir->name;
    if (!FindNextFileW(dir->handle, &find_data))
    {
        FindClose(dir->handle);
        dir->handle = nullptr;
        dir->name = nullptr;
        return nullptr;
    }
    dir->name = to_utf8(find_data.cFileName);
    return dir->name;
}

void FileSystem::close_directory(Directory* dir)
{
    if (dir->handle)
        FindClose(dir->handle);
    delete [] dir->name;
    delete dir;
}

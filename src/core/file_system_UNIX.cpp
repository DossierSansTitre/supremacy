#include <sys/types.h>
#include <sys/dir.h>
#include <sys/stat.h>
#include <cstring>
#include <core/file_system.h>

struct FileSystem::Directory
{
    DIR     dir;
};

bool FileSystem::file_exists(const char* path)
{
    struct stat s;

    if (stat(path, &s))
        return false;
    return true;
}

FileSystem::DirStatus FileSystem::mkdir(const char* path)
{
    if (::mkdir(path, S_IRWXU | S_IRWXG | S_IRWXO))
    {
        if (errno == EEXIST)
            return DirStatus::Exists;
        return DirStatus::Error;
    }
    return DirStatus::Ok;
}

FileSystem::Directory* FileSystem::open_directory(const char* path)
{
    DIR* dir;

    dir = opendir(path);
    return reinterpret_cast<Directory*>(dir);
}

const char* FileSystem::read_directory(FileSystem::Directory* dir)
{
    DIR* d;
    dirent* ent;

    d = reinterpret_cast<DIR*>(dir);

    if (!d)
        return nullptr;
    ent = readdir(d);
    if (ent)
        return ent->d_name;
    return nullptr;
}

void FileSystem::close_directory(Directory* dir)
{
    DIR* d;

    d = reinterpret_cast<DIR*>(dir);
    if (d)
        closedir(d);
}

#ifndef CORE_FILE_SYSTEM_H
#define CORE_FILE_SYSTEM_H

namespace FileSystem
{

enum class DirStatus
{
    Ok,
    Exists,
    Error
};

struct Directory;

DirStatus       mkdir(const char* path);
bool            mkpath(const char* path);
bool            file_exists(const char* path);

Directory*      open_directory(const char* path);
const char*     read_directory(Directory* dir);
void            close_directory(Directory* dir);

};

#endif

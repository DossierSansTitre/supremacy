#ifndef FILE_PATH_H
#define FILE_PATH_H

const char* root_path();
const char* data_path(const char* rel_path);
const char* save_path(const char* rel_path);
bool        make_path(const char* path);
bool        file_exist(const char* path);

#endif

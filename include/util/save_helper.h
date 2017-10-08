#ifndef UTIL_SAVE_HELPER_H
#define UTIL_SAVE_HELPER_H

#include <types.h>

const char* save_path_world_folder(u16 world_id);
const char* save_path_fortress_folder(u16 world_id);
const char* save_path_region_folder(u16 world_id);
const char* save_path_region(u16 world_id, u32 region_id);

#endif

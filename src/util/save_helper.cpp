#include <cstring>
#include <cstdio>
#include <util/save_helper.h>

static constexpr const size_t helper_buffer_size = 4096;
static char helper_buffer[helper_buffer_size];

const char* save_path_world_folder(u16 world_id)
{
    char tmp[32];

    sprintf(tmp, "world_%05d", world_id);
    strcpy(helper_buffer, save_path("worlds/"));
    strcat(helper_buffer, tmp);

    return helper_buffer;
}

const char* save_path_fortress(u16 world_id)
{
    save_path_world_folder(world_id);
    strcat(helper_buffer, "/fortress.bin");

    return helper_buffer;
}

const char* save_path_region_folder(u16 world_id)
{
    save_path_world_folder(world_id);
    strcat(helper_buffer, "/regions");

    return helper_buffer;
}

const char* save_path_region(u16 world_id, u32 region_id)
{
    char tmp[32];

    save_path_region_folder(world_id);
    strcat(helper_buffer, "/");
    sprintf(tmp, "%09d", region_id);
    strcat(helper_buffer, tmp);

    return helper_buffer;
}

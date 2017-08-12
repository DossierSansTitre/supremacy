#include <archive.h>
#include <biome.h>
#include <init.h>
#include <item_data.h>
#include <log.h>
#include <material.h>
#include <task.h>
#include <tile.h>
#include <util/file_path.h>

void init_game_data()
{
    Archive archive;

    log_line(LogLevel::Info, "Loading game data");
    archive.open(data_path("/supremacy.bin"));
    Tile::load(archive);
    Material::load(archive);
    ItemData::load(archive);
    Biome::load(archive);
    Task::load(archive);
    log_line(LogLevel::Info, "Game data loaded");
}

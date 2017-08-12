#include <tile.h>
#include <serialize.h>

SparseArray<Tile> Tile::_data;

static void load_tile(Tile& tile, MemoryFile& file)
{
    uint8_t flags;

    file.read(&tile.sym);
    file.read(&tile.dim_sym);
    file.read(&tile.dropping_frequency);
    file.read(&flags);
    tile.walkable = (flags & 0x01);
    tile.block = (flags & 0x02);
    tile.support = (flags & 0x04);
    tile.move_up = (flags & 0x08);
    tile.move_down = (flags & 0x10);
}

void Tile::load(Archive& archive)
{
    unserialize_resource_array(Tile::_data, archive, "tile.bin", load_tile);
}

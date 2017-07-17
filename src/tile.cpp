#include <tile.h>
#include <sup_file.h>
#include <memory_file.h>

Array<Tile> Tile::data;

void Tile::load(Archive& archive)
{
    SupFile sup;
    MemoryFile file;

    sup.open(archive, "tile.bin");
    while (sup.read(file))
    {
        uint16_t id;
        uint8_t flags;

        file.read(&id);
        data.resize(id + 1);
        Tile& tile = data.back();
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
}

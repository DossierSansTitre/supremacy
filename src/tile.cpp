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
        uint8_t tmp;

        file.read(&id);
        data.resize(id + 1);
        Tile& tile = data.back();
        file.read(&tile.sym);
        file.read(&tile.dim_sym);
        file.read(&tile.dropping_frequency);
        file.read(&tmp);
        tile.walkable = tmp;
    }
}

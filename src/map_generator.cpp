#include <map.h>

void generate_map(Map& map)
{
    static const int width = 256;
    static const int height = 256;
    static const int depth = 64;

    static const TileID tiles[] = {
        TileID::Block,
        TileID::Stairs
    };

    static const MaterialID materials[] = {
        MaterialID::Dirt,
        MaterialID::Rock,
        MaterialID::Wood
    };

    map.create(width, height, depth);

    for (int k = 0; k < depth; ++k)
    {
        for (int j = 0; j < height; ++j)
        {
            for (int i = 0; i < width; ++i)
            {
                map.set_tile(i, j, k, tiles[k % 2]);
                map.set_material(i, j, k, materials[(k / 2) % 3]);
            }
        }
    }
}

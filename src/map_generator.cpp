#include <map.h>

void generate_map(Map& map)
{
    static const int width = 200;
    static const int height = 200;

    map.create(width, height);

    for (int j = 0; j < height; ++j)
    {
        for (int i = 0; i < width; ++i)
        {
            map.set_tile(i, j, TileID::Stairs);
            map.set_material(i, j, MaterialID::Rock);
        }
    }
}

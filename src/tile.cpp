#include <tile.h>

const Tile Tile::tiles[] = {
    {' ', 0, false, 0},
    {' ', 0, false, 3},
    {'>', 0, true, 1},
    {128, 129, true, 1},
    {130, ' ', true, 0},
    {'T', ' ', false, 1},
};

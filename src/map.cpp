#include <map.h>

Map::Map(int width, int height) {
    _width = width;
    _height = height;
    _tiles = std::vector<TileID>(width * height);
    _materials = std::vector<MaterialID>(width * height);
}

void Map::generate() {
    for (int i = 0; i < _width * _height; ++i) {
        _tiles[i] = TileID::BLOCK;
        _materials[i] = MaterialID::ROCK;
    }
}

void Map::at(int x, int y, TileID &tile_id, MaterialID &material_id) {
    tile_id = tile_at(x, y);
    material_id = material_at(x, y);
}

TileID Map::tile_at(int x, int y) {
    return _tiles[toIndex(x, y)];
}

MaterialID Map::material_at(int x, int y) {
    return _materials[toIndex(x, y)];
}

int Map::toIndex(int x, int y) {
    return x + y * _width;
}

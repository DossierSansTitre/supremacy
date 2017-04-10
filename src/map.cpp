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
    int i = index(x, y);

    tile_id = tile_at(i);
    material_id = material_at(i);
}

TileID Map::tile_at(int i) {
    return _tiles[i];
}

TileID Map::tile_at(int x, int y) {
    return _tiles[index(x, y)];
}

MaterialID Map::material_at(int i) {
    return _materials[i];
}

MaterialID Map::material_at(int x, int y) {
    return _materials[index(x, y)];
}

int Map::index(int x, int y) {
    return x + y * _width;
}

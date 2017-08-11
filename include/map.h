#ifndef MAP_H
#define MAP_H

#include <vector>
#include <types.h>
#include <non_copyable.h>
#include <map_action.h>
#include <math/vector.h>
#include <std/array.h>

class Map : private NonCopyable
{
public:
    enum class Flash : uint8_t
    {
        None,
        Pending,
        Action
    };

    Map();
    ~Map();

    int width() const
    {
        return _width;
    }

    int height() const
    {
        return _height;
    }

    int depth() const
    {
        return _depth;
    }

    void at(int x, int y, int z, TileID& out_tile, MaterialID& out_material) const
    {
        int i = index(x, y, z);
        out_tile = tile_at(i);
        out_material = material_at(i);
    }

    TileID tile_at(int i) const
    {
        if (i == -1)
            return 0;
        return _tiles[i];
    }

    TileID tile_at(int x, int y, int z) const
    {
        return tile_at(index(x, y, z));
    }

    TileID tile_at(Vector3i pos) const
    {
        return tile_at(pos.x, pos.y, pos.z);
    }

    MaterialID material_at(int i) const
    {
        if (i == -1)
            return 0;
        return _materials[i];
    }

    MaterialID material_at(int x, int y, int z) const
    {
        return material_at(index(x, y, z));
    }

    uint16_t task_at(int i) const
    {
        if (i == -1)
            return 0;
        return _tasks[i];
    }

    uint16_t task_at(int x, int y, int z) const
    {
        return task_at(index(x, y, z));
    }

    MaterialID floor(Vector3i position) const
    {
        int i = index(position);
        if (i == -1)
            return 0;
        return _floors[i];
    }

    void set_floor(Vector3i position, MaterialID material)
    {
        int i = index(position);
        if (i == -1)
            return;
        _floors[i] = material;
    }

    bool visible(int i) const
    {
        if (i == -1)
            return false;
        return _visible[i];
    }

    bool visible(int x, int y, int z) const
    {
        return visible(index(x, y, z));
    }

    bool occupied(Vector3i pos) const
    {
        int i = index(pos);
        if (i == -1)
            return false;
        return _occupied[i];
    }

    Flash flash(Vector3i pos) const
    {
        int i = index(pos);
        if (i == -1)
            return Flash::None;
        return _flash[i];
    }

    int index(Vector3i pos) const
    {
        return index(pos.x, pos.y, pos.z);
    }

    int index(int x, int y, int z) const
    {
        if (x < 0 || x >= _width || y < 0 || y >= _height || z < 0 || z >= _depth)
            return -1;
        return x + y * _width + z * _width * _height;
    }

    void        create(int width, int height, int depth);
    void        destroy();

    void        set_tile(int index, TileID tile);
    void        set_tile(int x, int y, int z, TileID tile);
    void        set_tile(Vector3i pos, TileID tile);
    void        set_material(int index, MaterialID material);
    void        set_material(int x, int y, int z, MaterialID material);
    void        set_task(int x, int y, int z, uint16_t task);
    void        set_occupied(Vector3i pos, bool occupied);
    void        set_flash(Vector3i pos, Flash flash);

    void        compute_visibility(int x, int y, int z);
    void        compute_visibility();

    size_t      task_count() const { return _task_positions.size(); }
    Vector3i    task_by_index(size_t index) { return _task_positions[index]; }

    void        tick();

private:
    void        post_update(Vector3i pos);
    void        neighbor_updated(Vector3i pos);

    int                     _width;
    int                     _height;
    int                     _depth;
    TileID*                 _tiles;
    MaterialID*             _materials;
    Array<MaterialID>       _floors;
    Array<uint16_t>         _tasks;
    Array<Vector3i>         _task_positions;
    std::vector<bool>       _visible;
    std::vector<bool>       _occupied;
    std::vector<Flash>      _flash;
    std::vector<int>        _flash_reset;
};

void generate_map(Map& map, uint32_t seed);

#endif

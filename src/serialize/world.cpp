#include <fstream>
#include <serialize.h>
#include <world.h>
#include <util/save_helper.h>

void save_world(const World& world)
{
    std::ofstream stream;

    make_path(save_path_region_folder(world.world_id));
    stream.open(save_path_region(world.world_id, world.region_id), std::ios::binary);
    serialize_world(stream, world);
    stream.close();

    stream.open(save_path_fortress(world.world_id), std::ios::binary);
    stream.write((char*)&world.region_id, 4);
    stream.close();
}

void serialize_world(std::ofstream& stream, const World& world)
{
    serialize(stream, &world.size.x, 1);
    serialize(stream, &world.size.y, 1);
    serialize(stream, &world.size.z, 1);
    serialize_map(stream, world.map);
    serialize_actors(stream, world.actors);
    serialize_items(stream, world.items);
    serialize(stream, &world.camera.x, 1);
    serialize(stream, &world.camera.y, 1);
    serialize(stream, &world.camera.z, 1);
}

World* load_world(u16 world_id, u32 region_id)
{
    std::ifstream stream;

    stream.open(save_path_region(world_id, region_id), std::ios::binary);
    return unserialize_world(stream, world_id, region_id);
}

World* unserialize_world(std::ifstream& stream, u16 world_id, u32 region_id)
{
    World* world;
    Vector3i size;

    unserialize(stream, &size.x, 1);
    unserialize(stream, &size.y, 1);
    unserialize(stream, &size.z, 1);

    world = new World(world_id, region_id, size);
    unserialize_map(world->map, stream);
    unserialize_actors(world->actors, stream);
    unserialize_items(world->items, stream);
    unserialize(stream, &world->camera.x, 1);
    unserialize(stream, &world->camera.y, 1);
    unserialize(stream, &world->camera.z, 1);

    return world;
}

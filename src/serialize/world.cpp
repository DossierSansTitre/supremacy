#include <fstream>
#include <serialize.h>
#include <world.h>

void save_world(const World& world)
{
    std::ofstream stream;

    stream.open("world_dump.bin", std::ios::binary);
    serialize_world(stream, world);
}

void serialize_world(std::ofstream& stream, const World& world)
{
    serialize_map(stream, world.map);
    serialize_actors(stream, world.actors);
    serialize_items(stream, world.items);
    serialize(stream, &world.camera.x, 1);
    serialize(stream, &world.camera.y, 1);
    serialize(stream, &world.camera.z, 1);
}

#include <serialize.h>
#include <actors.h>

void serialize_actors(std::ofstream& stream, const Actors& actors)
{
    u32 size;
    u32 free_size;

    size = actors._actor_id.size();
    free_size = actors._free.size();

    stream.write((char*)&size, sizeof(size));
    stream.write((char*)&free_size, sizeof(free_size));

    serialize(stream, actors._actor_id.data(), size);
    serialize(stream, actors._pos.data(), size);
    serialize(stream, actors._health.data(), size);
    serialize(stream, actors._speed.data(), size);
    serialize(stream, actors._task.data(), size);
    serialize(stream, actors._counter.data(), size);
    serialize(stream, actors._free.data(), free_size);
    serialize(stream, &actors._count, 1);
}

void unserialize_actors(Actors& actors, std::ifstream& stream)
{
    u32 size;
    u32 free_size;

    stream.read((char*)&size, sizeof(size));
    stream.read((char*)&free_size, sizeof(free_size));

    actors._actor_id.resize(size);
    actors._pos.resize(size);
    actors._health.resize(size);
    actors._speed.resize(size);
    actors._task.resize(size);
    actors._path_finder.resize(size);
    actors._path.resize(size);
    actors._counter.resize(size);
    actors._free.resize(free_size);

    unserialize(stream, actors._actor_id.data(), size);
    unserialize(stream, actors._pos.data(), size);
    unserialize(stream, actors._health.data(), size);
    unserialize(stream, actors._speed.data(), size);
    unserialize(stream, actors._task.data(), size);
    unserialize(stream, actors._counter.data(), size);
    unserialize(stream, actors._free.data(), free_size);
    unserialize(stream, &actors._count, 1);
}

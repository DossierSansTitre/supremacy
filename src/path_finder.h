#ifndef PATH_FINDER_H
#define PATH_FINDER_H

#include <cstdint>
#include <vector>
#include <set>
#include <path.h>

class PathFinder
{
public:
    enum class State
    {
        Idle,
        Pending,
        Finished
    };

    PathFinder();
    ~PathFinder();

    void    start(Vec3 start);
    bool    fetch(Vec3& node);
    void    explore(Vec3 node);
    void    finish(Path& path);
    void    reset();

private:

    uint32_t                    _current;
    uint32_t                    _size;
    std::vector<uint32_t>       _open;
    std::vector<uint32_t>       _open_next;
    std::set<Vec3>              _closed;
    std::vector<Vec3>           _position;
    std::vector<uint32_t>       _parent;
};

#endif

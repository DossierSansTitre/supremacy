#ifndef PATH_FINDER_H
#define PATH_FINDER_H

#include <cstdint>
#include <vector>
#include <set>
#include <queue>
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

    void    start(Vec3 start, int32_t cost);
    bool    fetch(Vec3& node);
    void    finish(Path& path);
    void    finish_with(Path& path, Vec3 node);
    void    explore(Vec3 node, int32_t cost);
    void    reset();

private:
    struct InternalNode {
        uint32_t    index;
        int32_t     cost;
    };

    friend bool operator<(InternalNode, InternalNode);

    using OpenQueue = std::priority_queue<InternalNode>;

    uint32_t                    _current;
    uint32_t                    _size;
    OpenQueue                   _open;
    std::set<Vec3>              _closed;
    std::vector<Vec3>           _position;
    std::vector<uint32_t>       _parent;
};

inline bool operator<(PathFinder::InternalNode lhs, PathFinder::InternalNode rhs)
{
    return lhs.cost > rhs.cost;
}


#endif

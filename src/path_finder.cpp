#include <path_finder.h>

PathFinder::PathFinder()
{
    reset();
}

PathFinder::~PathFinder()
{

}

void PathFinder::start(Vector3i start, int32_t cost)
{
    reset();
    explore(start, cost);
}

bool PathFinder::fetch(Vector3i& node)
{
    InternalNode internal_node;

    if (_open.empty())
        return false;
    internal_node = _open.top();
    _open.pop();
    node = _position[internal_node.index];
    _current = internal_node.index;
    return true;
}

void PathFinder::finish(Path& path)
{
    uint32_t index;

    index = _current;
    path.clear();
    for (;;)
    {
        path.push_back(_position[index]);
        if (index == 0)
            break;
        index = _parent[index];
    }
    reset();
}

void PathFinder::finish_with(Path& path, Vector3i node)
{
    _position.push_back(node);
    _parent.push_back(_current);
    _current = _size;
    _size++;
    finish(path);
}

void PathFinder::explore(Vector3i node, int32_t cost)
{
    if (_closed.count(node) == 0)
    {
        _closed.insert(node);
        _position.push_back(node);
        _parent.push_back(_current);
        _open.push({_size, cost});
        _size++;
    }
}

void PathFinder::reset()
{
    _current = 0;
    _size = 0;
    _open = OpenQueue();
    _closed.clear();
    _position.clear();
    _parent.clear();
}

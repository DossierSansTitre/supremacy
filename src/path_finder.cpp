#include <path_finder.h>
#include <iostream>

PathFinder::PathFinder()
{
    reset();
}

PathFinder::~PathFinder()
{

}

void PathFinder::start(Vec3 start)
{
    reset();
    explore(start);
}

bool PathFinder::fetch(Vec3& node)
{
    uint32_t index;

    if (_open.empty())
    {
        if (_open_next.empty())
            return false;
        _open = _open_next;
        _open_next.clear();
    }
    index = _open.back();
    _open.pop_back();
    node = _position[index];
    _current = index;
    return true;
}

void PathFinder::finish()
{
    std::cout << "Found node: " << _position[_current].x << " " << _position[_current].y << " " << _position[_current].z << std::endl;
    reset();
}

void PathFinder::finish_with(Vec3 node)
{
    _position.push_back(node);
    _parent.push_back(_current);
    _current = _size;
    _size++;
    finish();
}

void PathFinder::explore(Vec3 node)
{
    if (_closed.count(node) == 0)
    {
        _closed.insert(node);
        _position.push_back(node);
        _parent.push_back(_current);
        _open_next.push_back(_size);
        _size++;
    }
}

void PathFinder::reset()
{
    _current = 0;
    _size = 0;
    _open.clear();
    _open_next.clear();
    _closed.clear();
    _position.clear();
    _parent.clear();
    _path.clear();
}

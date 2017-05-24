#ifndef PATH_FINDER_H
#define PATH_FINDER_H

#include <non_copyable.h>
#include <vec3.h>
#include <vector.h>

class PathFinder : public NonCopyable
{
public:
    PathFinder();
    ~PathFinder();

private:
    std::vector<Vec3>   _nodes;
};

/* Use case: */


#endif

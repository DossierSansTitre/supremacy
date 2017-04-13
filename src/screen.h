#ifndef SCREEN_H
#define SCREEN_H

#include <wish.h>
#include <non_copyable.h>

struct Screen : private NonCopyable
{
    wish_term*  term;
    wish_view*  screen;
    wish_view*  top_bar;
};

#endif

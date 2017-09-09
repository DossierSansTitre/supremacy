#ifndef DRAW_H
#define DRAW_H

#include <types.h>
#include <math/rect.h>
#include <draw_buffer.h>

class Game;
class DrawBuffer;
struct World;
class Selection;
class Worldmap;

void draw_selection(DrawBuffer& draw_buffer, World& world, Selection& selection, u32 render_tick);
void draw_world(DrawBuffer& draw_buffer, World& world, Game& game, u32 render_tick);
void draw_worldmap(DrawBuffer& buffer, Worldmap& worldmap, Vector2i offset);
void draw(DrawBuffer& db, const Worldmap& worldmap, Rect2i dst, Rect2i src);

#endif

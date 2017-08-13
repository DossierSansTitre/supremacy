#ifndef UPDATE_H
#define UPDATE_H

#include <types.h>

struct World;
class Keyboard;
class Selection;
class DrawBuffer;

void update_ai(World& world, u32 tick);
void update_game(World& world);
void update_ui(World& world, Keyboard& keyboard, Selection& selection, Vector2u viewport);

#endif

#ifndef UPDATE_H
#define UPDATE_H

#include <types.h>
#include <engine/input.h>

struct World;
class Keyboard;
class Selection;
class DrawBuffer;

void update_ai(World& world, u32 tick);
void update_ui(InputEvent e, Keyboard& keyboard, World& world, Selection& selection, Vector2u viewport);

#endif

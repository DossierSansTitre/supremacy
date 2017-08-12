#include <scene/main_menu_scene.h>
#include <scene/ingame_scene.h>
#include <engine/game.h>

static void render_centered(DrawBuffer& buffer, int y, const char* str, Color color)
{
    int x;

    x = buffer.width() / 2 - strlen(str) / 2;
    print(buffer, x, y, str, color, {0, 0, 0});
}

void MainMenuScene::setup()
{
    _selection = 0;
}

void MainMenuScene::update()
{
    auto& keyboard = game().keyboard();

    if (keyboard.pressed(SDL_SCANCODE_RETURN))
        menu_action();
    else if (keyboard.pressed(SDL_SCANCODE_DOWN))
        _selection++;
    else if (keyboard.pressed(SDL_SCANCODE_UP))
        _selection--;
    if (_selection < 0)
        _selection = 0;
    else if (_selection > 2)
        _selection = 2;
}

void MainMenuScene::render(DrawBuffer& draw_buffer)
{
    static const char* menu_items[] = {
        "Create World",
        "Load World",
        nullptr,
        "Quit"
    };
    size_t max;
    int sel;
    Color color;

    render_centered(draw_buffer, 8, "SUPREMACY", {255, 0, 0});

    max = sizeof(menu_items) / sizeof(*menu_items);
    sel = 0;
    for (size_t i = 0; i < max; ++i)
    {
        color = {255, 255, 255};
        if (menu_items[i] == nullptr)
            continue;
        if (sel != _selection)
            color *= 0.5;
        render_centered(draw_buffer, 14 + i, menu_items[i], color);
        sel++;
    }
}

void MainMenuScene::menu_action()
{
    if (_selection == 0)
    {
        game().set_scene<IngameScene>();
    }
    else if (_selection == 2)
    {
        game().stop();
    }
}

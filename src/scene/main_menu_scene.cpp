#include <scene/main_menu_scene.h>
#include <scene/ingame_scene.h>
#include <scene/worldmap_generation_scene.h>
#include <scene/load_world_selection_scene.h>
#include <engine/game.h>

static void render_centered(DrawBuffer& buffer, int y, const char* str, Color color)
{
    int x;

    x = (int)(buffer.width() / 2 - strlen(str) / 2);
    print(buffer, x, y, str, color, {0, 0, 0});
}

void MainMenuScene::setup()
{
    _selection = 0;
}

void MainMenuScene::update()
{
    auto& input = game().input();
    InputEvent e;

    while (input.poll(e))
    {
        if (e.type != InputEventType::KeyDown)
            continue;
        switch (e.key.scancode)
        {
            case Keyboard::Enter:
                menu_action();
                break;
            case Keyboard::Down:
                _selection++;
                break;
            case Keyboard::Up:
                _selection--;
        }
        if (_selection < 0)
            _selection = 0;
        else if (_selection > 2)
            _selection = 2;
    }
}

void MainMenuScene::render(DrawBuffer& draw_buffer)
{
    static const char* menu_items[] = {
        "Play",
        "Create World",
        nullptr,
        "Quit"
    };
    size_t max;
    int sel;
    Color color;

    render_centered(draw_buffer, 8, "SUPREMACY", {255, 0, 0});

    max = (int)(sizeof(menu_items) / sizeof(*menu_items));
    sel = 0;
    for (int i = 0; i < max; ++i)
    {
        color = {255, 255, 255};
        if (menu_items[i] == nullptr)
            continue;
        if (sel != _selection)
            color *= 0.5;
        render_centered(draw_buffer, 14 + i, menu_items[i], color);
        sel++;
    }
    int fps_render = game().fps_counter_render().get();
    int fps_update = game().fps_counter_update().get();
    printf(draw_buffer, 0, 0, "FPS: %d(%d)", {0, 0, 0}, {255, 255, 255}, fps_render, fps_update);
}

void MainMenuScene::menu_action()
{
    if (_selection == 0)
    {
        game().set_scene<LoadWorldSelectionScene>();
    }
    if (_selection == 1)
    {
        game().set_scene<WorldmapGenerationScene>();
    }
    else if (_selection == 2)
    {
        game().stop();
    }
}

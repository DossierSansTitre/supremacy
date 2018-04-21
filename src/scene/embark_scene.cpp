#include <scene/embark_scene.h>
#include <scene/load_world_selection_scene.h>
#include <scene/ingame_scene.h>
#include <serialize.h>
#include <engine/game.h>
#include <draw.h>
#include <math/algorithm.h>
#include <biome.h>

static const int panel_size = 15;

EmbarkScene::EmbarkScene(u16 world_id)
: _world_id(world_id)
{

}

EmbarkScene::~EmbarkScene()
{

}

void EmbarkScene::setup()
{
    int w;
    int h;

    _cursor = {0, 0};
    _camera = {0, 0};
    _worldmap = load_worldmap(_world_id);
    w = game().draw_buffer().size().x / 2 - 4 - panel_size;
    h = game().draw_buffer().size().y - 2;
    _width = min(w, h);
}

void EmbarkScene::teardown()
{
    delete _worldmap;
}

void EmbarkScene::update()
{
    auto& input = game().input();
    InputEvent e;

    while (input.poll(e))
    {
        if (e.type == InputEventType::KeyDown)
        {
            bool shift = input.keyboard.down(Keyboard::Shift);
            int speed = shift ? 10 : 1;

            switch (e.key.scancode)
            {
            case Keyboard::Escape:
                game().set_scene<LoadWorldSelectionScene>();
                break;
            case Keyboard::Enter:
                if (embark())
                    return;
                break;
            case Keyboard::Right:
                move_cursor(Vector2i(speed, 0));
                break;
            case Keyboard::Left:
                move_cursor(Vector2i(-speed, 0));
                break;
            case Keyboard::Up:
                move_cursor(Vector2i(0, -speed));
                break;
            case Keyboard::Down:
                move_cursor(Vector2i(0, speed));
                break;
            }
        }
    }
    fix_camera();
}

void EmbarkScene::render(DrawBuffer& db)
{
    static const Color white = {255, 255, 255};
    static const Color red = {255, 0, 0};
    static const Color black = {0, 0, 0};

    int w = _width;
    float dt = float(w) / _worldmap->size().x;

    draw(db, *_worldmap, {{1, 1}, {w, w}}, {{0, 0}, _worldmap->size()});
    draw(db, *_worldmap, {{w + 2, 1}, {w, w}}, {_camera, {w, w}});

    print(db, 1, 0, "Global", white, black);
    print(db, w + 2, 0, "Region", white, black);

    putchar_fast(db, (int)(1 + _cursor.x * dt), (int)(1 + _cursor.y * dt), 'X', {255, 0, 0}, white);
    putchar_fast(db, w + 2 + _cursor.x - _camera.x, 1 + _cursor.y - _camera.y, 'X', {255, 0, 0}, white);

    const auto& biome = Biome::from_id(_worldmap->biome(_cursor));
    print(db, 2 * w + 3, 1, biome.name, biome.no_embark ? red : white, black);
    printf(db, 2 * w + 3, 2, "Height:      %-4d", white, black, _worldmap->height(_cursor));
    printf(db, 2 * w + 3, 3, "Temperature: %-4d", white, black, _worldmap->temperature(_cursor));
    printf(db, 2 * w + 3, 4, "Rain:        %-4d", white, black, _worldmap->rain(_cursor));
    printf(db, 2 * w + 3, 5, "Drainage:    %-4d", white, black, _worldmap->drainage(_cursor));
}

void EmbarkScene::move_cursor(Vector2i delta)
{
    _cursor += delta;

    for (int i = 0; i < 2; ++i)
    {
        if (_cursor[i] < 0)
            _cursor[i] = 0;
        else if (_cursor[i] >= _worldmap->size()[i])
            _cursor[i] = _worldmap->size()[i] - 1;
    }
}

void EmbarkScene::fix_camera()
{
    int trigger = _width / 4;
    int rel_pos;

    if (trigger < 3)
        trigger = 3;

    for (int i = 0; i < 2; ++i)
    {
        rel_pos = _cursor[i] - _camera[i];
        if (rel_pos < trigger || rel_pos >= _width - trigger)
        {
            _camera[i] = _cursor[i] - _width / 2;
            if (_camera[i] < 0)
                _camera[i] = 0;
            else if (_camera[i] + _width > _worldmap->size()[i])
                _camera[i] = _worldmap->size()[i] - _width;
        }
    }
}

bool EmbarkScene::embark()
{
    const auto& biome = Biome::from_id(_worldmap->biome(_cursor));

    if (!biome.no_embark)
    {
        game().set_scene<IngameScene>(_world_id, _cursor.x + _worldmap->size().x * _cursor.y);
        return true;
    }
    return false;
}

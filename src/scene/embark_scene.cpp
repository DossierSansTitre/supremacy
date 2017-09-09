#include <scene/embark_scene.h>
#include <scene/load_world_selection_scene.h>
#include <serialize.h>
#include <engine/game.h>
#include <draw.h>

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
    w = game().draw_buffer().size().x / 2 - 4;
    h = game().draw_buffer().size().y - 2;

    if (w > h)
        _width = h;
    else
        _width = w;
}

void EmbarkScene::teardown()
{
    delete _worldmap;
}

void EmbarkScene::update()
{
    auto& kb = game().keyboard();

    if (kb.pressed(SDL_SCANCODE_ESCAPE))
    {
        game().set_scene<LoadWorldSelectionScene>();
    }

    int speed = 1;

    if (kb.down(SDL_SCANCODE_LSHIFT))
        speed = 10;
    if (kb.repeated(SDL_SCANCODE_RIGHT))
        move_cursor({speed, 0});
    if (kb.repeated(SDL_SCANCODE_LEFT))
        move_cursor({-speed, 0});
    if (kb.repeated(SDL_SCANCODE_UP))
        move_cursor({0, -speed});
    if (kb.repeated(SDL_SCANCODE_DOWN))
        move_cursor({0, speed});
    fix_camera();
}

void EmbarkScene::render(DrawBuffer& db)
{
    static const Color white = {255, 255, 255};
    static const Color black = {0, 0, 0};

    int w = _width;
    float dt = float(w) / _worldmap->size().x;

    draw(db, *_worldmap, {{1, 1}, {w, w}}, {{0, 0}, _worldmap->size()});
    draw(db, *_worldmap, {{w + 2, 1}, {w, w}}, {_camera, {w, w}});

    print(db, 1, 0, "Global", white, black);
    print(db, w + 2, 0, "Region", white, black);

    putchar_fast(db, 1 + _cursor.x * dt, 1 + _cursor.y * dt, 'X', {255, 0, 0}, white);
    putchar_fast(db, w + 2 + _cursor.x - _camera.x, 1 + _cursor.y - _camera.y, 'X', {255, 0, 0}, white);
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

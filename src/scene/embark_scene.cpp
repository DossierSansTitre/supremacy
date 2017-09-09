#include <scene/embark_scene.h>
#include <scene/load_world_selection_scene.h>
#include <serialize.h>
#include <engine/game.h>

EmbarkScene::EmbarkScene(u16 world_id)
: _world_id(world_id)
{

}

EmbarkScene::~EmbarkScene()
{

}

void EmbarkScene::setup()
{
    _worldmap = load_worldmap(_world_id);
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
}

void EmbarkScene::render(DrawBuffer& db)
{

}

#include <scene/embark_scene.h>
#include <scene/load_world_selection_scene.h>
#include <engine/game.h>

EmbarkScene::EmbarkScene()
{

}

EmbarkScene::~EmbarkScene()
{

}

void EmbarkScene::setup()
{

}

void EmbarkScene::teardown()
{

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

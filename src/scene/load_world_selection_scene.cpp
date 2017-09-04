#include <scene/load_world_selection_scene.h>
#include <scene/main_menu_scene.h>
#include <engine/game.h>
#include <dirent.h>
#include <util/file_path.h>
#include <log.h>

void LoadWorldSelectionScene::setup()
{
    static const char* prefix = "world_";

    const char* name;
    DIR* dir;
    dirent* ent;
    u16 world_id;

    _cursor = 0;
    dir = opendir(save_path("worlds"));
    while ((ent = readdir(dir)))
    {
        name = ent->d_name;
        if (strncmp(name, prefix, strlen(prefix)) != 0)
            continue;
        world_id = atol(name + strlen(prefix));
        log_line(LogLevel::Debug, "Found world %x", world_id);
        _worldmaps.push_back(world_id);
    }
    closedir(dir);
}

void LoadWorldSelectionScene::update()
{
    auto& kb = game().keyboard();

    if (kb.pressed(SDL_SCANCODE_ESCAPE))
        game().set_scene<MainMenuScene>();
    if (kb.repeated(SDL_SCANCODE_UP) && _cursor)
        _cursor--;
    if (kb.repeated(SDL_SCANCODE_DOWN) && (_cursor < _worldmaps.size() - 1))
        _cursor++;
}

void LoadWorldSelectionScene::render(DrawBuffer& db)
{
    static const Color selected = {255, 255, 128};
    static const Color unselected = {127, 127, 127};

    for (size_t i = 0; i < _worldmaps.size(); ++i)
    {
        printf(db, 5, 5 + i, "World %u", (i == _cursor ? selected : unselected), {0, 0, 0}, _worldmaps[i]);
    }
}

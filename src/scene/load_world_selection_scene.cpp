#include <scene/load_world_selection_scene.h>
#include <scene/main_menu_scene.h>
#include <scene/embark_scene.h>
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
        _worldmaps.push_back(world_id);
    }
    closedir(dir);
}

void LoadWorldSelectionScene::update()
{
    auto& kb = game().keyboard();

    if (kb.pressed(Keyboard::Escape))
        game().set_scene<MainMenuScene>();
    if (kb.repeated(Keyboard::Up) && _cursor)
        _cursor--;
    if (kb.repeated(Keyboard::Down) && (_cursor < _worldmaps.size() - 1))
        _cursor++;
    if (kb.repeated(Keyboard::Enter))
        game().set_scene<EmbarkScene>(_worldmaps[_cursor]);
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

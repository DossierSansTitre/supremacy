#include <scene/load_world_selection_scene.h>
#include <scene/main_menu_scene.h>
#include <scene/embark_scene.h>
#include <scene/ingame_scene.h>
#include <engine/game.h>
#include <util/save_helper.h>
#include <log.h>
#include <core/file_system.h>

void LoadWorldSelectionScene::setup()
{
    static const char* prefix = "world_";

    FileSystem::Directory* dir;
    const char* name;
    u16 world_id;

    _cursor = 0;
    dir = FileSystem::open_directory(save_path("worlds"));
    for (;;)
    {
        name = FileSystem::read_directory(dir);
        if (!name)
            break;
        if (strncmp(name, prefix, strlen(prefix)) != 0)
            continue;
        world_id = atol(name + strlen(prefix));
        _worldmaps.push_back(world_id);
    }
    FileSystem::close_directory(dir);
    sort(_worldmaps);
}

void LoadWorldSelectionScene::update()
{
    auto& input = game().input();
    InputEvent e;

    while (input.poll(e))
    {
        if (e.type == InputEventType::KeyDown)
        {
            switch (e.key.scancode)
            {
                case Keyboard::Escape:
                    game().set_scene<MainMenuScene>();
                    break;
                case Keyboard::Up:
                    if (_cursor)
                        _cursor--;
                    break;
                case Keyboard::Down:
                    if (_cursor < _worldmaps.size() - 1)
                        _cursor++;
                    break;
                case Keyboard::Enter:
                    submit();
                    return;
            }
        }
    }
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

void LoadWorldSelectionScene::submit()
{
    const char* fortress_path;
    u16 world_id;

    world_id = _worldmaps[_cursor];
    fortress_path = save_path_fortress(world_id);
    if (FileSystem::file_exists(fortress_path))
    {
        u32 region_id;
        std::ifstream stream;

        stream.open(fortress_path, std::ios::binary);
        stream.read((char*)&region_id, 4);
        stream.close();

        game().set_scene<IngameScene>(world_id, region_id);
    }
    else
    {
        game().set_scene<EmbarkScene>(world_id);
    }
}

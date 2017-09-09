#include <draw.h>
#include <worldmap.h>
#include <biome.h>
#include <math/algorithm.h>

void draw(DrawBuffer& db, const Worldmap& worldmap, Rect2i dst, Rect2i src)
{
    float dt;

    dt = src.size.x / dst.size.x;
    iterate(dst.size, [&] (Vector2i d) {
        Vector2i s;

        s = src.origin + d * dt;
        d += dst.origin;

        const auto& biome = Biome::from_id(worldmap.biome(s));
        putchar_fast(db, d.x, d.y, biome.symbol, biome.color, biome.color_bg);
    });
}

#include <material.h>
#include <item_id.h>

const Material Material::materials[] = {
    {{0, 0, 0}, {3, 3, 3}, false, ItemID::None},
    {{173, 107, 53}, {99, 58, 25}, true, ItemID::Dirt},
    {{216, 216, 216}, {165, 165, 165}, true, ItemID::Rock},
    {{204, 102, 0}, {153, 76, 0}, false, ItemID::Wood},
    {{144, 209, 92}, {80, 132, 38}, true, ItemID::Dirt}
};

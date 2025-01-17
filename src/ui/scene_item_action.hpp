#pragma once

#include <utility>

enum class SceneItemActionType
{
    Undefined = 0,
    Movement,
    Visibility,
    Removing,
};

struct SceneItemAction
{
    SceneItemActionType type;
    std::pair<int, int> initial_pos;
    std::pair<int, int> delta_coord;
    int id;
    bool is_visible;
};
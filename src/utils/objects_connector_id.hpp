#pragma once

#include <QString>

namespace gamma::utils {

class ObjectsConnectorID
{
public:
    ObjectsConnectorID() = default;

    static QString SHAPE_PROCESSED;
    static QString SCENE_ITEM_ACTION;
    static QString ITEM_VISIBILITY;
    static QString SCENE_ITEM_REMOVING;
};

}
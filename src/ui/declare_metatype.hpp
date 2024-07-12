#pragma once

#include "core/types/shape.hpp"

#include "scene.hpp"

#include <QMetaType>

#include <memory>

Q_DECLARE_METATYPE(std::shared_ptr<gamma::types::IShape>);
Q_DECLARE_METATYPE(SceneEllipseItem);
Q_DECLARE_METATYPE(SceneRectItem);
Q_DECLARE_METATYPE(SceneLineItem);
Q_DECLARE_METATYPE(ScenePolygonItem);
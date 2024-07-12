#include "model_item.hpp"

#include "core/log/log.hpp"

QString ShapeModelItem::get_data_str(int id) const noexcept
{
    switch(id)
    {
        case 0: return get_id_str();
        case 1: return get_type_str();
        case 2: return get_pos_str();
        case 3: return get_region_str();
        default:
        {
            GAMMA_LOG(L_ERROR, "Invalid id for ShapeModelItem");
            return QString("Invalid");
        }
    }
}

QString ShapeModelItem::get_id_str() const noexcept
{
    return QString::number(id);
}

QString ShapeModelItem::get_type_str() const noexcept
{
    if(!shape)
    {
        GAMMA_LOG(L_ERROR, "Invalid shape in ShapeModelItem");
        return QString("NULL");
    }

    switch(shape->type())
    {
        case gamma::types::IShape::ShapeType::Ellipse: return QString("Ellipse");
        case gamma::types::IShape::ShapeType::Line: return QString("Line");
        case gamma::types::IShape::ShapeType::Rect: return QString("Rect");
        case gamma::types::IShape::ShapeType::Triangle: return QString("Triangle");
        default:
        {
            GAMMA_LOG(L_ERROR, "Invalid shape type in ShapeModelItem");
            return QString("Invalid");
        }
    }
}

QString ShapeModelItem::get_pos_str() const noexcept
{
    if(!shape)
    {
        GAMMA_LOG(L_ERROR, "Invalid shape in ShapeModelItem");
        return QString("NULL");
    }

    const auto [x, y] = shape->coord();
    return QString::number(x) + QString("px, ") + QString::number(y) + QString("px");
}

QString ShapeModelItem::get_region_str() const noexcept
{
     if(!shape)
    {
        GAMMA_LOG(L_ERROR, "Invalid shape in ShapeModelItem");
        return QString("NULL");
    }

    const auto [w, h] = shape->bbox();
    return QString::number(w) + QString("px, ") + QString::number(h) + QString("px");
}
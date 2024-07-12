#pragma once

#include "core/types/shape.hpp"

#include <QString>

#include <memory>

struct ShapeModelItem
{
    std::shared_ptr<gamma::types::IShape> shape;
    bool is_visible{true};

    QString get_data_str(int id) const noexcept;

private:
    QString get_id_str() const noexcept;
    QString get_type_str() const noexcept;
    QString get_pos_str() const noexcept;
    QString get_region_str() const noexcept;
};
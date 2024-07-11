#pragma once

#include <string>

#include <fmt/format.h>

namespace gamma::types {

struct IShape
{
public:
    enum class ShapeType { Undefined = 0, Rect, Ellipse, Triangle, Line };

public:
    virtual ~IShape() = default;

    virtual ShapeType type() const noexcept = 0;
};

struct BaseShape : public IShape
{
    std::string hex_color_str;
};

struct Rect : public BaseShape
{
    int x;
    int y;
    int width;
    int height;

    ShapeType type() const noexcept override { return ShapeType::Rect; };
};

struct Ellipse : public BaseShape
{
    int x;
    int y;
    int r1;
    int r2;

    ShapeType type() const noexcept override { return ShapeType::Ellipse; };
};

struct Triangle : public BaseShape
{
    int x1, y1, x2, y2, x3, y3;

    ShapeType type() const noexcept override { return ShapeType::Triangle; };
};

struct Line : public BaseShape
{
    int x1, y1, x2, y2;

    ShapeType type() const noexcept override { return ShapeType::Line; };
};

}

template <>
struct fmt::formatter<gamma::types::Ellipse> : fmt::formatter<string_view>
{
    template <typename FormatContext>
    auto format(const gamma::types::Ellipse& shape, FormatContext& ctx)
    {
        return fmt::format_to(ctx.out(), "(x: {}; y: {}; r1: {}; r2: {}; color: {})", shape.x, shape.y, shape.r1, shape.r2, shape.hex_color_str);
    }
};

template <>
struct fmt::formatter<gamma::types::Line> : fmt::formatter<string_view>
{
    template <typename FormatContext>
    auto format(const gamma::types::Line& shape, FormatContext& ctx)
    {
        return fmt::format_to(ctx.out(), "(x1: {}; y1: {}; x2: {}; y2: {}; color: {})", shape.x1, shape.y1, shape.x2, shape.y2, shape.hex_color_str);
    }
};

template <>
struct fmt::formatter<gamma::types::Rect> : fmt::formatter<string_view>
{
    template <typename FormatContext>
    auto format(const gamma::types::Rect& shape, FormatContext& ctx)
    {
        return fmt::format_to(ctx.out(), "(x: {}; y: {}; w: {}; h: {}; color: {})", shape.x, shape.y, shape.width, shape.height, shape.hex_color_str);
    }
};

template <>
struct fmt::formatter<gamma::types::Triangle> : fmt::formatter<string_view>
{
    template <typename FormatContext>
    auto format(const gamma::types::Triangle& shape, FormatContext& ctx)
    {
        return fmt::format_to(ctx.out(), "(x1: {}; y1: {}; x2: {}; y2: {}; x3: {}; y3: {}; color: {})", shape.x1, shape.y1, shape.x2, shape.y2, shape.x3, shape.y3, shape.hex_color_str);
    }
};

template <>
struct fmt::formatter<std::shared_ptr<gamma::types::IShape>> : fmt::formatter<string_view>
{
    template <typename FormatContext>
    auto format(const std::shared_ptr<gamma::types::IShape>& shape, FormatContext& ctx)
    {
        if(!shape)
            return fmt::format_to(ctx.out(), "Null shape");

        switch(shape->type())
        {
            case gamma::types::IShape::ShapeType::Ellipse:
                return fmt::format_to(ctx.out(), "Ellipse {}", *(std::dynamic_pointer_cast<gamma::types::Ellipse>(shape)));
            case gamma::types::IShape::ShapeType::Line:
                return fmt::format_to(ctx.out(), "Line {}", *(std::dynamic_pointer_cast<gamma::types::Line>(shape)));
            case gamma::types::IShape::ShapeType::Rect:
                return fmt::format_to(ctx.out(), "Rect {}", *(std::dynamic_pointer_cast<gamma::types::Rect>(shape)));
            case gamma::types::IShape::ShapeType::Triangle:
                return fmt::format_to(ctx.out(), "Triangle {}", *(std::dynamic_pointer_cast<gamma::types::Triangle>(shape)));
            default:
                return fmt::format_to(ctx.out(), "Invalid shape");
        }
    }
};
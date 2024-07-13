#include "shape.hpp"

namespace gamma::types {

std::pair<int, int> Ellipse::coord() const noexcept
{
    return { x - r1, y - r2 };
}

std::pair<int, int> Triangle::coord() const noexcept
{
    int x_min = std::min(x1, x2) < x3 ? std::min(x1, x2) : x3;
    int y_min = std::min(y1, y2) < y3 ? std::min(y1, y2) : y3;

    return {x_min, y_min};
}

std::pair<int, int> Line::coord() const noexcept
{
    return { std::min(x1, x2), std::min(y1, y2)};
}

std::pair<int, int> Rect::bbox() const noexcept
{
    return {width, height};
}

std::pair<int, int> Line::bbox() const noexcept
{
    return {std::abs(x1 - x2), std::abs(y1 - y2)};
}

std::pair<int, int> Triangle::bbox() const noexcept
{
    int x_min = std::min(x1, x2) < x3 ? std::min(x1, x2) : x3;
    int y_min = std::min(y1, y2) < y3 ? std::min(y1, y2) : y3;
    int x_max = std::max(x1, x2) > x3 ? std::max(x1, x2) : x3;
    int y_max = std::max(y1, y2) > y3 ? std::max(y1, y2) : y3;

    return {std::abs(x_max - x_min), std::abs(y_max - y_min)};
}

std::pair<int, int> Ellipse::bbox() const noexcept
{
    return {2*r1, 2*r2};
}

void Rect::move(int dx, int dy) noexcept
{
    x += dx;
    y += dy;
}

void Line::move(int dx, int dy) noexcept
{
    x1 += dx;
    y1 += dy;
    x2 += dx;
    y2 += dy;
}

void Ellipse::move(int dx, int dy) noexcept
{
    x += dx;
    y += dy;
}

void Triangle::move(int dx, int dy) noexcept
{
    x1 += dx;
    y1 += dy;
    x2 += dx;
    y2 += dy;
    x3 += dx;
    y3 += dy;
}

}
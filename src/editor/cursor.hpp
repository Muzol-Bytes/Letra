#ifndef _CURSOR_HPP_
#define _CURSOR_HPP_

#include <dwrite.h>

#include "buffer.hpp"
#include "../graphics/rectangle.hpp"
#include "../graphics/renderer.hpp"

class Cursor
{
public:
    enum Direction
    {
        RIGHT,
        LEFT,
        UP,
        DOWN
    };

    Cursor(const DWRITE_HIT_TEST_METRICS htm);

    Rect getBody() const;

    float getRow() const;
    
    float getCol() const;

    void move(const Direction dir, Buffer& buffer);

    void setPosition(const float row, const float col);

    void draw(Renderer& render);

private:
    Rect m_body;

    float m_row;
    float m_col;
    float m_width;
    float m_height;
};

#endif // _CURSOR_HPP_

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

    void setPosition(const float row, const float col);

    void setColor(const uint32_t color);

    void move(const float row_offset, const float col_offset);

    void move(const WPARAM wParam, Buffer& buffer);


    void draw(Renderer& render);

private:
    Rect m_body;

    float m_row;
    float m_col;
    float m_width;
    float m_height;
};

#endif // _CURSOR_HPP_

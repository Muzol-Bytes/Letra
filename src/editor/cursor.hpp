#ifndef _CURSOR_HPP_
#define _CURSOR_HPP_

#include <dwrite.h>

#include "buffer.hpp"
#include "editor.hpp"
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

    float getRowOffset() const;
    
    float getColOffset() const;

    size_t getRowPadding() const;

    void setRowOffset(const float row_offset);

    void setColOffset(const float col_offset);

    void setRowPadding(size_t& row_padding);

    void setColor(const uint32_t color);

    void setPosition(const float row, const float col, Editor& editor);

    void setPosition(const float row, const float col);

    void move(const float row_offset, const float col_offset);

    void handleCharInput(const WPARAM wParam, Buffer& buffer, Editor& editor);

    void handleKeyInput(const WPARAM wParam, Buffer& buffer, Editor& editor);

    void draw(Renderer& render);

private:
    Rect m_body;

    float m_row;
    float m_col;
    float m_row_offset;
    float m_col_offset;
    float m_width;
    float m_height;
    size_t *p_row_padding;
};

#endif // _CURSOR_HPP_

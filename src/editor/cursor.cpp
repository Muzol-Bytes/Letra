#include "cursor.hpp"

#include <iostream>

Cursor::Cursor(const  DWRITE_HIT_TEST_METRICS htm)
    : m_body(10.0f, 10.0f)
{
   m_row    = 0.0f;
   m_col    = 0.0f;
   m_width  = htm.width;
   m_height = htm.height;

   m_body.setSize(m_width, m_height);
   m_body.setColor(0xfe8019);
}

Rect Cursor::getBody() const
{
    return m_body;
}

float Cursor::getRow() const
{
    return m_row;
}

float Cursor::getCol() const
{
    return m_col;
}

void Cursor::setColor(const uint32_t color)
{
    m_body.setColor(color);
}

void Cursor::setPosition(const float row, const float col, Editor& editor)
{
    m_row = row;
    m_col = col;

    m_body.setPosition(m_row * m_width, m_col * m_height);
}

void Cursor::setPosition(const float row, const float col)
{
    m_row = row;
    m_col = col;

    m_body.setPosition(m_row * m_width, m_col * m_height);
}

void Cursor::move(const float row_offset, const float col_offset)
{
    m_row += row_offset;
    m_col += col_offset;

}

void Cursor::move(const WPARAM wParam, Buffer& buffer, Editor& editor)
{
    switch (wParam)
    {
        case VK_RIGHT:
        {
            if (m_row > editor.max_row - 1)
            {
                editor.row_offset += 1;
                break;
            }
            if (m_row + 1 > buffer.getLineLengthAt(m_col) - 1)
                break;
            m_row += 1;
        }
        break;
        case VK_LEFT:
        {
            if (m_row > editor.max_row - 1)
            {
                editor.row_offset -= 1;
                break;
            }

            if (m_row > 0)
                m_row -= 1;
        }
        break;
        case VK_UP:
        {
            if (m_col < 1 && editor.col_offset != 0)
            {
                editor.col_offset -= 1;
                break;
            }
            if (m_col - 1 >= 0)
            {
                if (buffer.getLineLengthAt(m_col - 1) <= 1)
                {
                    m_row = 0;
                    m_col -= 1;
                    break;
                }
                else if (m_row > buffer.getLineLengthAt(m_col - 1) - 1)
                {
                    m_row = buffer.getLineLengthAt(m_col - 1) - 1;
                    m_col -= 1;
                }
                else
                {
                    m_col -= 1;
                }
            }
        }
        break;
        case VK_DOWN:
        {
            /// TODO: cursor goes down eternaly
            if (m_col + 1 > editor.max_col - 2)
            {
                editor.col_offset += 1;
                break;
            }

            if (m_col + 1 < buffer.getLineNum())
            {
                if (buffer.getLineLengthAt(m_col + 1) <= 1)
                {
                    m_row = 0;
                    m_col += 1;
                }
                else if (m_row + 1 > buffer.getLineLengthAt(m_col + 1) - 1)
                {
                    m_row = buffer.getLineLengthAt(m_col + 1) - 1;
                    m_col += 1;
                }
                else
                    m_col += 1;
            }
        }
        break;
    };

    m_body.setPosition(m_row * m_width, m_col * m_height);
}

void Cursor::draw(Renderer& render)
{
    render.draw(&m_body);
}

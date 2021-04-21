#include "cursor.hpp"

#include <cstdio>

Cursor::Cursor(const  DWRITE_HIT_TEST_METRICS htm)
    : m_body(10.0f, 10.0f)
{
    m_row        = 0.0f;
    m_col        = 0.0f;
    m_row_offset = 0.0f;
    m_col_offset = 0.0f;
    m_width      = htm.width;
    m_height     = htm.height;
    p_row_padding = NULL;

    m_body.setSize(m_width, m_height);
    m_body.setColor(0xfe8019);
}

Rect Cursor::getBody() const
{
    return m_body;
}

float Cursor::getRow() const
{
    return (m_row + m_row_offset);
}

float Cursor::getCol() const
{
    return m_col + m_col_offset;
}

float Cursor::getRowOffset() const
{
    return m_row_offset;
}

float Cursor::getColOffset() const
{
    return m_col_offset;
}

size_t Cursor::getRowPadding() const
{
    return *(p_row_padding);
}

void Cursor::setRowOffset(const float row_offset)
{
    m_row_offset = row_offset;
}

void Cursor::setColOffset(const float col_offset)
{
    m_col_offset = col_offset;
}

void Cursor::setRowPadding(size_t& row_padding)
{
    p_row_padding = &row_padding;
}

void Cursor::setColor(const uint32_t color)
{
    m_body.setColor(color);
}

void Cursor::setPosition(const float row, const float col, Editor& editor)
{
    m_row = row;
    m_col = col;

}

void Cursor::setPosition(const float row, const float col)
{
    m_row = row;
    m_col = col;
}

void Cursor::move(const float row_offset, const float col_offset)
{
    m_row += row_offset;
    m_col += col_offset;

}

void Cursor::handleCharInput(const WPARAM wParam, Buffer& buffer, Editor& editor)
{
    switch (wParam)
    {
        case 0x08: // Backspace
        {
            if (m_row > 0)
            {
                move(-1, 0);
            }
        }
        break;
        case 0x09: // Tab
        {
            setPosition(m_row + 4, m_col, editor);
        }
        break;
        case 0x0D: // Enter
        {
            setPosition(0, m_col + 1);
            if (m_col - 1 > editor.max_col - 3)
            {
                m_col_offset += 1;
                m_col -= 1;
                break;
            }
        }
        break;
        case 0x13: // Ctrl-S
        {
            if (!editor.is_file_new)
                break;
        }
        case 0x07: // Ctrl-G
        case 0x10: // Ctrl-P
        {
            setPosition(0, editor.max_col - 1);
            setColor(0x151515);
        }
        break;
        default:
        {
            move(1, 0);
        }
        break;
    }
}

void Cursor::handleKeyInput(const WPARAM wParam, Buffer& buffer, Editor& editor)
{
    switch (wParam)
    {
        case VK_RIGHT:
        {
            if (m_row > editor.max_row - 1)
            {
                m_row_offset += 1;
                break;
            }
            if (m_row + 1 > buffer.getLineLengthAt(getCol()) - 1)
                break;
            m_row += 1;
        }
        break;
        case VK_LEFT:
        {
            if (m_row > editor.max_row - 1)
            {
                m_row_offset -= 1;
                break;
            }

            if (m_row > 0)
                m_row -= 1;
        }
        break;
        case VK_UP:
        {
            if (m_col < 1 && m_col_offset != 0)
            {
                m_col_offset -= 1;
                break;
            }
            if (m_col - 1 >= 0)
            {
                if (buffer.getLineLengthAt(getCol() - 1) <= 1)
                {
                    m_row = 0;
                    m_col -= 1;
                    break;
                }
                else if (m_row > buffer.getLineLengthAt(getCol() - 1) - 1)
                {
                    m_row = buffer.getLineLengthAt(getCol() - 1) - 1;
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
            /// Avoids that cursor go down eternily
            if (getCol() + 1 > buffer.getLineNum() - 1)
                return;

            if (m_col + 1 > editor.max_col - 2)
            {
                m_col_offset += 1;
                break;
            }

            if (m_col + 1 < buffer.getLineNum())
            {
                if (buffer.getLineLengthAt(getCol() + 1) <= 1)
                {
                    m_row = 0;
                    m_col += 1;
                }
                else if (m_row + 1 > buffer.getLineLengthAt(getCol() + 1) - 1)
                {
                    m_row = buffer.getLineLengthAt(getCol() + 1) - 1;
                    m_col += 1;
                }
                else
                    m_col += 1;
            }
        }
        break;
    };

}

void Cursor::draw(Renderer& render)
{
    m_body.setPosition((m_row + *(p_row_padding)) * m_width, m_col * m_height);
    render.draw(&m_body);
}

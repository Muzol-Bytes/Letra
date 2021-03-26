#ifndef _EDITOR_HPP_
#define _EDITOR_HPP_

#include <d2d1.h>

#include "ui/text.hpp"
#include "buffer.hpp"

struct Editor
{
    Editor();

    void init(ID2D1HwndRenderTarget *render_target);

    void updateCursor(D2D1_RECT_F& cursor);

    void norm_cursor();

    void moveCursor(const float x_offset, const float y_offset);
    
    void setCursorPosition(const float row, const float col);

    float cursor_width;
    float cursor_height;
    float cursor_x;
    float cursor_y;
    float cursor_row;
    float cursor_col;

    Text text;           // ID2D1WriteTextLayout wrapper
    Buffer buffer;       // Buffer to be renderer to the screen
};

#endif // _EDITOR_HPP_

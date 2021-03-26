#include "editor.hpp"

#include "file.hpp"

Editor::Editor()
    : text()
    , buffer()
{
    cursor_width  = 0.0f;
    cursor_height = 0.0f;
    cursor_x      = 0.0f;
    cursor_y      = 0.0f;
    cursor_row    = 0.0f;
    cursor_col    = 0.0f;
}

void Editor::init(ID2D1HwndRenderTarget *render_target)
{
    text.createDeviceIndependentResources(render_target);
    text.setString(buffer.getContent());
    DWRITE_HIT_TEST_METRICS htm = text.getCharacterMetricsAt(1, 1);

    cursor_x      = htm.left;
    cursor_y      = htm.top;
    cursor_width  = htm.width;
    cursor_height = htm.height;
}

void Editor::updateCursor(D2D1_RECT_F& cursor)
{
    /// Math XD
    cursor.left   = cursor_row * cursor_width;
    cursor.top    = (cursor_col * cursor_height);
    cursor.right  = cursor.left + cursor_width;
    cursor.bottom = (cursor_col * cursor_height) + cursor_height;
}

void Editor::norm_cursor()
{
    DWRITE_HIT_TEST_METRICS htm = text.getCharacterMetricsAt((UINT32)cursor_row);
    /* DWRITE_HIT_TEST_METRICS htm = text.getCharacterMetricsAt(cursor_width * cursor_row, cursor_height * cursor_col); */
    

    cursor_x      = htm.left;
    cursor_y      = htm.top;
    cursor_width  = htm.width;
    cursor_height = htm.height;
}

void Editor::moveCursor(const float x_offset, const float y_offset)
{
    cursor_row += x_offset;
    cursor_col += y_offset;

    /* norm_cursor(); */
}

void Editor::setCursorPosition(const float row, const float col)
{
    cursor_row = row;
    cursor_col = col;
    /* norm_cursor(); */
}

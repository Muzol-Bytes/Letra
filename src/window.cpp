#include "window.hpp"

#include "graphics/rectangle.hpp"

#include <iostream>

#include <string>

template <class T> static void SafeRelease(T **ppT)
{
    if (*ppT)
    {
        (*ppT)->Release();
        *ppT = NULL;
    }
}

Window::Window (const std::wstring &filename)
    : render()
    , m_file(filename)
    , editor()
    , p_brush(NULL)
{
    cursor  = D2D1::RectF(0.0f, 0.0f, 10.0f, 25.0f);

    editor.buffer.setBuffer(m_file.read());
}

void Window::init()
{
    render.createGraphicsResources(m_hwnd);
    render.render_target->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &p_brush);

    editor.init(render.render_target);
}

LPCSTR Window::getClassName() const
{
    return "Letra Window Class";
}

LRESULT Window::handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CREATE:
        {
            init();

            cursor.left   = editor.cursor_x;
            cursor.top    = editor.cursor_y;
            cursor.right  = cursor.left + editor.cursor_width;
            cursor.bottom = cursor.top  + editor.cursor_height;
        }
            return 0;
        case WM_KEYDOWN:
            {
                switch(wParam)
                {
                    case VK_LEFT:
                        {
                            if (editor.cursor_row > 0)
                                editor.moveCursor(-1, 0);
                        }
                        break;
                    case VK_RIGHT:
                        {
                            if (editor.cursor_row + 1 > editor.buffer.getLineLengthAt(editor.cursor_col) - 1)
                                break;
                            editor.moveCursor(1, 0);
                        }
                        break;
                    case VK_DOWN:
                        {
                            /* std::cout << editor.buffer.getLineLengthAt(editor.cursor_col) << '\n'; */
                            if (editor.buffer.getLineLengthAt(editor.cursor_col + 1) <= 1)
                            {
                                editor.setCursorPosition(0, editor.cursor_col + 1); 
                                break;
                            }
                            if (editor.cursor_col + 1 < editor.buffer.getLineNum())
                            {
                                if (editor.cursor_row + 2 > editor.buffer.getLineLengthAt(editor.cursor_col + 1) - 1 && editor.buffer.getLineLengthAt(editor.cursor_col + 1) != 1)
                                    editor.setCursorPosition(editor.buffer.getLineLengthAt(editor.cursor_col + 1) - 2, editor.cursor_col + 1);
                                else
                                editor.moveCursor(0, 1);
                            }
                        }
                        break;
                    case VK_UP:
                        {
                            if (editor.cursor_col > 0 && editor.buffer.getLineLengthAt(editor.cursor_col - 1) <= 1)
                            {
                                editor.setCursorPosition(0, editor.cursor_col - 1); 
                                break;
                            }
                            if (editor.cursor_col > 0)
                            {
                                editor.moveCursor(0, -1);
                            }
                        }
                        break;
                    default: break;
                }
            }
            return 0;
        case WM_CHAR:
        {
            switch ((wchar_t)wParam)
            {
                case 0x08: // Backspace
                    if (editor.cursor_row > 0)
                    {
                        editor.buffer.deleteaAt(editor.cursor_col, --editor.cursor_row, 1);
                    }
                    break;
                case 0x09: // Replace tab to space
                    editor.buffer.insertAt(editor.cursor_col, editor.cursor_row, 4, L' ');
                    editor.moveCursor(4, 0);
                    break;
                case 0x0D:
                    {
                        std::wstring temp_str = editor.buffer.getLine(editor.cursor_col);
                        std::wstring new_str = L"";

                        for (size_t i = editor.cursor_row; i < temp_str.size(); i++)
                        {
                            new_str += temp_str[i];
                        }
                        editor.buffer.deleteaAt(editor.cursor_col, editor.cursor_row, new_str.size() - 1);
                        editor.buffer.append(new_str, ++editor.cursor_col);
                        editor.setCursorPosition(0, editor.cursor_col);
                    }
                    break;
                case 0x1B:
                    DestroyWindow(m_hwnd);
                break;
                case 0x13:
                    m_file.write(editor.buffer.getContent()); 
                break;
                default:
                    editor.cursor_row++;
                    editor.buffer.insertAt(editor.cursor_col, editor.cursor_row - 1, 1, (wchar_t)wParam);
                    /* printf("%d\n", (int)wParam); */
                break;
            }
                editor.text.setString(editor.buffer.getContent());
        }
            return 0;
        case WM_DESTROY:
            SafeRelease(&p_brush);
            PostQuitMessage(0);
            return 0;

        case WM_PAINT:
            {
                onPaint();
            }
            return 0;
        default:
            return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
    }
}

void Window::onPaint()
{
    editor.updateCursor(cursor);
    HRESULT hr = S_OK;
    PAINTSTRUCT ps;
    BeginPaint(m_hwnd, &ps);
    render.render_target->BeginDraw();

    render.render_target->Clear(D2D1::ColorF(0x15 / 255.0f, 0x15 / 255.0f, 0x15 / 255.0f));
    render.render_target->DrawRectangle(cursor, p_brush);
    render.draw(&editor.text);

    hr = render.render_target->EndDraw();

    EndPaint(m_hwnd, &ps);
}

void Window::show()
{ 
    ShowWindow(m_hwnd, SW_SHOWDEFAULT);

    MSG msg = { 0 }; 
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        InvalidateRect(m_hwnd, NULL, false);
    }
}


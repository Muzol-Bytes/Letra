#include "application.hpp"

#include "log/log.hpp"

const static uint16_t SCREEN_WIDTH  = 1080;
const static uint16_t SCREEN_HEIGHT = 720;

Application::Application(const std::vector<std::wstring> argv)
    : window()
    , render()
    , m_file(argv[0])
    , editor()
    , p_brush(NULL)
{
    if (!window.create("Win32", WS_OVERLAPPEDWINDOW, 0, SCREEN_WIDTH, SCREEN_HEIGHT))
        errorExit("Create Window: ");
    
    render.createGraphicsResources(window.getWindowHandler());
    render.render_target->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &p_brush);

    editor.buffer.setBuffer(m_file.read());
    editor.init(render.render_target);

    cursor.left   = editor.cursor_x;
    cursor.top    = editor.cursor_y;
    cursor.right  = cursor.left + editor.cursor_width;
    cursor.bottom = cursor.top  + editor.cursor_height;
}

template <class T> static void SafeRelease(T **ppT)
{
    if (*ppT)
    {
        (*ppT)->Release();
        *ppT = NULL;
    }
}

Application::~Application()
{
    SafeRelease(&p_brush);
}

void Application::run()
{
    window.show();

    MSG msg = { 0 }; 
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);

        handleInput(msg);
        update();

        DispatchMessage(&msg);
        InvalidateRect(window.getWindowHandler(), NULL, false);
    }
}

void Application::update()
{
}

void Application::handleInput(const MSG& msg)
{
    /// NOTE: lParam and msg.wParam are inside the msg struct
    switch (msg.message)
    {
        case WM_KEYDOWN:
            {
                switch(msg.wParam)
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
            break;
        case WM_CHAR:
        {
            switch ((wchar_t)msg.wParam)
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
                    window.destroy();
                break;
                case 0x13:
                    m_file.write(editor.buffer.getContent()); 
                break;
                default:
                    editor.cursor_row++;
                    editor.buffer.insertAt(editor.cursor_col, editor.cursor_row - 1, 1, (wchar_t)msg.wParam);
                    /* printf("%d\n", (int)msg.wParam); */
                break;
            }
            editor.text.setString(editor.buffer.getContent());
        }
            break;

        case WM_PAINT:
        {
            onPaint();
        }
            break;
        default: break;
    }
}

void Application::onPaint()
{
    editor.updateCursor(cursor);
    HRESULT hr = S_OK;
    PAINTSTRUCT ps;
    BeginPaint(window.getWindowHandler(), &ps);
    render.render_target->BeginDraw();

    render.render_target->Clear(D2D1::ColorF(0x15 / 255.0f, 0x15 / 255.0f, 0x15 / 255.0f));
    render.render_target->DrawRectangle(cursor, p_brush);
    render.draw(&editor.text);

    hr = render.render_target->EndDraw();

    EndPaint(window.getWindowHandler(), &ps);
}

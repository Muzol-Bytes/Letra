#include "application.hpp"

#include "log/log.hpp"

#include <iostream>

const static uint16_t SCREEN_WIDTH  = 1080;
const static uint16_t SCREEN_HEIGHT = 720;

Application::Application(const std::vector<std::wstring> argv)
    : window("Letra", SCREEN_WIDTH, SCREEN_HEIGHT)
    , render(window.getWindowHandler())
    , m_file(argv[0])
    , m_buffer(m_file.read())
    , m_text(m_buffer.getContent(), render.render_target)
    , m_cursor(m_text.getCharacterMetricsAt(1, 1))
    , m_command_prompt(0.0f, 0.0f, render.render_target)
    , p_brush(NULL)
{
    render.render_target->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &p_brush);
    m_command_prompt.setWidth(SCREEN_WIDTH);
    
    /// CommandPrompt config
    D2D1_SIZE_F size = render.getSize();
    float char_size = m_text.getCharacterMetricsAt(1, 1).height;
    m_command_prompt.setPosition(0.0f, size.height - char_size);
    /// ---------------------------------------------------------
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
    if (m_command_prompt.getInput())
    {
        m_command_prompt.handleInput(msg.wParam);
    }

    /// NOTE: lParam and msg.wParam are inside the msg struct
    switch (msg.message)
    {
        case WM_KEYDOWN:
            {
                switch(msg.wParam)
                {
                    case VK_LEFT:
                        {
                            m_cursor.move(Cursor::LEFT, m_buffer);
                        }
                        break;
                    case VK_RIGHT:
                        {
                            m_cursor.move(Cursor::RIGHT, m_buffer);
                        }
                        break;
                    case VK_DOWN:
                        {
                            m_cursor.move(Cursor::DOWN, m_buffer);
                        }
                        break;
                    case VK_UP:
                        {
                            m_cursor.move(Cursor::UP, m_buffer);
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
                    if (m_cursor.getRow() > 0)
                    {
                        m_cursor.move(Cursor::LEFT, m_buffer);
                        m_buffer.deleteaAt(m_cursor.getCol(), m_cursor.getRow(), 1);
                    }
                    break;
                case 0x09: // Replace tab to space
                    {
                        m_buffer.insertAt(m_cursor.getCol(), m_cursor.getRow(), 4, L' ');
                        m_cursor.setPosition(m_cursor.getRow() + 4, m_cursor.getCol());
                    }
                    break;
                case 0x0D:
                    {
                        std::wstring temp_str = m_buffer.getLine(m_cursor.getCol());
                        std::wstring new_str = L"";

                        for (size_t i = m_cursor.getRow(); i < temp_str.size(); i++)
                        {
                            new_str += temp_str[i];
                        }
                        m_buffer.deleteaAt(m_cursor.getCol(), m_cursor.getRow(), new_str.size() - 1);
                        m_buffer.append(new_str, m_cursor.getCol() + 1);
                        m_cursor.setPosition(0, m_cursor.getCol() + 1);
                    }
                    break;
                case 0x1B:
                    window.destroy();
                break;
                case 0x13: // Ctrl-S
                    m_file.write(m_buffer.getContent()); 
                    m_command_prompt.setString(L"File Saved!");
                break;
                default:
                {
                    m_cursor.setPosition(m_cursor.getRow() + 1, m_cursor.getCol());

                    if (m_cursor.getRow() == 0)
                    {
                        m_buffer.insertAt(m_cursor.getCol(), m_cursor.getRow(), 1, (wchar_t)msg.wParam);
                    }
                    else
                    {
                        m_buffer.insertAt(m_cursor.getCol(), m_cursor.getRow() - 1, 1, (wchar_t)msg.wParam);
                    }
                }
                    /* printf("%d\n", (int)msg.wParam); */
                break;
            }
            m_text.setString(m_buffer.getContent());
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
    HRESULT hr = S_OK;
    PAINTSTRUCT ps;
    BeginPaint(window.getWindowHandler(), &ps);
    render.render_target->BeginDraw();

    render.render_target->Clear(D2D1::ColorF(0x15 / 255.0f, 0x15 / 255.0f, 0x15 / 255.0f));
    m_cursor.draw(render);
    render.draw(&m_text);
    m_command_prompt.draw(render);

    hr = render.render_target->EndDraw();

    EndPaint(window.getWindowHandler(), &ps);
}

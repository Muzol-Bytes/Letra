#include "application.hpp"

#include "log/log.hpp"

#include <cstdio>

const static uint16_t SCREEN_WIDTH  = 1080;
const static uint16_t SCREEN_HEIGHT = 720;

Application::Application(const std::vector<std::wstring> argv)
    : m_window("Letra", SCREEN_WIDTH, SCREEN_HEIGHT)
    , m_render(m_window.getWindowHandler())
    , m_file(argv[0])
    , m_buffer(m_file.read())
    , m_text(m_buffer, m_render.render_target) 
    , m_cursor(m_text.getCharacterMetricsAt(1, 1))
    , m_command_prompt(0.0f, 0.0f, m_render.render_target)
    , p_brush(NULL)
{
    m_render.render_target->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &p_brush);
    m_command_prompt.setWidth(SCREEN_WIDTH);

    /// Cursor
    m_cursor.setRowPadding(m_buffer.getRowPadding());

    /// Editor
    D2D1_SIZE_F size = m_render.getSize();

    DWRITE_HIT_TEST_METRICS char_size = m_text.getCharacterMetricsAt(1, 1);
    m_editor.row_padding = &m_buffer.getRowPadding(); 
    m_editor.row_offset  = 0.0f;
    m_editor.col_offset  = 0.0f;
    m_editor.max_row     = size.width / char_size.width;
    m_editor.max_col     = size.height / char_size.height;
    m_editor.is_file_modified = false;
    m_editor.is_file_new = (m_file.getFilename().size() == 0);
    ///
    
    /// CommandPrompt config
    m_command_prompt.setPosition(0.0f, size.height - char_size.height);
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
    m_window.show();

    MSG msg = { 0 }; 
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);

        handleInput(msg);
        update();

        DispatchMessage(&msg);
        InvalidateRect(m_window.getWindowHandler(), NULL, false);
    }
}

void Application::update()
{
    if (m_text.update())
    {
        m_text.setString(m_buffer.getLines(m_cursor.getColOffset(),
                         m_cursor.getColOffset() + (m_editor.max_col - 1)));
    }
}

void Application::handleInput(const MSG& msg)
{
    /// NOTE: lParam and msg.wParam are inside the msg struct
    switch (msg.message)
    {
        case WM_KEYDOWN:
        {
            if (!m_command_prompt.getInput())
            {
                m_cursor.handleKeyInput(msg.wParam, m_buffer, m_editor);
            } 

            /// if scrolling down or up update the text
            if (m_cursor.getCol() + 1 > m_editor.max_col - 2 || m_cursor.getCol() - 1 < 0)
            {
                m_text.updateText();
            }
        }
            break;
        case WM_CHAR:
        {
            if (m_command_prompt.getInput())
            {
                bool done = m_command_prompt.handleInput(msg.wParam, m_cursor);
                if (done)
                {
                    m_cursor.setColor(0xfe8019);
                    m_cursor.setPosition(0, 0, m_editor);
                    switch (m_command_prompt.getCmdType())
                    {
                        case CommandPrompt::OPEN_FILE:
                        {
                            m_file.setFileName(m_command_prompt.getContent());
                            if (m_file.exist())
                            {
                                m_cursor.setColOffset(0);
                                m_buffer.setBuffer(m_file.read());
                                m_text.updateText();
                            }
                            else
                            {
                                m_command_prompt.setString(L"Couldn't open File!");
                            }
                        }
                            break;
                        case CommandPrompt::SAVE_FILE_AS:
                        {
                            m_file.setFileName(m_command_prompt.getContent());
                            m_file.write(m_buffer.getContent());
                            m_command_prompt.setString(L"File Saved!");
                        }
                            break;
                        case CommandPrompt::GOTO_LINE:
                        {
                            size_t line_num = std::stoi(m_command_prompt.getContent()); 
                            if (line_num == 0) break;
                            if (line_num > m_buffer.getLineNum() &&
                                m_buffer.getLineNum() < m_editor.max_col)
                            {
                                m_cursor.setPosition(0, m_buffer.getLineNum() - 1);
                                break;
                            }
                            if (line_num > m_editor.max_col - 1)
                            {
                                if (line_num > m_buffer.getLineNum())
                                {
                                    m_cursor.setColOffset(m_buffer.getLineNum() - (int)(m_editor.max_col) + 1);
                                }
                                else
                                {
                                    m_cursor.setColOffset(line_num - (int)(m_editor.max_col) + 1);
                                }

                                m_cursor.setPosition(0, (int)(m_editor.max_col) - 2);
                            }
                            else
                            {
                                if (line_num > m_buffer.getLineNum())
                                    line_num = m_buffer.getLineNum();

                                m_cursor.setColOffset(0);
                                m_cursor.setPosition(0, line_num - 1);
                            }
                            m_text.updateText();
                        }
                        break;
                        default: break;
                    }
                }
                break;
            }
            switch ((wchar_t)msg.wParam)
            {
                case 0x10: // Ctrl-P
                {
                    m_command_prompt.setInput(true, CommandPrompt::OPEN_FILE);
                }
                    break;
                case 0x07: // Ctrl-G
                {
                    m_command_prompt.setInput(true, CommandPrompt::GOTO_LINE);
                }
                    break;
                case 0x1B:
                    m_window.destroy();
                break;
                case 0x13: // Ctrl-S
                {
                    if (m_file.getFilename().size() == 0)
                    {
                        m_command_prompt.setInput(true, CommandPrompt::SAVE_FILE_AS);
                        break;
                    }
                    m_file.write(m_buffer.getContent()); 
                    m_command_prompt.setString(L"File Saved!");
                }
                    
                break;
                default:
                {
                    /* printf("%d\n", (int)msg.wParam); */
                }
                break;
            }

            m_buffer.handleCharInput(msg.wParam, (uint32_t)m_cursor.getRow(), (uint32_t)m_cursor.getCol());
            m_cursor.handleCharInput(msg.wParam, m_buffer, m_editor);
            m_text.updateText();
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
    BeginPaint(m_window.getWindowHandler(), &ps);
    m_render.render_target->BeginDraw();

    m_render.render_target->Clear(D2D1::ColorF(0x15 / 255.0f, 0x15 / 255.0f, 0x15 / 255.0f));
    m_command_prompt.draw(m_render);
    m_cursor.draw(m_render);
    m_render.draw(&m_text);

    hr = m_render.render_target->EndDraw();

    EndPaint(m_window.getWindowHandler(), &ps);
}

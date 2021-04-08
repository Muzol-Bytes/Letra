#include "application.hpp"

#include "log/log.hpp"

#include <iostream>

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

    /// Editor
    D2D1_SIZE_F size = m_render.getSize();

    DWRITE_HIT_TEST_METRICS char_size = m_text.getCharacterMetricsAt(1, 1);
    m_editor.row_offset = 0.0f;
    m_editor.col_offset = 0.0f;
    m_editor.max_row    = size.width / char_size.width;
    m_editor.max_col    = size.height / char_size.height;
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
    /* std::cout << "Cursor Pos: " << m_cursor.getCol() + m_editor.col_offset << '|' */
    /*           << "max_rows: " << m_editor.max_col << '|' */
    /*           << "col: "  << m_buffer.getLineNum() << '\r'; */
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
                m_cursor.move(msg.wParam, m_buffer, m_editor);
            } 
            m_text.setString(m_buffer.getLines(m_editor.col_offset,
                             m_editor.col_offset + (m_editor.max_col - 1)));
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
                            m_command_prompt.setColor(0x3333ff);
                            m_file.setFileName(m_command_prompt.getContent());
                            if (m_file.exist())
                            {
                                m_editor.col_offset = 0;
                                m_buffer.setBuffer(m_file.read());
                                m_text.setString(m_buffer.getLines(m_editor.col_offset,
                                                 m_editor.col_offset + (m_editor.max_col - 1)));
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
                            m_command_prompt.setColor(0x3333ff);
                            m_file.write(m_buffer.getContent());
                            m_command_prompt.setString(L"File Saved!");
                        }
                            break;
                        case CommandPrompt::GOTO_LINE:
                        {
                            size_t line_num = std::stoi(m_command_prompt.getContent()); 
                            if (line_num == 0) break;
                            if (line_num > m_editor.max_col - 1)
                            {
                                if (line_num > m_buffer.getLineNum())
                                {
                                    m_editor.col_offset = m_buffer.getLineNum() - (int)(m_editor.max_col) + 1;
                                }
                                else
                                {
                                    m_editor.col_offset = line_num - (int)(m_editor.max_col) + 1;
                                }

                                m_cursor.setPosition(0, (int)(m_editor.max_col) - 2);
                                m_text.setString(m_buffer.getLines(m_editor.col_offset,
                                                 m_editor.col_offset + (m_editor.max_col - 1)));
                            }
                            else
                            {
                                m_editor.col_offset = 0;
                                m_cursor.setPosition(0, line_num - 1);
                                m_text.setString(m_buffer.getLines(m_editor.col_offset,
                                                 m_editor.col_offset + (m_editor.max_col - 1)));
                            }
                        }
                        break;
                        default: break;
                    }
                }
                break;
            }
            switch ((wchar_t)msg.wParam)
            {
                case 0x08: // Backspace
                    if (m_cursor.getRow() > 0)
                    {
                        m_cursor.move(VK_LEFT, m_buffer, m_editor);
                        m_buffer.deleteaAt(m_cursor.getCol() + m_editor.col_offset, m_cursor.getRow(), 1);
                    }
                    break;
                case 0x09: // Replace tab to space
                    {
                        m_buffer.insertAt(m_cursor.getCol() + m_editor.col_offset, m_cursor.getRow() + m_editor.col_offset, 4, L' ');
                        m_cursor.setPosition(m_cursor.getRow() + 4, m_cursor.getCol(), m_editor);
                    }
                    break;
                case 0x0D: // Enter key
                    {
                        std::wstring temp_str = m_buffer.getLine(m_cursor.getCol() + m_editor.col_offset);
                        std::wstring new_str = L"";

                        for (size_t i = m_cursor.getRow(); i < temp_str.size(); i++)
                        {
                            new_str += temp_str[i];
                        }
                        m_buffer.deleteaAt(m_cursor.getCol() + m_editor.col_offset,
                                           m_cursor.getRow(), new_str.size() - 1);
                        m_buffer.append(new_str, m_cursor.getCol() + 1 + m_editor.col_offset);
                        if (m_cursor.getCol() > m_editor.max_col - 3)
                        {
                            m_editor.col_offset += 1;
                            break;
                        }
                        m_cursor.setPosition(0, m_cursor.getCol() + 1, m_editor);
                    }
                    break;
                case 0x10: // Ctrl-P
                {
                    m_cursor.setPosition(0, m_editor.max_col - 1, m_editor);
                    m_cursor.setColor(0x151515);
                    m_command_prompt.setInput(true, CommandPrompt::OPEN_FILE);
                }
                    break;
                case 0x07: // Ctrl-G
                {
                    m_cursor.setPosition(0, m_editor.max_col - 1, m_editor);
                    m_cursor.setColor(0x151515);
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
                        m_cursor.setPosition(0, m_editor.max_col, m_editor);
                        m_cursor.setColor(0x151515);
                        m_command_prompt.setInput(true, CommandPrompt::SAVE_FILE_AS);
                        break;
                    }
                    m_file.write(m_buffer.getContent()); 
                    m_command_prompt.setString(L"File Saved!");
                }
                    
                break;
                default:
                {
                    m_cursor.setPosition(m_cursor.getRow() + 1, m_cursor.getCol(), m_editor);

                    if (m_cursor.getRow() == 0)
                    {
                        m_buffer.insertAt(m_cursor.getCol() + m_editor.col_offset,
                                          m_cursor.getRow(), 1,
                                          (wchar_t)msg.wParam);
                    }
                    else
                    {
                        m_buffer.insertAt(m_cursor.getCol() + m_editor.col_offset,
                                         (m_cursor.getRow() - 1), 1,
                                         (wchar_t)msg.wParam);
                    }
                }
                    /* printf("%d\n", (int)msg.wParam); */
                break;
            }
                m_text.setString(m_buffer.getLines(m_editor.col_offset,
                                 m_editor.col_offset + (m_editor.max_col - 1)));
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

#include "command_prompt.hpp"

#include <iostream>

CommandPrompt::CommandPrompt(const float x, const float y, ID2D1HwndRenderTarget *render_target)
    : m_text(L"Prompt", render_target)
    , m_body(10.0f, 10.0f)
    , m_buffer()
    , m_input(false)
    , m_cmd_type(CommandPrompt::NONE)
{
    DWRITE_HIT_TEST_METRICS htm = m_text.getCharacterMetricsAt(1, 1);
    m_body.setPosition(x, y);
    m_body.setSize(htm.width, htm.height);
    m_text.setPosition(x, y);
}

std::wstring CommandPrompt::getContent()
{
    std::wstring temp_buffer = m_buffer;
    m_buffer = L"";
    return temp_buffer;
}

CommandPrompt::Type CommandPrompt::getCmdType() const
{
    return m_cmd_type;
}

bool CommandPrompt::getInput() const
{
    return m_input;
}

void CommandPrompt::setInput(const bool inpt, const CommandPrompt::Type cmd_type)
{
    m_body.setColor(0xff3333);
    m_buffer   = L"";
    m_text.setString(m_buffer);
    m_cmd_type = cmd_type;
    m_input    = inpt;
}

void CommandPrompt::setColor(const uint32_t color)
{
    m_body.setColor(color);
}

void CommandPrompt::setString(const std::wstring& str)
{
    m_text.setString(str);
}

bool CommandPrompt::handleInput(const wchar_t chr, Cursor& cursor)
{
    size_t row = cursor.getRow() - cursor.getRowPadding();

    if (m_input)
    {
        switch (chr)
        {
        case 0x08: // Backspace
        {
            if (row > 0)
            {
                cursor.setPosition(row - 1, cursor.getCol());
                m_buffer.erase(row, 1);
            }
        }
        break;
        case 0x0D: // enter
        {
            m_input = false;

            cursor.setPosition(0, 0);
            setColor(0x3333ff);
            return true;
        }
        break;
        case 0x1B:
        {
            m_buffer = L"";
            m_text.setString(m_buffer);
            m_input = false;
            cursor.setColor(0xfe8019);
            cursor.setPosition(0, 0);
            setColor(0x3333ff);
        }
        break;
        default:
        {
            m_buffer += chr;
            cursor.setPosition(row + 1, cursor.getCol());
        }
        break;
        }
        m_text.setString(m_buffer);
    }
    return false;
}

void CommandPrompt::setPosition(const float x, const float y)
{
    m_body.setPosition(x, y);
    m_text.setPosition(x, y);
}

void CommandPrompt::setWidth(const float width)
{
    DWRITE_HIT_TEST_METRICS htm = m_text.getCharacterMetricsAt(1, 1);
    /* std::cout << htm.height << '\n'; */
    m_body.setSize(width, htm.height);
    /* std::cout << m_body.getHeight() << " | " << m_body.getWidth() << '\n'; */
}

void CommandPrompt::draw(Renderer& render)
{
    render.draw(&m_body);
    render.draw(&m_text);
}

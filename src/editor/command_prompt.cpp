#include "command_prompt.hpp"

#include <iostream>

CommandPrompt::CommandPrompt(const float x, const float y, ID2D1HwndRenderTarget *render_target)
    : m_text(L"Prompt", render_target)
    , m_body(10.0f, 10.0f)
    , m_buffer()
    , m_get_input(false)
{
    DWRITE_HIT_TEST_METRICS htm = m_text.getCharacterMetricsAt(1, 1);
    m_body.setPosition(x, y);
    m_body.setSize(htm.width, htm.height);
    m_text.setPosition(x, y);
}

std::wstring CommandPrompt::getContent()
{
    return m_buffer.str();
}

bool CommandPrompt::getInput() const
{
    return m_get_input;
}

void CommandPrompt::setString(const std::wstring& str)
{
    m_text.setString(str);
}

void CommandPrompt::handleInput(const wchar_t chr)
{
    if (m_get_input)
    {
        switch (chr)
        {
        case 0x0D:
        {
            m_get_input = false;
        }
            break;
        default:
        {
            m_buffer << chr;
        }
            break;
        }
        m_text.setString(getContent());
    }
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


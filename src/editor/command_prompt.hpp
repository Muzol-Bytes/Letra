#ifndef _COMMAND_PROMPT_HPP_
#define _COMMAND_PROMPT_HPP_

#include <d2d1.h>
#include <string>
#include <sstream>
#include <cwchar>

#include "../graphics/renderer.hpp"
#include "../graphics/text.hpp"
#include "../graphics/rectangle.hpp"

class CommandPrompt
{
public:
    CommandPrompt(const float x, const float y, ID2D1HwndRenderTarget *render_target);

    std::wstring getContent();

    bool getInput() const;

    void setString(const std::wstring& str);

    void setPosition(const float x, const float y);

    void setWidth(const float width);

    void handleInput(const wchar_t chr);

    void draw(Renderer& render);

private:
    Text m_text;
    Rect m_body;

    std::wstringstream m_buffer;
    bool m_get_input;
};

#endif // _COMMAND_PROMPT_HPP_

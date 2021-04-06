#ifndef _COMMAND_PROMPT_HPP_
#define _COMMAND_PROMPT_HPP_

#include <d2d1.h>
#include <string>
#include <cwchar>

#include "cursor.hpp"
#include "../graphics/renderer.hpp"
#include "../graphics/text.hpp"
#include "../graphics/rectangle.hpp"

/// TODO: make a enum to diferenciate between different kinda of commands
/// like OPEN_FILE, GOTO_LINE, SAVE_FILE_AS etc

class CommandPrompt
{
public:
    CommandPrompt(const float x, const float y, ID2D1HwndRenderTarget *render_target);

    std::wstring getContent();

    bool getInput() const;

    bool getInputReady() const;

    void setColor(const uint32_t color);

    void setInput(const bool inpt);

    void setString(const std::wstring& str);

    void setPosition(const float x, const float y);

    void setWidth(const float width);

    bool handleInput(const wchar_t chr, Cursor& cursor);

    void draw(Renderer& render);

private:
    Text m_text;
    Rect m_body;

    std::wstring m_buffer;
    bool m_input;
};

#endif // _COMMAND_PROMPT_HPP_

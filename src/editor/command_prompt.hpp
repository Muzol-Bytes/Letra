#ifndef _COMMAND_PROMPT_HPP_
#define _COMMAND_PROMPT_HPP_

#include <d2d1.h>
#include <string>
#include <cwchar>

#include "cursor.hpp"
#include "../graphics/renderer.hpp"
#include "../graphics/text.hpp"
#include "../graphics/rectangle.hpp"

class CommandPrompt
{
public:
    enum Type
    {
        NONE,
        OPEN_FILE,
        GOTO_LINE,
        SAVE_FILE_AS,
    };

    CommandPrompt(const float x, const float y, ID2D1HwndRenderTarget *render_target);

    std::wstring getContent();

    CommandPrompt::Type getCmdType() const;

    bool getInput() const;

    bool getInputReady() const;

    void setColor(const uint32_t color);

    void setInput(const bool inpt, const CommandPrompt::Type cmd_type);

    void setString(const std::wstring& str);

    void setPosition(const float x, const float y);

    void setWidth(const float width);

    bool handleInput(const wchar_t chr, Cursor& cursor);

    void draw(Renderer& render);

private:
    Text m_text;
    Rect m_body;
    Type m_cmd_type;

    std::wstring m_buffer;
    bool m_input;
};

#endif // _COMMAND_PROMPT_HPP_

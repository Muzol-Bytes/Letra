#ifndef _APPLICATION_HPP_
#define _APPLICATION_HPP_

#include <vector>

#include "graphics/text.hpp"
#include "editor/buffer.hpp"
#include "editor/command_prompt.hpp"
#include "file.hpp"
#include "editor/cursor.hpp"
#include "graphics/renderer.hpp"
#include "graphics/rectangle.hpp"
#include "window/window.hpp"

class Application
{
public:
    Application(const std::vector<std::wstring> argv);

    ~Application();

    void run();

    void update();

    void handleInput(const MSG& msg);

    void onPaint();

private:
    Window   window;
    Renderer render;
    File     m_file;
    Buffer   m_buffer;
    Text     m_text;
    Cursor   m_cursor;
    CommandPrompt m_command_prompt;

    ID2D1SolidColorBrush *p_brush;
};

#endif // _APPLICATION_HPP_

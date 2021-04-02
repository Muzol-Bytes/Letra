#ifndef _APPLICATION_HPP_
#define _APPLICATION_HPP_

#include <vector>

#include "editor/editor.hpp"
#include "file.hpp"
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
    Window window;
    Editor editor;
    File m_file;
    Renderer render;

    ID2D1SolidColorBrush *p_brush;
    D2D1_RECT_F cursor;
};

#endif // _APPLICATION_HPP_

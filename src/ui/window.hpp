#ifndef _WINDOW_HPP_
#define _WINDOW_HPP_

#include <cstdint>
#include <d2d1.h>

#include "../editor.hpp"
#include "base_window.hpp"
#include "renderer.hpp"

class Window : public BaseWindow<Window>
{
public:
    Window(const std::wstring &filename);

    LPCSTR  getClassName() const;

    LRESULT handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

    void init();

    void onPaint();

    void show();

private:
    Editor editor;

    Renderer render;
    ID2D1SolidColorBrush *p_brush;
    D2D1_RECT_F cursor;
};

/* template <class T> static void SafeRelease(T **ppT); */

#endif // _WINDOW_HPP_

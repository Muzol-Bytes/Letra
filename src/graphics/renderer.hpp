#ifndef _RENDERER_HPP_
#define _RENDERER_HPP_

#include <windows.h>
#include <d2d1.h>

#include "drawable.hpp"

class Renderer
{
public:
    Renderer(HWND hwnd);

    ~Renderer();

    HRESULT createGraphicsResources(HWND hwnd);

    D2D1_SIZE_F getSize() const;

    void draw(IDrawable *drawable);

    ID2D1HwndRenderTarget *render_target;
    
private:
    void discardGraphicsResources();

    ID2D1Factory          *p_factory;
    ID2D1SolidColorBrush  *p_bg_brush;
    ID2D1SolidColorBrush  *p_drawable_brush;
};

#endif // _RENDERER_HPP_

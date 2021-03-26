#ifndef _RENDERER_HPP_
#define _RENDERER_HPP_

#include <windows.h>
#include <d2d1.h>

class Renderer
{
public:
    Renderer();

    ~Renderer();

    HRESULT createGraphicsResources(HWND hwnd);

    ID2D1HwndRenderTarget *render_target;
    
private:
    void discardGraphicsResources();

    ID2D1Factory          *p_factory;
    ID2D1SolidColorBrush  *p_bg_brush;
};

#endif // _RENDERER_HPP_

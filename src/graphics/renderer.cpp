#include "renderer.hpp"

#include "../log/log.hpp"

Renderer::Renderer(HWND hwnd)
{
    p_factory        = NULL;
    render_target    = NULL;
    p_bg_brush       = NULL;
    p_drawable_brush = NULL;

    createGraphicsResources(hwnd);
}

Renderer::~Renderer()
{
    discardGraphicsResources();
}

#if 0
ID2D1HwndRenderTarget* Renderer::get_render_target() const
{
    return render_target;
}
#endif

HRESULT Renderer::createGraphicsResources(HWND hwnd)
{

    HRESULT hr = S_OK;

    if (FAILED(D2D1CreateFactory(
                    D2D1_FACTORY_TYPE_SINGLE_THREADED, &p_factory)))
    {
        errorExit("D2D1CreateFactory");
        return S_FALSE;  // Fail to create D2D1Factory.
    }
    
    RECT rect;

    GetClientRect(hwnd, &rect);
    D2D1_SIZE_U size = D2D1::SizeU(rect.right, rect.bottom);

    hr = p_factory->CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(hwnd, size),
            &render_target);

    if (FAILED(hr) && render_target == NULL)
    {
        errorExit("CreateHwndRenderTarget");
    }

    const D2D1::ColorF color = D2D1::ColorF(1.0f, 1.0f, 0.0f);
    hr = render_target->CreateSolidColorBrush(color, &p_bg_brush);

    if (FAILED(hr))
    {
        errorExit("CreateSolidBrush");
    }

    hr = render_target->CreateSolidColorBrush(D2D1::ColorF(1.0f, 1.0f, 1.0f), &p_drawable_brush);

    return hr;
}

void Renderer::draw(IDrawable *drawable)
{
    drawable->draw(render_target, p_drawable_brush);
}

template <class T> static void SafeRelease(T **ppT)
{
    if (*ppT)
    {
        (*ppT)->Release();
        *ppT = NULL;
    }
}


void Renderer::discardGraphicsResources()
{
    SafeRelease(&render_target);
    SafeRelease(&p_bg_brush);
    SafeRelease(&p_drawable_brush);
}

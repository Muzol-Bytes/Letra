#include "text.hpp"
#include "../util/log.hpp"

#include <cstdio>

template <class T> static void SafeRelease(T **ppT)
{
    if (*ppT)
    {
        (*ppT)->Release();
        *ppT = NULL;
    }
}

Text::Text()
{
    m_screen_size = { 0 };
    p_Dwrite_factory = NULL;
    p_Dtext_format   = NULL;
    p_color_brush    = NULL;
    m_text           = L"";
}

Text::Text(std::wstring text)
{
    m_screen_size = { 0 };
    p_Dwrite_factory = NULL;
    p_Dtext_format   = NULL;
    p_color_brush    = NULL;
    m_text           = text;
}

Text::~Text()
{
    SafeRelease(&p_Dwrite_factory);
    SafeRelease(&p_Dtext_format);
    SafeRelease(&p_Dtext_layout);
    SafeRelease(&p_color_brush);
}

HRESULT Text::createDeviceIndependentResources(ID2D1HwndRenderTarget *render_target)
{
    HRESULT hr = S_OK;

    hr = DWriteCreateFactory(
        DWRITE_FACTORY_TYPE_SHARED,
        __uuidof(IDWriteFactory),
        reinterpret_cast<IUnknown**>(&p_Dwrite_factory)
        );

    if (SUCCEEDED(hr))
    {
        hr = p_Dwrite_factory->CreateTextFormat(
            L"Liberation Mono",
            NULL,
            DWRITE_FONT_WEIGHT_REGULAR,
            DWRITE_FONT_STYLE_NORMAL,
            DWRITE_FONT_STRETCH_NORMAL,
            16.0f,
            L"en_us",
            &p_Dtext_format
            );
    }
    else
    {
        ErrorExit("CreateTextFormat");
    }

    if (SUCCEEDED(hr))
    {
        HWND hwnd = render_target->GetHwnd();
        GetClientRect(hwnd, &m_screen_size);

        hr = p_Dwrite_factory->CreateTextLayout(
            m_text.c_str(),
            m_text.size(),
            p_Dtext_format,
            m_screen_size.right,
            m_screen_size.bottom,
            &p_Dtext_layout);
    }

    if (SUCCEEDED(hr))
    {
        hr = render_target->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &p_color_brush); 
        if (hr != S_OK)
        {
            ErrorExit("CreateSolidColorBrush");
        }
    }

    return hr;
}

void Text::setString(const std::wstring& str)
{

    HRESULT hr = S_OK;
    IDWriteTextLayout *temp_layout = NULL;

    hr = p_Dwrite_factory->CreateTextLayout(
        str.c_str(),
        str.size(),
        p_Dtext_format,
        m_screen_size.right,
        m_screen_size.bottom,
        &temp_layout);

    if (SUCCEEDED(hr))
    {
        SafeRelease(&p_Dtext_layout);
        p_Dtext_layout = temp_layout;
        m_text = str;
    }
}

DWRITE_HIT_TEST_METRICS Text::getCharacterMetricsAt(UINT32 char_pos)
{
    DWRITE_HIT_TEST_METRICS htm;
    FLOAT x;
    FLOAT y;
    p_Dtext_layout->HitTestTextPosition(
            char_pos,
            1,
            &x,
            &y,
            &htm);

#if 0
    printf("x = %f; y = %f\n", x, y);
    printf("left %f\n", htm.left);
    printf("right %f\n", htm.left+htm.width);
    printf("top %f\n", htm.top);
    printf("bottom %f\n", htm.top + htm.height);
#endif

    return htm;
}

DWRITE_HIT_TEST_METRICS Text::getCharacterMetricsAt(const float x, const float y)
{
    DWRITE_HIT_TEST_METRICS htm;
    BOOL is_trailinghit;
    BOOL is_inside;

    p_Dtext_layout->HitTestPoint(
            (FLOAT) x,
            (FLOAT) y,
            &is_trailinghit,
            &is_inside,
            &htm);


    return htm;
}

void Text::draw(ID2D1HwndRenderTarget *render_target)
{
    render_target->DrawTextLayout(D2D1_POINT_2F{0.0f, 0.0f}, p_Dtext_layout, p_color_brush);
}

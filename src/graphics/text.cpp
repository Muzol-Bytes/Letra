#include "text.hpp"
#include "../log/log.hpp"

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
    m_screen_size    = { 0 };
    p_Dwrite_factory = NULL;
    p_Dtext_format   = NULL;
    m_text           = L" ";
    m_pos            = (D2D1_POINT_2F){0.0f, 0.0f};
    m_color          = 0xffffff;
}

Text::Text(std::wstring text, ID2D1HwndRenderTarget *render_target)
{
    m_screen_size    = { 0 };
    p_Dwrite_factory = NULL;
    p_Dtext_format   = NULL;
    m_text           = text;
    m_pos            = (D2D1_POINT_2F){0.0f, 0.0f};
    m_color          = 0xffffff;

    createDeviceIndependentResources(render_target);
}

Text::Text(Buffer buffer, ID2D1HwndRenderTarget *render_target)
{
    m_screen_size    = { 0 };
    p_Dwrite_factory = NULL;
    p_Dtext_format   = NULL;
    m_text           = L" ";
    m_pos            = (D2D1_POINT_2F){0.0f, 0.0f};
    m_color          = 0xffffff;

    createDeviceIndependentResources(render_target);

    float char_height     = getCharacterMetricsAt(1).height; 
    float max_screen_col  = m_screen_size.bottom / char_height;
    setString(buffer.getLines(0.0f, max_screen_col - 1));
}

Text::~Text()
{
    SafeRelease(&p_Dwrite_factory);
    SafeRelease(&p_Dtext_format);
    SafeRelease(&p_Dtext_layout);
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
        errorExit("CreateTextFormat");
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

    return hr;
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

void Text::setPosition(const float x, const float y)
{
    m_pos = (D2D1_POINT_2F){ x, y };
}

D2D1_POINT_2F Text::getPosition() const
{
    return m_pos;
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

void Text::draw(ID2D1HwndRenderTarget *render_target, ID2D1SolidColorBrush *brush)
{
    brush->SetColor(D2D1::ColorF(
                (m_color >> 16 & 0xff) / 255.0f,
                (m_color >> 8  & 0xff) / 255.0f,
                (m_color       & 0xff) / 255.0f)); 
    render_target->DrawTextLayout(m_pos, p_Dtext_layout, brush);
}

#ifndef _TEXT_HPP_
#define _TEXT_HPP_

#include <windows.h>
#include <dwrite.h>
#include <d2d1.h>

#include <string>
#include <cstdint>

#include "drawable.hpp"

class Text : public IDrawable
{
public:
    Text();

    Text(std::wstring text, ID2D1HwndRenderTarget *render_target);

    ~Text();

    /// Initialize the text core resources and create a renderable text object
    HRESULT createDeviceIndependentResources(ID2D1HwndRenderTarget *render_target);

    /// Get a character metrics at a certain position (use it like get a value from an array)
    DWRITE_HIT_TEST_METRICS getCharacterMetricsAt(UINT32 char_pos);

    /// Get a character metrics at x and y device independent pixels 
    DWRITE_HIT_TEST_METRICS getCharacterMetricsAt(const float x, const float y);

    void setPosition(const float x, const float y);

    D2D1_POINT_2F getPosition() const;

    /// NOTE: Only call this function after you call "createDeviceIndependentResources"
    void setString(const std::wstring& str);

    /// Draw text
    void draw(ID2D1HwndRenderTarget *render_target, ID2D1SolidColorBrush *brush);

private:
    IDWriteFactory    *p_Dwrite_factory;
    IDWriteTextFormat *p_Dtext_format;
    IDWriteTextLayout *p_Dtext_layout;
    RECT m_screen_size;

    std::wstring m_text;
    D2D1_POINT_2F m_pos;
};

#endif // _TEXT_HPP_

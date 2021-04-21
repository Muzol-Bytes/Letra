#ifndef _TEXT_HPP_
#define _TEXT_HPP_

#include <windows.h>
#include <dwrite.h>
#include <d2d1.h>

#include <string>
#include <cstdint>

#include "drawable.hpp"
#include "../editor/buffer.hpp"

class Text : public IDrawable
{
public:
    Text();

    Text(std::wstring text, ID2D1HwndRenderTarget *render_target);

    Text(Buffer buffer, ID2D1HwndRenderTarget *render_target);

    ~Text();

    /// Initialize the text core resources and create a renderable text object
    HRESULT createDeviceIndependentResources(ID2D1HwndRenderTarget *render_target, std::wstring text);

    /// Get a character metrics at a certain position (use it like get a value from an array)
    DWRITE_HIT_TEST_METRICS getCharacterMetricsAt(UINT32 char_pos);

    /// Get a character metrics at x and y device independent pixels 
    DWRITE_HIT_TEST_METRICS getCharacterMetricsAt(const float x, const float y);

    D2D1_POINT_2F getPosition() const;

    /// Sets a internal boolean for update text data when it's called
    void updateText(); 

    /// Returns the value of m_update
    bool update(); 

    /// Set a position of the text box
    void setPosition(const float x, const float y);

    /// NOTE: Only call this function after you call "createDeviceIndependentResources"
    void setString(const std::wstring& str);

    /// Draw text
    void draw(ID2D1HwndRenderTarget *render_target, ID2D1SolidColorBrush *brush);

private:
    IDWriteFactory    *p_Dwrite_factory;
    IDWriteTextFormat *p_Dtext_format;
    IDWriteTextLayout *p_Dtext_layout;
    RECT m_screen_size;

    D2D1_POINT_2F m_pos;
    bool m_update;
};

#endif // _TEXT_HPP_

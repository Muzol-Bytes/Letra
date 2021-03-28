#ifndef _RECTANGLE_HPP_
#define _RECTANGLE_HPP_

#include <d2d1.h>

#include "drawable.hpp"

class Rect : public IDrawable
{
public:
    Rect(float width, float height);

    void setPosition(const float x, const float y);

    void setSize(const float width, const float height);

    void setColor(const uint32_t color);

private:
    void draw(ID2D1HwndRenderTarget *render_target, ID2D1SolidColorBrush *brush);

    D2D1_RECT_F m_body;
};

#endif // _RECTANGLE_HPP_

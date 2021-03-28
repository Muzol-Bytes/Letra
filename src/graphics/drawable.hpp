#ifndef _DRAWABLE_HPP_
#define _DRAWABLE_HPP_

#include <cstdint>
#include <d2d1.h>

class IDrawable
{
public:
    IDrawable();

    virtual void draw(ID2D1HwndRenderTarget *render_target, ID2D1SolidColorBrush *brush) = 0;

    virtual ~IDrawable() {}
protected:
    uint32_t m_color;

};


#endif // _DRAWABLE_HPP_

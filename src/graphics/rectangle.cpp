#include "rectangle.hpp"

Rect::Rect(float width, float height)
{
    m_color       = 0x3333ff;
    m_body.left   = 0.0f;
    m_body.right  = width;
    m_body.top    = 0.0f;
    m_body.bottom = height;
}

float Rect::getHeight() const
{
    return m_body.bottom - m_body.top;
}

float Rect::getWidth() const
{
    return m_body.right - m_body.left;
}

void Rect::setPosition(const float x, const float y)
{
    float width   = m_body.right  - m_body.left;
    float height  = m_body.bottom - m_body.top;
    m_body.left   = x;
    m_body.right  = m_body.left + width;
    m_body.top    = y;
    m_body.bottom = m_body.top + height;
}

void Rect::setSize(const float width, const float height)
{
    m_body.right  = m_body.left + width;
    m_body.bottom = m_body.top + height;
}

void Rect::setColor(const uint32_t color)
{
    m_color = color;
}

void Rect::draw(ID2D1HwndRenderTarget *render_target, ID2D1SolidColorBrush *brush)
{
    brush->SetColor(D2D1::ColorF(
                (m_color >> 16 & 0xff) / 255.0f,
                (m_color >> 8  & 0xff) / 255.0f,
                (m_color       & 0xff) / 255.0f)); 

    render_target->FillRectangle(m_body, brush);
}

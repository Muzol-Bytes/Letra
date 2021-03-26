#ifndef _COLOR_HPP_
#define _COLOR_HPP_

#include <cstdint>

struct Color
{
    Color(uint8_t _red, uint8_t _green, uint8_t _blue, uint8_t _alpha);

    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t alpha;
};

#endif // _COLOR_HPP_

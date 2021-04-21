#ifndef __EDITOR_HPP__
#define __EDITOR_HPP__

#include <cstdint>

struct Editor
{
    size_t *row_padding;
    float row_offset;
    float col_offset;
    float max_row;
    float max_col;
    bool  is_file_modified;
    bool  is_file_new;
};

#endif // __EDITOR_HPP__

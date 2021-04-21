#include "buffer.hpp"

Buffer::Buffer()
    : m_content()
{
    m_row_padding = std::to_string(getLineNum()).size() + 1;
}

Buffer::Buffer(const std::vector<std::wstring> content)
    : m_content(content)
{
    m_row_padding = std::to_string(getLineNum()).size() + 1;
}

std::wstring Buffer::getContent()
{
    std::wstring data;

    for (std::wstring& line : m_content)
    {
        data += line;
    }

    return data;
}

std::wstring Buffer::getLines(const size_t from, size_t to)
{
    if (to > m_content.size())
        to = m_content.size();

    m_row_padding = std::to_string(getLineNum()).size() + 1;
    std::wstring data;
    size_t space_padding = 0;

    for (size_t i = from; i < to; i++)
    {
        /// Add number to line with padding
        space_padding = std::to_string(getLineNum()).size() - std::to_string(i + 1).size();
        std::wstring temp_str = m_content[i];
        temp_str.insert(0, space_padding, L' ');
        temp_str.insert(space_padding, std::to_wstring(i + 1) + L' ');
        ///////////////////////////////////

        data += temp_str;
    }

    return data;
}

std::wstring Buffer::getLine(const size_t line_num)
{
    return m_content[line_num];
}

size_t& Buffer::getRowPadding()
{
    return m_row_padding;
}

size_t Buffer::getLineNum()
{
    return m_content.size();
}

size_t Buffer::getLineLengthAt(const size_t line_num)
{
    return m_content[line_num].size();
}

void Buffer::handleCharInput(const WPARAM wParam, const uint32_t row, const uint32_t col)
{
    switch (wParam)
    {
        case 0x08: // Backspace
        if (row > 0)
        {
            deleteaAt(col, row - 1, 1);
        }
        break;
        case 0x09: // Replace tab to space
        { 
            insertAt(col, row, 4, L' ');
        }
        break;
        case 0x0D: // Enter key
        {
            std::wstring temp_str = getLine(col);
            std::wstring new_str = L"";

            for (size_t i = row; i < temp_str.size(); i++)
            {
                new_str += temp_str[i];
            }
            deleteaAt(col, row, new_str.size() - 1);
            append(new_str, col + 1);
        }
        break;
        default:
        {
            insertAt(col, row, 1, wParam);
        }
        break;
    }

}

void Buffer::setBuffer(const std::vector<std::wstring> file_content)
{
    m_content = file_content;
}

void Buffer::insertAt(const size_t line_num, const size_t pos, const size_t count, const wchar_t chr)
{
    /// Should never happen
    if (line_num > getLineNum())
        return;

    m_content[line_num].insert(pos, count, chr);
}

void Buffer::append(const std::wstring& data, const size_t pos)
{
    m_content.insert(m_content.begin() + pos, data); 
}

void Buffer::deleteaAt(const size_t line_num, const size_t pos, const size_t count)
{
    m_content[line_num].erase(pos, count);
}

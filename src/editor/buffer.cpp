#include "buffer.hpp"

Buffer::Buffer()
    : m_content()
{
    m_line_num_padding = std::to_string(getLineNum()).size();
}

Buffer::Buffer(const std::vector<std::wstring> content)
    : m_content(content)
{
    m_line_num_padding = std::to_string(getLineNum()).size();
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

    m_line_num_padding = std::to_string(getLineNum()).size();
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

size_t Buffer::getLineNumPadding() const
{
    return m_line_num_padding;
}

size_t Buffer::getLineNum()
{
    return m_content.size();
}

size_t Buffer::getLineLengthAt(const size_t line_num)
{
    return m_content[line_num].size();
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

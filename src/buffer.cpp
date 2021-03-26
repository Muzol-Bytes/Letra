#include "buffer.hpp"

Buffer::Buffer()
    : m_content()
{
}


void Buffer::setBuffer(const std::vector<std::wstring> file_content)
{
    m_content = file_content;
}
/* Buffer::Buffer(const std::wstring& content) */
/*     : m_content(content) */
/* { */
/* } */

std::wstring Buffer::getContent()
{
    std::wstring data;

    for (std::wstring& line : m_content)
    {
        data += line;
    }

    return data;
}

size_t Buffer::getLineNum()
{
    return m_content.size();
}

std::wstring Buffer::getLine(const size_t line_num)
{
    return m_content[line_num];
}

size_t Buffer::getLineLengthAt(const size_t line_num)
{
    return m_content[line_num].size();
}

void Buffer::insertAt(const size_t line_num, const size_t pos, const wchar_t chr)
{
    /// Should never happen
    if (line_num > getLineNum())
        return;

    m_content[line_num].insert(pos, 1, chr);
}

void Buffer::append(const std::wstring& data, const size_t pos)
{
    m_content.insert(m_content.begin() + pos, data); 
}

void Buffer::deleteaAt(const size_t line_num, const size_t pos, const size_t count)
{
    m_content[line_num].erase(pos, count);
}

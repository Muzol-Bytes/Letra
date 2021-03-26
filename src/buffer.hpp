#ifndef _BUFFER_HPP_
#define _BUFFER_HPP_

#include <string>
#include <vector>

class Buffer
{
public:
    Buffer();

    /* Buffer(const std::wstring& content); */

    void setBuffer(const std::vector<std::wstring> file_content);

    std::wstring getContent();

    size_t getLineNum();

    std::wstring getLine(const size_t line_num);

    size_t getLineLengthAt(const size_t line_num);

    void insertAt(const size_t line_num, const size_t pos, const wchar_t chr);

    void append(const std::wstring& data, const size_t pos);

    void deleteaAt(const size_t line_num, const size_t pos, const size_t count);

private:
    std::vector<std::wstring> m_content;
};

#endif // _BUFFER_HPP_

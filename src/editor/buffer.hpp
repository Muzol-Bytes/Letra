#ifndef _BUFFER_HPP_
#define _BUFFER_HPP_

#include <string>
#include <vector>

class Buffer
{
public:
    Buffer();
    Buffer(const std::vector<std::wstring> content);

    /// Get the entire content of the buffer as a std::wstring
    std::wstring getContent();

    /// Get a range of lines as a std::wstring
    std::wstring getLines(const size_t from, size_t to);

    /// Get the line as a std::wstring
    std::wstring getLine(const size_t line_num);

    /// Get the number of lines the buffer has
    size_t getLineNum();

    /// Get the length of a line of the given position
    size_t getLineLengthAt(const size_t line_num);

    /// Set the content of the buffer to the given one
    void setBuffer(const std::vector<std::wstring> file_content);

    /// Insert a character at a certain line/position
    void insertAt(const size_t line_num, const size_t pos, const size_t count, const wchar_t chr);

    /// Append a line in the struct must be '\n' terminated
    void append(const std::wstring& data, const size_t pos);

    /// Delete a give amount of characters in a certain line/position
    void deleteaAt(const size_t line_num, const size_t pos, const size_t count);

private:
    std::vector<std::wstring> m_content;
};

#endif // _BUFFER_HPP_

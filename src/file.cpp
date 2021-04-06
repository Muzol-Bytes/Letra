#include "file.hpp"

#include <fstream>

File::File(const std::wstring& filename)
    : m_filename(filename)
{
}

std::vector<std::wstring> File::read()
{
    std::vector<std::wstring> content;
    std::wifstream file(m_filename);

    if (file.is_open())
    {
        std::wstring line;
        while (std::getline(file, line))
            content.emplace_back(line + L"\n");
        file.close();
    }
    else
    {
        content.emplace_back(L" \n");
    }

    return content;
}

std::wstring File::getFilename() const
{
    return m_filename;
}

void File::setFileName(const std::wstring& filename)
{
    m_filename = filename;
}

void File::write(const std::wstring& content)
{
    std::wofstream file(m_filename);

    if (file.is_open())
    {
        file << content;
        file.close();
    }
}

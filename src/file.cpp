#include "file.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

std::vector<std::wstring> readFile(const std::wstring& filename)
{
    std::vector<std::wstring> content;
    std::wifstream file(filename);

    std::wstring line;
    while (std::getline(file, line))
        content.emplace_back(line + L"\n");

    return content;
}

#if 0
int main()
{
    std::vector<std::wstring> content = readFile(L"src/main.cpp");
    for (std::wstring& line : content)
    {
        std::wcout << line;
    }
}
#endif

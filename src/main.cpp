#include <cstdio>
#include <cwchar>
#include <cstdlib>
#include <cstdint>
#include <string>
#include <cstring>
#include <iostream>

#include "ui/window.hpp"

const uint16_t SCREEN_WIDTH  = 1080;
const uint16_t SCREEN_HEIGHT = 720;

int main(int argc, char* argv[])
{
    /* std::wstring filename = (const wchar_t*) argv[1]; */
    std::wstring filename = L"";
    for (int i = 0; i < strlen(argv[1]); i++)
    {
        filename += (wchar_t) argv[1][i];
    }

    /* std::string filename = (const char*)argv[1]; */

    Window window(filename);

    if (!window.create("Letra", WS_OVERLAPPEDWINDOW))
        return -1;

    window.show();
    return 0;
}

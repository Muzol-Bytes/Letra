#ifndef UNICODE
#define UNICODE
#endif

#include "application.hpp"

const uint16_t SCREEN_WIDTH  = 1080;
const uint16_t SCREEN_HEIGHT = 720;

int wmain( int argc, wchar_t *argv[], wchar_t *envp[] )
{
    std::vector<std::wstring> args = { L"" };

    if (argc > 1)
        /// (argc + 1) to skip the first argumnet which is the program filename
        args = std::vector<std::wstring>(argv + 1, argv + argc);
    else
        return 0;

    Application app(args);

    app.run();

    return 0;
}

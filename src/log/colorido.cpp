#include <cstdio>
#include <cstdint>

#include <string>

#include <windows.h>

namespace seda
{

bool enableVtMode()
{
    HANDLE h_out = GetStdHandle(STD_OUTPUT_HANDLE);
    if (h_out == INVALID_HANDLE_VALUE)
        return false;

    DWORD dw_mode = 0;
    if (!GetConsoleMode(h_out, &dw_mode))
        return false;

    dw_mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    if (!SetConsoleMode(h_out, dw_mode))
        return false;

    return true;
}

template <class Type>
struct Vector2
{
    Type x;
    Type y;
};

struct Window
{
    Window(const std::string &window_title)
    {
        Vector2<short> size = getSize();
        width  = size.x; 
        height = size.y; 
        title  = window_title;
    }

    static Vector2<short> getSize()
    {
        Vector2<short> size = { 0, 0 };
        HANDLE h_out = GetStdHandle(STD_OUTPUT_HANDLE);

        if (h_out == INVALID_HANDLE_VALUE)
            return size;

        CONSOLE_SCREEN_BUFFER_INFO screen_buffer_info;
        GetConsoleScreenBufferInfo(h_out, &screen_buffer_info);
        size.x = screen_buffer_info.srWindow.Right - screen_buffer_info.srWindow.Left + 1; 
        size.y = screen_buffer_info.srWindow.Bottom - screen_buffer_info.srWindow.Top + 1; 

        return size;
    }

    uint16_t width;
    uint16_t height;
    std::string title;
};

}

int main()
{
    if (!seda::enableVtMode())
    {
        printf("Couldn't enable VT processing mode.\n");
        return -1;
    }

    seda::Window window("Faith");

    printf("\x1b[33m%d;%d\n\x1b[0m", window.width, window.height);

    return 0;
}

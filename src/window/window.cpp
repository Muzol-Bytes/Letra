#include "window.hpp"
#include "../log/log.hpp"

#include <string>

Window::Window (LPCSTR title, int width, int height)
    :BaseWindow()
{
    if (!create(title, WS_OVERLAPPEDWINDOW, 0, width, height))
        errorExit("Create Window: ");
}

LPCSTR Window::getClassName() const
{
    return "Letra Window Class";
}

LRESULT Window::handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_DESTROY:
            PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
}

void Window::destroy()
{
    DestroyWindow(m_hwnd);
}

void Window::show()
{ 
    ShowWindow(m_hwnd, SW_SHOWDEFAULT);
}

#include "base_window.hpp"

template <class DERIVED_CLASS> 
LRESULT CALLBACK BaseWindow<DERIVED_CLASS>::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    DERIVED_CLASS *pThis = NULL;

    if (uMsg == WM_NCCREATE)
    {
        CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
        pThis = (DERIVED_CLASS*)pCreate->lpCreateParams;
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);

        pThis->m_hwnd = hwnd;
    }
    else
    {
        pThis = (DERIVED_CLASS*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    }
    if (pThis)
    {
        return pThis->handleMessage(uMsg, wParam, lParam);
    }
    else
    {
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

template <class DERIVED_CLASS> 
BaseWindow<DERIVED_CLASS>::BaseWindow()
    :m_hwnd(NULL)
{
}

template <class DERIVED_CLASS> 
BOOL BaseWindow<DERIVED_CLASS>::create(
        PCWSTR title,
        DWORD dwStyle,
        DWORD dwExStyle,
        int x,
        int y,
        int width,
        int height,
        HWND hWndParent,
        HMENU hMenu
        )
{
    WNDCLASS wc = {0};

    wc.lpfnWndProc   = DERIVED_CLASS::WindowProc;
    wc.hInstance     = GetModuleHandle(NULL);
    wc.lpszgetClassName = getClassName();

    RegisterClass(&wc);

    m_hwnd = CreateWindowEx(
            dwExStyle, getClassName(), title, dwStyle, x, y,
            width, height, hWndParent, hMenu, GetModuleHandle(NULL), this
            );

    return (m_hwnd ? TRUE : FALSE);
}

template <class DERIVED_CLASS> 
HWND BaseWindow<DERIVED_CLASS>::getWindowHandler() const
{ 
    return m_hwnd;
}

#ifndef _BASE_WINDOW_HPP_
#define _BASE_WINDOW_HPP_

#include <windows.h>

template <class DERIVED_CLASS> 
class BaseWindow
{
public:
    BaseWindow()
        : m_hwnd(NULL)
    {
    }

   static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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

    BOOL create(
            LPCSTR title,
            DWORD dwStyle,
            DWORD dwExStyle = 0,
            int width = CW_USEDEFAULT,
            int height = CW_USEDEFAULT,
            int x = CW_USEDEFAULT,
            int y = CW_USEDEFAULT,
            HWND hWndParent = 0,
            HMENU hMenu = 0
            )
    {
        WNDCLASS wc = { };

        wc.lpfnWndProc   = WindowProc;
        wc.style         = CS_HREDRAW | CS_VREDRAW;
        wc.hInstance     = GetModuleHandle(NULL);
        wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
        wc.hIcon         = LoadIcon(NULL, IDI_SHIELD);
        wc.lpszClassName = getClassName();

        RegisterClass(&wc);

        m_hwnd = CreateWindowEx(
                dwExStyle, getClassName(), title, dwStyle, x, y,
                width, height, hWndParent, hMenu, GetModuleHandle(NULL), this);

        return (m_hwnd ? TRUE : FALSE);
    }

    HWND getWindowHandler() const
    { 
        return m_hwnd;
    }

protected:
    virtual LPCSTR  getClassName() const = 0;
    virtual LRESULT handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

    HWND m_hwnd;
};

#endif // _BASE_WINDOW_HPP_

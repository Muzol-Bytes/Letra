#ifndef _WINDOW_HPP_
#define _WINDOW_HPP_

#include <cstdint>

#include "base_window.hpp"

class Window : public BaseWindow<Window>
{
public:
    Window();

    LPCSTR  getClassName() const;

    LRESULT handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

    void destroy();

    void show();
};

#endif // _WINDOW_HPP_

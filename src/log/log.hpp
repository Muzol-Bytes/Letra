#ifndef _LOG_HPP_
#define _LOG_HPP_

#include <windows.h>
#include <strsafe.h>

void errorExit(const char lpszFunction[]);

DWORD showError(const char lpszFunction[]);

#endif // _LOG_HPP_

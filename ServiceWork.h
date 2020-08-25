#pragma once
#include <Windows.h>
#include <string>
#include <sstream>
#include <fstream>

inline HHOOK hHook = NULL;


LRESULT CALLBACK MyLowLevelKeyBoardProc(int nCode, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK workWith_WM_KEYDOWN(int nCode, WPARAM wParam,LPARAM lParam);
LRESULT CALLBACK workWith_WM_KEYUP(int nCode, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK workWith_WM_SYSKEYDOWN(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK workWith_WM_SYSKEYUP(int nCode, WPARAM wParam, LPARAM lParam);

void start();
void init(HINSTANCE hInstance);
LRESULT CALLBACK MyWndProc(HWND hWnd, UINT Message, UINT wParam, LONG lParam);
void sendInputText(std::string text);





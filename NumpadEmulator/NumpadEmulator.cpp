#include <iostream>
#include <Windows.h>
#include <string>

HHOOK hHook;

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == HC_ACTION)
    {
        if (wParam == WM_KEYDOWN)
        {
            PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam;
            if ((p->vkCode >= 48 && p->vkCode <= 57) && GetAsyncKeyState(VK_PAUSE) < 0)
            {
                int button = p->vkCode - 48;
                INPUT ip;
                ip.type = INPUT_KEYBOARD;
                ip.ki.wScan = 0;
                ip.ki.time = 0;
                ip.ki.dwExtraInfo = 0;
                ip.ki.wVk = VK_NUMPAD0 + button;
                ip.ki.dwFlags = 0;
                SendInput(1, &ip, sizeof(INPUT));
                Sleep(20);
                ip.ki.dwFlags = KEYEVENTF_KEYUP;
                SendInput(1, &ip, sizeof(INPUT));
                return 1;
            }
        }
    }
    return CallNextHookEx(hHook, nCode, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    srand(NULL);
    hHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, hInstance, 0);
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}
#include "Window.h"

KeepActive::Window KeepActive::Window::m_sWindow;

bool KeepActive::Window::Init()
{
    WNDCLASSEXA wclsx = {};
    wclsx.cbSize = sizeof(wclsx);
    wclsx.hInstance = GetModuleHandle(nullptr);
    wclsx.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW);
    wclsx.lpszClassName = WIN_NAME;
    wclsx.style = CS_VREDRAW | CS_HREDRAW;
    wclsx.hCursor = LoadCursor(0, IDC_ARROW);
    wclsx.hIcon = wclsx.hIconSm = LoadIcon(0, IDI_APPLICATION);
    wclsx.lpfnWndProc = Window::WndProc;

    if (!RegisterClassExA(&wclsx))
        return false;

    m_WinHandle = CreateWindowExA(0, WIN_NAME, WIN_NAME, WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX, 
        GetSystemMetrics(SM_CXSCREEN) / 2 - WIN_WIDTH / 2, GetSystemMetrics(SM_CYSCREEN) / 2 - WIN_HEIGHT / 2, WIN_WIDTH, WIN_HEIGHT,
        0, 0, wclsx.hInstance, nullptr);

    if (!m_WinHandle)
        return false;

    RECT rect = {};
    GetClientRect(m_WinHandle, &rect);
    
    m_Button = CreateWindowExA(0, "Button", BUTTON_TEXT_OFF, WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        0, 0, rect.right, rect.bottom, m_WinHandle, reinterpret_cast<HMENU>(BUTTON_HM_ID), wclsx.hInstance, nullptr);
    

    if (!m_Button)
        return false;

    return true;
}

void KeepActive::Window::Show()
{
    if (!m_WinHandle)
        return;

    ShowWindow(m_WinHandle, SW_SHOW);
    UpdateWindow(m_WinHandle);
}

void KeepActive::Window::Run()
{
    MSG msg = {};

    while (GetMessageA(&msg, 0, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }
}

LRESULT KeepActive::Window::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    static bool timerOn = false;

    switch (msg)
    {
    case WM_COMMAND:
    {
        if (LOWORD(wparam) == BUTTON_HM_ID)
        {
            if (timerOn)
            {
                KillTimer(hwnd, TIMER_ANTIIDLE_ID);
                timerOn = false;
                SetWindowTextA(Window::GetWindow().m_Button, BUTTON_TEXT_OFF);
            }
            else
            {
                SetTimer(hwnd, TIMER_ANTIIDLE_ID, 2000, nullptr);
                timerOn = true;
                SetWindowTextA(Window::GetWindow().m_Button, BUTTON_TEXT_ON);
            }
        }
        break;
    }
    case WM_TIMER:
    {
        if (wparam != TIMER_ANTIIDLE_ID) // Somehow
            break;
        INPUT inputData = {};
        inputData.type = INPUT_KEYBOARD;
        inputData.ki.wVk = VK_ESCAPE;
        inputData.ki.wScan = 1;
        inputData.ki.dwFlags = KEYEVENTF_SCANCODE;
        SendInput(1, &inputData, sizeof(INPUT));

        Sleep(300);

        inputData.ki.dwExtraInfo = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
        SendInput(1, &inputData, sizeof(INPUT));

        static bool positive = true;

        POINT mousePos = {};
        GetCursorPos(&mousePos);
        mousePos.x += (positive ? 1 : -1);

        inputData = {};
        inputData.type = INPUT_MOUSE;
        inputData.mi.dx = mousePos.x * 65536 / GetSystemMetrics(SM_CXSCREEN);
        inputData.mi.dy = mousePos.y * 65536 / GetSystemMetrics(SM_CYSCREEN);
        inputData.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
        SendInput(1, &inputData, sizeof(INPUT));

        SetCursorPos(mousePos.x, mousePos.y);
        
        positive = !positive;

        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        if (timerOn)
        {
            // From what I remember, we should destroy timers manually when destroyed
            KillTimer(hwnd, TIMER_ANTIIDLE_ID);
            timerOn = false;
        }
        break;
    default:
        return DefWindowProcA(hwnd, msg, wparam, lparam);
    }
    return 0;
}

#pragma once
#pragma comment(linker,"\"/manifestdependency:type='win32' \ name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \ processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#define WIN_NAME "KeepActive"
#define WIN_WIDTH  300
#define WIN_HEIGHT 200
#define BUTTON_TEXT_OFF "Start"
#define BUTTON_TEXT_ON "Stop"
#define BUTTON_HM_ID 0x69
#define TIMER_ANTIIDLE_ID 0x42
#define SINGLETON class
#define SINGLETON_STUFF public

namespace KeepActive
{
	SINGLETON Window
	{
	public:
		bool Init();
		void Show();

		void Run();
	public:
		static inline void ShowMsg(const char* msg) { MessageBoxA(0, msg, WIN_NAME, 0); };
	private:
		static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
	private:
		HWND m_WinHandle = 0;
		HWND m_Button = 0;

	SINGLETON_STUFF:
		static inline Window& GetWindow() { return m_sWindow; };
		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;
	private:
		Window() {};
		static Window m_sWindow;
	};
}
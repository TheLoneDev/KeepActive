#include "Window.h"

int main()
{
	auto& window = KeepActive::Window::GetWindow();

	if (!window.Init())
	{
		KeepActive::Window::ShowMsg("Error initializing window");
		return -1;
	}

	window.Show();
	window.Run();

	return 0;
}
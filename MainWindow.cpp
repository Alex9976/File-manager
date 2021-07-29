#include "MainWindow.hpp"
#include "WorkWindow.hpp"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WorkWindow wWindow;
	if (!wWindow.Initialize(hInstance))
	{
		MessageBox(nullptr, TEXT("Initialization failure"), TEXT("Error"), MB_OK | MB_ICONERROR);
	}

	return wWindow.MessageLoop();
}
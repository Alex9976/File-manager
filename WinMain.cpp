#include "WinMain.hpp"
#include "WorkWindow.hpp"
#include "resource.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WorkWindow wWindow;
	if (!wWindow.Initialize(hInstance))
	{
		MessageBox(nullptr, TEXT("MainWindow initialization failure"), TEXT("Error"), MB_OK | MB_ICONERROR);
	}

	HACCEL hAccel = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR1));

	if (!hAccel)
	{
		MessageBox(nullptr, TEXT("Cannot load accelerator"), TEXT("Error"), MB_OK | MB_ICONERROR);
	}

	return wWindow.MessageLoop(hAccel);
}
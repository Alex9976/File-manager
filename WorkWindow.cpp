#include "WorkWindow.hpp"
#include "WinMain.hpp"
#include "Procedures.hpp"
#include "resource.h"


BOOL WorkWindow::Initialize(HINSTANCE hInstance)
{
	WNDCLASSEX  wndClass;
	LPCWSTR className = TEXT("WorkWindow");

	ZeroMemory(&wndClass, sizeof(wndClass));

	wndClass.cbSize = sizeof(wndClass);
	wndClass.lpfnWndProc = WndProc;
	wndClass.lpszClassName = className;
	wndClass.hInstance = hInstance;
	wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wndClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LOGO));
	wndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU);
	wndClass.style = CS_DBLCLKS;

	if (RegisterClassEx(&wndClass) == NULL) return FALSE;

	hWnd = CreateWindow(className, TEXT("File Manager"), WS_OVERLAPPEDWINDOW, 100, 100,
		WINDOW_HEIGHT, WINDOW_WIDTH, NULL, NULL, hInstance, NULL);

	if (hWnd == NULL) 
		return FALSE;

	int nTimerID;
	nTimerID = SetTimer(hWnd, TIMER_ID, TIMER_DELAY, NULL);

	ShowWindow(hWnd, SW_SHOWNORMAL);
	UpdateWindow(hWnd);

	return TRUE;
}

WPARAM WorkWindow::MessageLoop(HACCEL hAccel)
{
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateAccelerator(hWnd, hAccel, &msg);
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	KillTimer(hWnd, TIMER_ID);

	return msg.wParam;
}
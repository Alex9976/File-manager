#pragma once
#include <Windows.h>

class WorkWindow
{
private:
	const int WINDOW_HEIGHT = 1152;
	const int WINDOW_WIDTH = 648;
protected:
	HWND hWnd;
	SIZE sWindowSize;
public:
	WorkWindow() : hWnd(NULL) {}
	BOOL Initialize(HINSTANCE hInstance);
	WPARAM MessageLoop(HACCEL hAccel);
};
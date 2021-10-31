#pragma once
#include <Windows.h>

#define TIMER_ID 666
#define TIMER_DELAY 200

class WorkWindow
{
private:
	const int WINDOW_HEIGHT = 1152;
	const int WINDOW_WIDTH = 648;
protected:
	HWND hWnd;
	SIZE sWindowSize;
public:
	BOOL Initialize(HINSTANCE hInstance);
	WPARAM MessageLoop(HACCEL hAccel);
};
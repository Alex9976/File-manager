#include "Procedures.hpp"
#include "resource.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_COMMAND:
		CommandProcedure(hWnd, uMsg, wParam, lParam);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
}

LRESULT CommandProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case ID_FILE_EXIT:
		SendMessage(hWnd, WM_CLOSE, NULL, NULL);
		break;
	case ID_HELP_ABOUT:
		MessageBox(nullptr, TEXT("A simple file manager"), TEXT("About File Manager"), MB_OK | MB_ICONINFORMATION);
		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
}
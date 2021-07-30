#include "Procedures.hpp"
#include "resource.h"
#include "WorkPane.hpp"

WorkPane leftWorkPane, rightWorkPane;

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
		if (!leftWorkPane.Initialize(hWnd, ID_LEFTCOMBOBOX, ID_LEFTLISTVIEW, ID_LEFTTEXT, TRUE))
		{
			MessageBox(nullptr, TEXT("Left work pane initialization failure"), TEXT("Error"), MB_OK | MB_ICONERROR);
		}
		leftWorkPane.isSelected = TRUE;
		if (!rightWorkPane.Initialize(hWnd, ID_RIGHTCOMBOBOX, ID_RIGHTLISTVIEW, ID_RIGHTTEXT, FALSE))
		{
			MessageBox(nullptr, TEXT("Right work pane nitialization failure"), TEXT("Error"), MB_OK | MB_ICONERROR);
		}
		break;
	case WM_SIZE:
		leftWorkPane.Resize(hWnd, lParam);
		rightWorkPane.Resize(hWnd, lParam);
		break;
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
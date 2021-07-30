#pragma once
#include <Windows.h>

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

LRESULT CommandProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

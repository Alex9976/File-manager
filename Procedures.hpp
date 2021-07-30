#pragma once
#include <Windows.h>

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

LRESULT CommandProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

LRESULT Notify(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void CopyToBufferr(HWND hList, TCHAR* tPath);

void CopyOrMoveFiles(HWND hList, TCHAR* tPath);

BOOL CALLBACK NewFileDialog(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void CreateNew(TCHAR* tPath);

BOOL CALLBACK OpenDialog(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
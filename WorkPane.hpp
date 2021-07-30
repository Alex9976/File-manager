#pragma once
#pragma comment(lib, "comctl32.lib")
#include <Windows.h>

class WorkPane
{
private:
	BOOL isLeft;
	BOOL CreateListView(HWND hWnd, UINT uId);
	void ListViewInitialize();
	BOOL CreateComboBox(HWND hWnd, UINT uId);
	void ComboBoxInit();
	BOOL CreatePathLabel(HWND hWnd, UINT uId);
public: 
	HWND hListView, hLabel, hComboBox;
	BOOL isSelected;
	BOOL Initialize(HWND hWnd, UINT uComboId, UINT uListId, UINT uTextId, BOOL isLeft);
	void Resize(HWND hWnd, LPARAM lParam);
};


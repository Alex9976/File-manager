#pragma once
#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "Shlwapi.lib")
#include <Windows.h>
#include "VolumeInfo.hpp"

class WorkPane
{
private:
	BOOL isLeft;
	BOOL CreateListView(HWND hWnd, UINT uId);
	void ListViewInitialize();
	BOOL CreateComboBox(HWND hWnd, UINT uId);
	void ComboBoxInit();
	BOOL CreatePathLabel(HWND hWnd, UINT uId);
	BOOL CreateSizeLabel(HWND hWnd, UINT uId);
	TCHAR currentPath[256];
	void CreateIconList();
	int GetIconIndex(WIN32_FIND_DATA data);
	void InsertItem(UINT uItem, WIN32_FIND_DATA data, HANDLE hFile);
public: 
	HWND hListView, hLabel, hComboBox, hSizeLabel;
	BOOL isSelected;
	BOOL Initialize(HWND hWnd, UINT uComboId, UINT uListId, UINT uTextId, BOOL isLeft);
	void Resize(HWND hWnd, LPARAM lParam);
	VolumeInfo volumeInfo;
	LPWSTR GetCurrentPath();
	void SetCurrentPath(TCHAR* path);
	WorkPane();
	void UpdateList(TCHAR* path);
	void UpdateSizes();
};


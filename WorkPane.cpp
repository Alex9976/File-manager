#include "WorkPane.hpp"
#include <CommCtrl.h>
#include <windowsx.h>
#include "Convert.hpp"
#include "resource.h"
#include <tchar.h>
#include <Shlwapi.h>
#include "Constants.hpp"

const TCHAR* sizes[] = { L"bytes", L"KB", L"MB", L"GB", L"TB", L"PB", L"EB", L"ZB", L"EB" };

BOOL WorkPane::Initialize(HWND hWnd, UINT uComboId, UINT uListId, UINT uTextId, BOOL isLeft)
{
	this->isLeft = isLeft;

	InitCommonControls();

	if (!CreateListView(hWnd, uListId))
	{
		MessageBox(nullptr, TEXT("List view creation falture"), TEXT("Error"), MB_OK | MB_ICONERROR);
		return FALSE;
	}

	if (!CreateComboBox(hWnd, uComboId))
	{
		MessageBox(nullptr, TEXT("Combobox creation falture"), TEXT("Error"), MB_OK | MB_ICONERROR);
		return FALSE;
	}
	

	if (!CreatePathLabel(hWnd, uTextId))
	{
		MessageBox(nullptr, TEXT("Path label creation falture"), TEXT("Error"), MB_OK | MB_ICONERROR);
		return FALSE;
	}

	if (!CreateSizeLabel(hWnd, uTextId))
	{
		MessageBox(nullptr, TEXT("Size label creation falture"), TEXT("Error"), MB_OK | MB_ICONERROR);
		return FALSE;
	}

	ListViewInitialize();
	ComboBoxInit();
	UpdateSizes();

	auto hNormalFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	SendMessage(hListView, WM_SETFONT, (WPARAM)hNormalFont, 0);
	SendMessage(hComboBox, WM_SETFONT, (WPARAM)hNormalFont, 0);
	SendMessage(hLabel, WM_SETFONT, (WPARAM)hNormalFont, 0);
	SendMessage(hSizeLabel, WM_SETFONT, (WPARAM)hNormalFont, 0);

	return TRUE;
}

BOOL WorkPane::CreateListView(HWND hWnd, UINT uId)
{
	hListView = CreateWindowEx(WS_EX_CLIENTEDGE, WC_LISTVIEW, TEXT("ListView"), WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_EDITLABELS | LVS_SHOWSELALWAYS,
		0, 0, 0, 0, hWnd, (HMENU)uId, nullptr, nullptr);

	ListView_SetExtendedListViewStyle(hListView, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	if (hListView == nullptr)
		return FALSE;
	else return TRUE;
}

void WorkPane::ListViewInitialize()
{
	CreateIconList();

	LVCOLUMN lvc;
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	lvc.fmt = LVCFMT_LEFT;
	lvc.cx = 300;
	lvc.pszText = (LPWSTR)L"Name";
	lvc.iSubItem = 0;
	lvc.iOrder = 0;
	ListView_InsertColumn(hListView, 0, &lvc);

	lvc.cx = 144;
	lvc.pszText = (LPWSTR)L"Size";
	lvc.iSubItem = 1;
	ListView_InsertColumn(hListView, 1, &lvc);

	lvc.cx = 120;
	lvc.pszText = (LPWSTR)L"Date";
	lvc.iSubItem = 2;
	ListView_InsertColumn(hListView, 2, &lvc);

	UpdateList(volumeInfo.path);
	SetCurrentPath(volumeInfo.path);	
}

void WorkPane::ListViewUpdateColumnSize(SIZE size)
{
	ListView_DeleteColumn(hListView, 2);
	ListView_DeleteColumn(hListView, 1);
	ListView_DeleteColumn(hListView, 0);

	LVCOLUMN lvc;
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	lvc.fmt = LVCFMT_LEFT;
	lvc.cx = size.cx * 25 / 47;
	lvc.pszText = (LPWSTR)L"Name";
	lvc.iSubItem = 0;
	lvc.iOrder = 0;
	ListView_InsertColumn(hListView, 0, &lvc);

	lvc.cx = size.cx * 12 / 47;
	lvc.pszText = (LPWSTR)L"Size";
	lvc.iSubItem = 1;
	ListView_InsertColumn(hListView, 1, &lvc);

	lvc.cx = size.cx * 10 / 47 - 3;
	lvc.pszText = (LPWSTR)L"Date";
	lvc.iSubItem = 2;
	ListView_InsertColumn(hListView, 2, &lvc);

	UpdateList(volumeInfo.path);
}

void WorkPane::Resize(HWND hWnd, LPARAM lParam)
{
	SIZE Size;
	Size.cx = LOWORD(lParam) / 2;
	Size.cy = HIWORD(lParam);
	ListViewUpdateColumnSize(Size);
	if (isLeft)
	{
		MoveWindow(hComboBox, 1, 0, 50, 20, TRUE);
		MoveWindow(hLabel, 0, 20, Size.cx, 20, TRUE);
		MoveWindow(hSizeLabel, 51, 00, Size.cx - 51, 20, TRUE);
		MoveWindow(hListView, 0, 40, Size.cx, Size.cy - 45, TRUE);
	}
	else
	{
		MoveWindow(hComboBox, Size.cx + 1, 0, 50, 2, TRUE);
		MoveWindow(hLabel, Size.cx, 20, Size.cx, 20, TRUE);
		MoveWindow(hSizeLabel, Size.cx + 51, 0, Size.cx - 51, 20, TRUE);
		MoveWindow(hListView, Size.cx, 40, Size.cx, Size.cy - 45, TRUE);
	}
}

BOOL WorkPane::CreateComboBox(HWND hWnd, UINT uId)
{
	hComboBox = CreateWindowEx(WS_EX_CLIENTEDGE, WC_COMBOBOX, TEXT("ComboBox"), WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST,
		0, 0, 0, 100, hWnd, (HMENU)uId, nullptr, nullptr);

	if (hComboBox == nullptr)
		return FALSE;
	else return TRUE;
}

void WorkPane::ComboBoxInit()
{
	TCHAR drives[10][4];
	UINT uNumber = volumeInfo.GetNumberOfVolumes(drives);

	TCHAR A[40];
	memset(&A, 0, sizeof(A));

	for (size_t k = 0; k <= uNumber - 1; k++)
	{
		wcscpy_s(A, sizeof(A) / sizeof(TCHAR), (TCHAR*)drives[k]);
		SendMessage(hComboBox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)A);
	}

	SendMessage(hComboBox, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
	size_t length = ComboBox_GetTextLength(hComboBox);
	auto string = (LPWSTR)GlobalAlloc(GPTR, length + 1);
	ComboBox_GetText(hComboBox, string, length + 1);

	SetCurrentPath(string);
	volumeInfo.Update(string);
}

BOOL WorkPane::CreatePathLabel(HWND hWnd, UINT uId)
{
	hLabel = CreateWindowEx(WS_EX_CLIENTEDGE, WC_EDIT, nullptr, WS_CHILD | WS_VISIBLE | WS_BORDER | WS_DISABLED | WS_EX_LEFT,
		0, 0, 0, 0, hWnd, (HMENU)uId, nullptr, nullptr);

	if (hLabel == nullptr)
		return FALSE;
	else return TRUE;
}

void WorkPane::SetCurrentPath(TCHAR* path)
{
	lstrcpyn(currentPath, path, 256);
}

LPWSTR WorkPane::GetCurrentPath()
{
	return currentPath;
}

WorkPane::WorkPane() : listViewItemsCount(0), cycleCount(0)
{
	memset(currentPath, '\0', 256);
}

void WorkPane::UpdateList(TCHAR* _path, BOOL isTimerRefresh)
{
	auto* oldPath = new TCHAR[256];
	memset(oldPath, '\0', 256);
	wcscpy(oldPath, currentPath);

	if (isTimerRefresh)
	{
		int length = lstrlen(_path) + lstrlen(TEXT("*.*"));
		auto* path = new TCHAR[length];
		wcscpy(path, _path);
		wcscat(path, TEXT("*.*"));

		WIN32_FIND_DATA data;
		HANDLE hFile = FindFirstFile(path, &data);

		int i = 0;
		while (FindNextFile(hFile, &data))
		{
			i++;
		}

		++this->cycleCount;

		if (this->cycleCount <= MAX_CYCLE_COUNT)
		{
			if (this->listViewItemsCount == i)
				return;
		}
		else
		{
			this->cycleCount = 0;
		}
		
	}

	this->listViewItemsCount = 0;

	ListView_DeleteAllItems(hListView);

	int length = lstrlen(_path) + lstrlen(TEXT("*.*"));
	auto* path = new TCHAR[length];
	wcscpy(path, _path);
	Edit_SetText(hLabel, path);
	SetCurrentPath(path);
	wcscat(path, TEXT("*.*"));

	WIN32_FIND_DATA data;
	HANDLE hFile = FindFirstFile(path, &data);

	SendMessage(hListView, LB_RESETCONTENT, 0, 0);

	int i = 0;
	while (FindNextFile(hFile, &data))
	{
		InsertItem(i, data, hFile);
		i++;
	}

	this->listViewItemsCount = i;

	if (i == 0 && (MessageBox(nullptr, TEXT("Access denied"), TEXT("Error"), MB_OK | MB_ICONERROR) == IDOK))
	{
		UpdateList(oldPath);
	}

	UpdateSizes();

	FindClose(hFile);
}

void WorkPane::InsertItem(UINT uItem, WIN32_FIND_DATA data, HANDLE hFile)
{
	LVITEM lvi;
	lvi.mask = LVIF_TEXT | LVIF_IMAGE;

	lvi.pszText = data.cFileName;
	lvi.iItem = uItem;
	lvi.iSubItem = 0;
	lvi.iImage = GetIconIndex(data);
	ListView_InsertItem(hListView, &lvi);

	if (lvi.iImage == 1 || lvi.iImage == 2)
		lvi.pszText = (LPWSTR)L"Folder";
	else
	{
		LPWSTR s1 = GetLPWSTR(data.nFileSizeHigh * (MAXDWORD + 1) + data.nFileSizeLow), s2 = (LPWSTR)L" bytes";
		int lenght = lstrlen(s1) + lstrlen(s2);
		auto* tmp = new TCHAR[lenght];
		wcscpy(tmp, s1);
		wcscat(tmp, s2);

		lvi.pszText = tmp;
	}
	lvi.iSubItem = 1;
	ListView_SetItem(hListView, &lvi);

	lvi.pszText = GetLPWSTR(data.ftLastWriteTime);
	lvi.iSubItem = 2;
	ListView_SetItem(hListView, &lvi);
}

void WorkPane::CreateIconList()
{
	HIMAGELIST hImageList = ImageList_Create(16, 16, ILC_COLOR32, 8, 0);
	HBITMAP hBmp = nullptr;

	hBmp = LoadBitmap(GetModuleHandle(nullptr), MAKEINTRESOURCE(IDB_FILE));
	ImageList_Add(hImageList, hBmp, (HBITMAP)nullptr);
	DeleteObject(hBmp);

	hBmp = LoadBitmap(GetModuleHandle(nullptr), MAKEINTRESOURCE(IDB_FOLDER));
	ImageList_Add(hImageList, hBmp, (HBITMAP)nullptr);
	DeleteObject(hBmp);

	hBmp = LoadBitmap(GetModuleHandle(nullptr), MAKEINTRESOURCE(IDB_FOLDER_OPEN));
	ImageList_Add(hImageList, hBmp, (HBITMAP)nullptr);
	DeleteObject(hBmp);

	hBmp = LoadBitmap(GetModuleHandle(nullptr), MAKEINTRESOURCE(IDB_EXE));
	ImageList_Add(hImageList, hBmp, (HBITMAP)nullptr);
	DeleteObject(hBmp);

	hBmp = LoadBitmap(GetModuleHandle(nullptr), MAKEINTRESOURCE(IDB_AUDIO));
	ImageList_Add(hImageList, hBmp, (HBITMAP)nullptr);
	DeleteObject(hBmp);

	hBmp = LoadBitmap(GetModuleHandle(nullptr), MAKEINTRESOURCE(IDB_VIDEO));
	ImageList_Add(hImageList, hBmp, (HBITMAP)nullptr);
	DeleteObject(hBmp);

	hBmp = LoadBitmap(GetModuleHandle(nullptr), MAKEINTRESOURCE(IDB_IMAGE));
	ImageList_Add(hImageList, hBmp, (HBITMAP)nullptr);
	DeleteObject(hBmp);

	hBmp = LoadBitmap(GetModuleHandle(nullptr), MAKEINTRESOURCE(IDB_DOCUMENT));
	ImageList_Add(hImageList, hBmp, (HBITMAP)nullptr);
	DeleteObject(hBmp);

	ListView_SetImageList(hListView, hImageList, LVSIL_SMALL);
}

int WorkPane::GetIconIndex(WIN32_FIND_DATA data)
{
	LPWSTR lpsPath = ConnectTwoString(this->currentPath, (LPWSTR)data.cFileName, TRUE);
	LPWSTR lpsExt = PathFindExtension(lpsPath);

	if ((!_tcscmp(lpsExt, TEXT("")) || !_tcscmp(data.cFileName, TEXT(".."))) && data.nFileSizeLow == 0)
	{
		return !_tcscmp(data.cFileName, TEXT("..")) ? 2 : 1;
	}
	else
	{
		for (int i = 0; i < MAX_COUNT_FORMATS; i++)
		{
			if (!_tcscmp(lpsExt, exeFormat[i]))
				return 3;
			if (!_tcscmp(lpsExt, audioFormat[i])) 
				return 4;
			if (!_tcscmp(lpsExt, videoFormat[i])) 
				return 5;
			if (!_tcscmp(lpsExt, imageFormat[i])) 
				return 6;
			if (!_tcscmp(lpsExt, documentFormat[i])) 
				return 7;
		}
	}
	return 0;
}

BOOL WorkPane::CreateSizeLabel(HWND hWnd, UINT uId)
{
	hSizeLabel = CreateWindowEx(WS_EX_CLIENTEDGE, WC_EDIT, nullptr, WS_CHILD | WS_VISIBLE | WS_BORDER | WS_DISABLED | WS_EX_LEFT,
		0, 0, 0, 0, hWnd, (HMENU)uId, nullptr, nullptr);

	if (hSizeLabel == nullptr)
		return FALSE;
	else return TRUE;
}

void WorkPane::UpdateSizes()
{	
	volumeInfo.Update(this->GetCurrentPath());
	TCHAR buf[512];
	DWORDLONG freeS = volumeInfo.freeSpace, fullS = volumeInfo.fullSpace;
	int i = 0, j = 0;
	while (freeS / 1024 >= 1)
	{
		i++;
		freeS /= 1024;
	}
	while (fullS / 1024 >= 1)
	{
		j++;
		fullS /= 1024;
	}
	_stprintf(buf, TEXT("Free %I64u bytes [%I64u %s] of %I64u bytes [%I64u %s]"), volumeInfo.freeSpace, freeS, sizes[i], volumeInfo.fullSpace, fullS, sizes[j]);
	Edit_SetText(hSizeLabel, buf);
}
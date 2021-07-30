#include "WorkPane.hpp"
#include <CommCtrl.h>
#include <windowsx.h>

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
	ComboBoxInit();

	if (!CreatePathLabel(hWnd, uTextId))
	{
		MessageBox(nullptr, TEXT("Path label creation falture"), TEXT("Error"), MB_OK | MB_ICONERROR);
		return FALSE;
	}

	ListViewInitialize();

	auto hNormalFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	SendMessage(hListView, WM_SETFONT, (WPARAM)hNormalFont, 0);
	SendMessage(hComboBox, WM_SETFONT, (WPARAM)hNormalFont, 0);
	SendMessage(hLabel, WM_SETFONT, (WPARAM)hNormalFont, 0);

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
	LVCOLUMN lvc;
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	lvc.fmt = LVCFMT_LEFT;
	lvc.cx = 300;
	lvc.pszText = (LPWSTR)L"Name";
	lvc.iSubItem = 0;
	lvc.iOrder = 0;
	ListView_InsertColumn(hListView, 0, &lvc);

	lvc.cx = 134;
	lvc.pszText = (LPWSTR)L"Size";
	lvc.iSubItem = 1;
	ListView_InsertColumn(hListView, 1, &lvc);

	lvc.cx = 130;
	lvc.pszText = (LPWSTR)L"Date";
	lvc.iSubItem = 2;
	ListView_InsertColumn(hListView, 2, &lvc);
}

void WorkPane::Resize(HWND hWnd, LPARAM lParam)
{
	SIZE Size;
	Size.cx = LOWORD(lParam) / 2;
	Size.cy = HIWORD(lParam);
	if (isLeft)
	{
		MoveWindow(hComboBox, 1, 0, 50, 20, TRUE);
		MoveWindow(hLabel, 0, 20, Size.cx, 20, TRUE);
		MoveWindow(hListView, 0, 40, Size.cx, Size.cy - 45, TRUE);
	}
	else
	{
		MoveWindow(hComboBox, Size.cx + 1, 0, 50, 2, TRUE);
		MoveWindow(hLabel, Size.cx, 20, Size.cx, 20, TRUE);
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
	SendMessage(hComboBox, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
	size_t length = ComboBox_GetTextLength(hComboBox);
	auto string = (LPWSTR)GlobalAlloc(GPTR, length + 1);
	ComboBox_GetText(hComboBox, string, length + 1);
}

BOOL WorkPane::CreatePathLabel(HWND hWnd, UINT uId)
{
	hLabel = CreateWindowEx(WS_EX_CLIENTEDGE, WC_EDIT, nullptr, WS_CHILD | WS_VISIBLE | WS_BORDER | WS_DISABLED | WS_EX_LEFT,
		0, 0, 0, 0, hWnd, (HMENU)uId, nullptr, nullptr);

	if (hLabel == nullptr)
		return FALSE;
	else return TRUE;
}
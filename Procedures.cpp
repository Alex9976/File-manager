#include "Procedures.hpp"
#include "resource.h"
#include "WorkPane.hpp"
#include <Windows.h>
#include <windowsx.h>
#include <CommCtrl.h>
#include <wchar.h>
#include <string.h>
#include <Shlwapi.h>
#include <tchar.h>
#include "Convert.hpp"

WorkPane leftWorkPane, rightWorkPane;
POINT pPoint;
TCHAR* tBuffer0 = new TCHAR[256], * tBuffer1 = new TCHAR[256], ** tCopyBuffer = NULL;
int countCopyBuffer = 0;
BOOL cutMode;

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
	case WM_NOTIFY:
		Notify(hWnd, uMsg, wParam, lParam);
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
	case ID_LEFTCOMBOBOX:
		leftWorkPane.isSelected = TRUE;
		rightWorkPane.isSelected = FALSE;

		if (HIWORD(wParam) == CBN_SELENDOK)
		{
			ComboBox_GetCurSel(leftWorkPane.hComboBox);
			size_t length = ComboBox_GetTextLength(leftWorkPane.hComboBox);

			auto lpsString = (LPWSTR)GlobalAlloc(GPTR, length + 1);
			ComboBox_GetText(leftWorkPane.hComboBox, lpsString, length + 1);

			leftWorkPane.volumeInfo.Update(lpsString);
			leftWorkPane.UpdateList(leftWorkPane.volumeInfo.path);

			GlobalFree(lpsString);
		}
		break;
	case ID_RIGHTCOMBOBOX:
		rightWorkPane.isSelected = TRUE;
		leftWorkPane.isSelected = FALSE;

		if (HIWORD(wParam) == CBN_SELENDOK)
		{
			ComboBox_GetCurSel(rightWorkPane.hComboBox);
			size_t length = ComboBox_GetTextLength(rightWorkPane.hComboBox);

			auto lpsString = (LPWSTR)GlobalAlloc(GPTR, length + 1);
			ComboBox_GetText(rightWorkPane.hComboBox, lpsString, length + 1);

			rightWorkPane.volumeInfo.Update(lpsString);
			rightWorkPane.UpdateList(rightWorkPane.volumeInfo.path);

			GlobalFree(lpsString);
		}
		break;
	case ID_NEW:
	case ID_FILE_NEW:
		DialogBox(NULL, MAKEINTRESOURCE(IDD_FILE_NEW_DIALOG), hWnd, (DLGPROC)NewFileDialog);
		break;
	case ID_OPEN:
	case ID_FILE_OPEN:
		DialogBox(NULL, MAKEINTRESOURCE(IDD_FILE_NEW_DIALOG), hWnd, (DLGPROC)OpenDialog);
		break;
	case ID_FILE_EXIT:
		SendMessage(hWnd, WM_CLOSE, NULL, NULL);
		break;
	case ID_COPY:
	case ID_EDIT_COPY:
		cutMode = FALSE;
		if (leftWorkPane.isSelected)
			CopyToBufferr(leftWorkPane.hListView, leftWorkPane.GetCurrentPath());
		else
			CopyToBufferr(rightWorkPane.hListView, rightWorkPane.GetCurrentPath());
		break;
	case ID_CUT:
	case ID_EDIT_CUT:
		cutMode = TRUE;
		if (leftWorkPane.isSelected)
			CopyToBufferr(rightWorkPane.hListView, rightWorkPane.GetCurrentPath());
		else
			CopyToBufferr(rightWorkPane.hListView, rightWorkPane.GetCurrentPath());
		break;
	case ID_PASTE:
	case ID_EDIT_PASTE:
		if (leftWorkPane.isSelected)
		{
			CopyOrMoveFiles(leftWorkPane.hListView, leftWorkPane.GetCurrentPath());
		}
		else
		{
			CopyOrMoveFiles(rightWorkPane.hListView, rightWorkPane.GetCurrentPath());
		}
		leftWorkPane.UpdateList(leftWorkPane.GetCurrentPath());
		rightWorkPane.UpdateList(rightWorkPane.GetCurrentPath());
		break;
	case ID_DEL:
	case ID_EDIT_DELETE:
		//TODO: Add MessageBox

		SendMessage(hWnd, WM_COMMAND, ID_EDIT_COPY, NULL);
		if (tCopyBuffer)
		{
			for (int i = 0; i < countCopyBuffer; i++)
			{
				if (PathIsDirectory(tCopyBuffer[i]))
				{
					if (!PathIsDirectoryEmpty(tCopyBuffer[i]))
					{
						MessageBox(nullptr, TEXT("Directory is not empty! Remove content!"), TEXT("Warning"), MB_OK | MB_ICONWARNING);
					}
					RemoveDirectory(tCopyBuffer[i]);
				}
				else DeleteFile(tCopyBuffer[i]);

				leftWorkPane.UpdateList(leftWorkPane.GetCurrentPath());
				rightWorkPane.UpdateList(rightWorkPane.GetCurrentPath());

				delete[] tCopyBuffer[i];
			}
			delete[] tCopyBuffer;
		}
		break;
	case ID_INFO:
	case ID_HELP_ABOUT:
		MessageBox(nullptr, TEXT("This is a simple file manager"), TEXT("About File Manager"), MB_OK | MB_ICONINFORMATION);
		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
}

LRESULT Notify(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	auto lpnmItem = (LPNMITEMACTIVATE)lParam;
	bool isBack = false;

	if ((((LPNMHDR)lParam)->hwndFrom) == GetDlgItem(hWnd, ID_LEFTLISTVIEW))
	{
		switch (((LPNMHDR)lParam)->code)
		{
		case NM_RCLICK:
			leftWorkPane.isSelected = TRUE;
			rightWorkPane.isSelected = FALSE;

			ListView_GetItemPosition(leftWorkPane.hListView, lpnmItem->iItem, &pPoint);
			ClientToScreen(leftWorkPane.hListView, &pPoint);

			TrackPopupMenu(GetSubMenu(LoadMenu(NULL, MAKEINTRESOURCE(IDR_MENU)), 1), 0, pPoint.x + 50, pPoint.y, 0, hWnd, NULL);

			break;
		case NM_CLICK:
			leftWorkPane.isSelected = TRUE;
			rightWorkPane.isSelected = FALSE;
			break;

		case NM_DBLCLK:
			memset(tBuffer0, '\0', 256);
			memset(tBuffer1, '\0', 256);
			isBack = false;

			ListView_GetItemText(leftWorkPane.hListView, lpnmItem->iItem, 0, tBuffer0, 256);
			if (!_tcscmp(tBuffer0, TEXT("..")))
				isBack = true;

			if (isBack)
			{
				tBuffer1 = GetPastPath(leftWorkPane.GetCurrentPath());
			}
			else
			{
				tBuffer1 = ConnectTwoString(leftWorkPane.GetCurrentPath(), tBuffer0, FALSE);
			}

			memset(tBuffer0, '\0', 256);
			ListView_GetItemText(leftWorkPane.hListView, lpnmItem->iItem, 1, tBuffer0, 256);

			if (!_tcscmp(tBuffer0, TEXT("Folder")))
			{
				leftWorkPane.UpdateList(tBuffer1);
			}
			else
			{
				ShellExecute(nullptr, TEXT("open"), tBuffer1, nullptr, nullptr, SW_SHOWNORMAL);
			}
			break;
		default:
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
		}
	}
	else if ((((LPNMHDR)lParam)->hwndFrom) == GetDlgItem(hWnd, ID_RIGHTLISTVIEW))
	{
		switch (((LPNMHDR)lParam)->code)
		{
		case NM_RCLICK:
			rightWorkPane.isSelected = TRUE;
			leftWorkPane.isSelected = FALSE;

			ListView_GetItemPosition(rightWorkPane.hListView, lpnmItem->iItem, &pPoint);
			ClientToScreen(rightWorkPane.hListView, &pPoint);

			TrackPopupMenu( GetSubMenu(LoadMenu(NULL, MAKEINTRESOURCE(IDR_MENU)), 1), 0, pPoint.x + 50, pPoint.y, 0, hWnd, NULL);

			break;
		case NM_CLICK:
			rightWorkPane.isSelected = TRUE;
			leftWorkPane.isSelected = FALSE;
			break;

		case NM_DBLCLK:
			memset(tBuffer0, '\0', 256);
			memset(tBuffer1, '\0', 256);
			isBack = false;

			ListView_GetItemText(rightWorkPane.hListView, lpnmItem->iItem, 0, tBuffer0, 256);
			if (!_tcscmp(tBuffer0, TEXT("..")))
				isBack = true;

			if (isBack)
			{
				tBuffer1 = GetPastPath(rightWorkPane.GetCurrentPath());
			}
			else
			{
				tBuffer1 = ConnectTwoString(rightWorkPane.GetCurrentPath(), tBuffer0, FALSE);
			}

			memset(tBuffer0, '\0', 256);
			ListView_GetItemText(rightWorkPane.hListView, lpnmItem->iItem, 1, tBuffer0, 256);

			if (!_tcscmp(tBuffer0, TEXT("Folder")))
			{
				rightWorkPane.UpdateList(tBuffer1);
			}
			else
			{
				ShellExecute(nullptr, TEXT("open"), tBuffer1, nullptr, nullptr, SW_SHOWNORMAL);
			}
			break;
		default:
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
		}
	}
}

void CopyToBufferr(HWND hList, TCHAR* tPath)
{
	countCopyBuffer = ListView_GetSelectedCount(hList);

	tCopyBuffer = new TCHAR * [countCopyBuffer];
	auto position = ListView_GetNextItem(hList, -1, LVNI_SELECTED);
	for (int i = 0; i < countCopyBuffer; i++)
	{
		tCopyBuffer[i] = new TCHAR[256];

		memset(tBuffer0, '\0', 256);
		ListView_GetItemText(hList, position, 0, tBuffer0, 256);
		tCopyBuffer[i] = ConnectTwoString(tPath, tBuffer0, TRUE);

		position = ListView_GetNextItem(hList, position, LVNI_SELECTED);
	}
}

void CopyOrMoveFiles(HWND hList, TCHAR* tPath)
{
	if (tCopyBuffer)
	{
		for (int i = 0; i < countCopyBuffer; i++)
		{
			memset(tBuffer0, '\0', 256);
			tBuffer0 = PathFindFileName(tCopyBuffer[i]);

			memset(tBuffer1, '\0', 256);
			tBuffer1 = ConnectTwoString(tPath, tBuffer0, TRUE);

			if (cutMode)
				MoveFile(tCopyBuffer[i], tBuffer1);
			else
				CopyFile(tCopyBuffer[i], tBuffer1, FALSE);

			delete[] tCopyBuffer[i];
		}
		delete[] tCopyBuffer;
		tCopyBuffer = nullptr;
		countCopyBuffer = 0;
	}
}

BOOL CALLBACK NewFileDialog(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HWND hTextField = GetDlgItem(hWnd, IDC_EDIT1);

	switch (uMsg)
	{
	case WM_INITDIALOG:
		if (leftWorkPane.isSelected == TRUE)
			SetWindowText(hWnd, leftWorkPane.GetCurrentPath());
		else
			SetWindowText(hWnd, rightWorkPane.GetCurrentPath());
		break;
	case WM_COMMAND:
		memset(tBuffer0, '\0', 256);
		memset(tBuffer1, '\0', 256);

		switch (LOWORD(wParam))
		{
		case IDOK:
			Edit_GetText(hTextField, tBuffer0, 256);

			if (leftWorkPane.isSelected)
			{
				CreateNew(leftWorkPane.GetCurrentPath());
			}
			else
			{
				CreateNew(rightWorkPane.GetCurrentPath());
			}
			leftWorkPane.UpdateList(leftWorkPane.GetCurrentPath());
			rightWorkPane.UpdateList(rightWorkPane.GetCurrentPath());
		case IDCANCEL:
			EndDialog(hWnd, NULL);
			break;
		default:
			break;
		}
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

void CreateNew(TCHAR* tPath)
{
	tBuffer1 = ConnectTwoString(tPath, tBuffer0, TRUE);
	memset(tBuffer0, '\0', 256);
	tBuffer0 = PathFindExtension(tBuffer1);

	if (!_tcscmp(tBuffer0, TEXT("")))
	{
		wcscat(tBuffer1, TEXT("\\"));
		CreateDirectory(tBuffer1, nullptr);
	}
	else
	{
		HANDLE hAppend = CreateFile(tBuffer1, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, CREATE_NEW,
			FILE_ATTRIBUTE_NORMAL, nullptr);
		CloseHandle(hAppend);
	}
}

BOOL CALLBACK OpenDialog(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HWND hTextField = GetDlgItem(hWnd, IDC_EDIT1);

	switch (uMsg)
	{
	case WM_INITDIALOG:
		SetWindowText(hWnd, TEXT("Open"));
		break;
	case WM_COMMAND:
		memset(tBuffer0, '\0', 10);
		memset(tBuffer1, '\0', 256);

		switch (LOWORD(wParam))
		{
		case IDOK:
			Edit_GetText(hTextField, tBuffer0, 256);

			tBuffer1 = PathFindExtension(tBuffer0);

			if (!_tcscmp(tBuffer1, TEXT("")))
			{
				if (leftWorkPane.isSelected)
					leftWorkPane.UpdateList(ConnectTwoString(tBuffer0, (LPWSTR)L"\\", TRUE));
				else
					rightWorkPane.UpdateList(ConnectTwoString(tBuffer0, (LPWSTR)L"\\", TRUE));
			}
			else
				ShellExecute(nullptr, TEXT("open"), tBuffer0, nullptr, nullptr, SW_SHOWNORMAL);

		case IDCANCEL:
			EndDialog(hWnd, NULL);
			break;
		default:
			break;
		}
		break;
	default:
		return FALSE;
	}
	return TRUE;
}
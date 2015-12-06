// metabuilder.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "metabuilder.h"
#include <commctrl.h>
#include <string>

using namespace std;

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    AboutDlgProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    AccountDlgProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    SizeAndStopsDlgProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    DataIntervalDlgProc(HWND, UINT, WPARAM, LPARAM);
BOOL				CenterDlgWindow(HWND);
HWND				CreateStatusBar(HWND, HINSTANCE);
BOOL				SetStatusBarParts(HWND, HWND);
LRESULT CALLBACK	EditDoubleControlProc(HWND, UINT, WPARAM, LPARAM, UINT_PTR, DWORD_PTR);
LRESULT CALLBACK	EditIntControlProc(HWND, UINT, WPARAM, LPARAM, UINT_PTR, DWORD_PTR);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDS_WINDOW_CLASS, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ACCELERATORS));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_METABUILDER));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MENU);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_METABUILDER));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   int w = 900, h = 600;
   int x = (GetSystemMetrics(SM_CXSCREEN) - w) / 2;
   int y = (GetSystemMetrics(SM_CYSCREEN) - h - 100) / 2;

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      x, y, w, h, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_CREATE:
		{
			INITCOMMONCONTROLSEX icex;
			icex.dwICC = ICC_BAR_CLASSES;
			InitCommonControlsEx(&icex);
			CreateStatusBar(hWnd, hInst);
		}
		break;
	case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
			case IDM_ACCOUNT:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_ACCOUNT), hWnd, AccountDlgProc);
				break;
			case IDM_SIZENSTOPS:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_SIZENSTOPS), hWnd, SizeAndStopsDlgProc);
				break;
			case IDM_DATAINT:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_DATAINT), hWnd, DataIntervalDlgProc);
				break;
			case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUT), hWnd, AboutDlgProc);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
	case WM_SIZE:
		// Auto-resize statusbar
		SendMessage(GetDlgItem(hWnd, IDC_STATUSBAR), WM_SIZE, 0, 0);
		SetStatusBarParts(hWnd, GetDlgItem(hWnd, IDC_STATUSBAR));
		break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for About box.
INT_PTR CALLBACK AboutDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
		CenterDlgWindow(hDlg);
        return TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return TRUE;
        }
        break;
    }
    return FALSE;
}

// Message handler for Account Settings box.
INT_PTR CALLBACK AccountDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		CenterDlgWindow(hDlg);
		SetWindowSubclass(GetDlgItem(hDlg, IDC_EDIT_DEPOSIT), EditDoubleControlProc, 0, 0);
		SetWindowSubclass(GetDlgItem(hDlg, IDC_EDIT_LEVERAGE), EditIntControlProc, 0, 0);
		SendMessage(hDlg, WM_NEXTDLGCTL, (WPARAM)GetDlgItem(hDlg, IDC_EDIT_DEPOSIT), TRUE);
		return FALSE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		break;
	}
	return FALSE;
}

// Message handler for Trading Size and Stops box.
INT_PTR CALLBACK SizeAndStopsDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		CenterDlgWindow(hDlg);
		return TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		break;
	}
	return FALSE;
}

// Message handler for Data Interval box.
INT_PTR CALLBACK DataIntervalDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		CenterDlgWindow(hDlg);
		return TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		break;
	}
	return FALSE;
}

// Center dialog box relative to its parent
BOOL CenterDlgWindow(HWND hDlg)
{
	HWND hParent = GetParent(hDlg);
	RECT rcDlg, rcParent;

	GetWindowRect(hDlg, &rcDlg);
	GetWindowRect(hParent, &rcParent);

	int nWidth = rcDlg.right - rcDlg.left;
	int nHeight = rcDlg.bottom - rcDlg.top;

	int nX = ((rcParent.right - rcParent.left) - nWidth) / 2 + rcParent.left;
	int nY = ((rcParent.bottom - rcParent.top) - nHeight) / 2 + rcParent.top;

	int nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	int nScreenHeight = GetSystemMetrics(SM_CYSCREEN);

	// make sure that the dialog box never moves outside of the screen
	if (nX < 0) nX = 0;
	if (nY < 0) nY = 0;
	if (nX + nWidth > nScreenWidth) nX = nScreenWidth - nWidth;
	if (nY + nHeight > nScreenHeight) nY = nScreenHeight - nHeight;

	MoveWindow(hDlg, nX, nY, nWidth, nHeight, FALSE);

	return TRUE;
}

HWND CreateStatusBar(HWND hParent, HINSTANCE hInst)
{
	HWND hStatus = CreateWindowEx(
		0,						// no extended styles
		STATUSCLASSNAME,		// name of status bar class
		(PCTSTR)NULL,			// no text when first created
		WS_CHILD | WS_VISIBLE,	// creates a visible child window
		0, 0, 0, 0,				// ignores size and position
		hParent,				// handle to parent window
		(HMENU)IDC_STATUSBAR,	// child window identifier
		hInst,					// handle to application instance
		NULL);					// no window creation data

	SetStatusBarParts(hParent, hStatus);

	return hStatus;
}

BOOL SetStatusBarParts(HWND hParent, HWND hStatus)
{
	// Get the coordinates of the parent window's client area.
	RECT rcClient;
	GetClientRect(hParent, &rcClient);
	// Set parts
	int i = 200;
	int a = (rcClient.right - i) / 2;
	int b = a + i;
	int paParts[] = { a, b, -1 };

	// Tell the status bar to create the window parts.
	SendMessage(hStatus, SB_SETPARTS, (WPARAM)(sizeof(paParts) / sizeof(int)), (LPARAM)paParts);

	return TRUE;
}

LRESULT CALLBACK EditDoubleControlProc(HWND hWnd, UINT uMsg, WPARAM wParam,
	LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	if (uMsg == WM_CHAR)
	{
		wchar_t wszDecimal[3];
		GetLocaleInfoEx(LOCALE_NAME_USER_DEFAULT, LOCALE_SDECIMAL,
			wszDecimal, sizeof(wszDecimal) / sizeof(wchar_t));
		wchar_t wszText[13];
		GetWindowText(hWnd, wszText, sizeof(wszText) / sizeof(wchar_t));
		wstring wstr(wszText);
		size_t f = wstr.find(wszDecimal);
		size_t len = wcslen(wszText);
		int pos;
		SendMessage(hWnd, EM_GETSEL, (WPARAM)&pos, 0);

		// Make sure we only allow specific characters
		if (!((wParam >= '0' && wParam <= '9')
			|| wParam == VK_BACK
			|| (wParam == wszDecimal[0] && f == string::npos)))
		{
			return FALSE;
		}

		// limit number of decimals and integers
		if (wParam != VK_BACK && 
			((wParam != wszDecimal[0] && f == string::npos && len == 9) ||
			(f != string::npos && ((pos <= f && f == 9) || (pos > f && len - f == 3)))))
		{
			return FALSE;
		}

	}

	return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK EditIntControlProc(HWND hWnd, UINT uMsg, WPARAM wParam,
	LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	if (uMsg == WM_CHAR)
	{
		// Make sure we only allow specific characters
		if (!((wParam >= '0' && wParam <= '9')
			|| wParam == VK_BACK))
		{
			return FALSE;
		}

		// limit number of characters
		if (wParam != VK_BACK && GetWindowTextLength(hWnd) == 4)
		{
			return FALSE;
		}
	}

	return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}
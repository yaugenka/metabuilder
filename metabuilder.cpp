// metabuilder.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "metabuilder.h"
#include <commctrl.h>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Account(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    SizeAndStops(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    DataInterval(HWND, UINT, WPARAM, LPARAM);
BOOL				CenterDlgWindows(HWND);
HWND				CreateStatusBar(HWND, HINSTANCE);
BOOL				SetStatusBarParts(HWND, HWND);

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
				DialogBox(hInst, MAKEINTRESOURCE(IDD_ACCOUNT), hWnd, Account);
				break;
			case IDM_SIZENSTOPS:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_SIZENSTOPS), hWnd, SizeAndStops);
				break;
			case IDM_DATAINT:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_DATAINT), hWnd, DataInterval);
				break;
			case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUT), hWnd, About);
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
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
		CenterDlgWindows(hDlg);
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

// Message handler for Account Settings box.
INT_PTR CALLBACK Account(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		CenterDlgWindows(hDlg);
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

// Message handler for Trading Size and Stops box.
INT_PTR CALLBACK SizeAndStops(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		CenterDlgWindows(hDlg);
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

// Message handler for Data Interval box.
INT_PTR CALLBACK DataInterval(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		CenterDlgWindows(hDlg);
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

// Center dialog box relative to its parent
BOOL CenterDlgWindows(HWND hDlg)
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

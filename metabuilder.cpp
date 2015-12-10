// metabuilder.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "metabuilder.h"

using namespace std;

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // main window class name

int deposit = 1000;
int leverage = 200;


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
LRESULT CALLBACK	DepositProc(HWND, UINT, WPARAM, LPARAM, UINT_PTR, DWORD_PTR);
LRESULT CALLBACK	LeverageProc(HWND, UINT, WPARAM, LPARAM, UINT_PTR, DWORD_PTR);
LRESULT CALLBACK	InLotsProc(HWND, UINT, WPARAM, LPARAM, UINT_PTR, DWORD_PTR);
LRESULT CALLBACK	InPerCentProc(HWND, UINT, WPARAM, LPARAM, UINT_PTR, DWORD_PTR);
LRESULT CALLBACK	SLTPProc(HWND, UINT, WPARAM, LPARAM, UINT_PTR, DWORD_PTR);
BOOL				EditControlCharHandler(HWND, WPARAM, int, int, bool);
BOOL				EditControlKeyDownHandler(HWND, WPARAM, int, int, bool);

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


//  Registers the window class.
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


//   Saves instance handle and creates main window
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

//  Processes messages for the main window.
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
	{
		CenterDlgWindow(hDlg);
		SetWindowSubclass(GetDlgItem(hDlg, IDC_EDIT_DEPOSIT), DepositProc, 0, 0);
		SetWindowSubclass(GetDlgItem(hDlg, IDC_EDIT_LEVERAGE), LeverageProc, 0, 0);
		SetWindowText(GetDlgItem(hDlg, IDC_EDIT_DEPOSIT), to_wstring(deposit).c_str());
		SetWindowText(GetDlgItem(hDlg, IDC_EDIT_LEVERAGE), to_wstring(leverage).c_str());
		//SendMessage(hDlg, WM_NEXTDLGCTL, (WPARAM)GetDlgItem(hDlg, IDC_EDIT_DEPOSIT), TRUE);
		return TRUE;
	}
	case WM_COMMAND:
	{
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		break;
	}
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
	{
		CenterDlgWindow(hDlg);
		SetWindowSubclass(GetDlgItem(hDlg, IDC_EDIT_LOTS), InLotsProc, 0, 0);
		SetWindowSubclass(GetDlgItem(hDlg, IDC_EDIT_PC), InPerCentProc, 0, 0);
		SetWindowSubclass(GetDlgItem(hDlg, IDC_EDIT_SL), SLTPProc, 0, 0);
		SetWindowSubclass(GetDlgItem(hDlg, IDC_EDIT_TP), SLTPProc, 0, 0);
		return TRUE;
	}
	case WM_COMMAND:
	{
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		break;
	}
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
	{
		CenterDlgWindow(hDlg);
		return TRUE;
	}
	case WM_COMMAND:
	{
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		break;
	}
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

// Deposit edit control proc
LRESULT CALLBACK DepositProc(HWND hWnd, UINT uMsg, WPARAM wParam,
	LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	switch (uMsg)
	{
	case WM_CHAR:
	{
		if (!EditControlCharHandler(hWnd, wParam, 9, 0, false))
			return FALSE;
		break;
	}
	case WM_CONTEXTMENU:
		return FALSE;
	}
	return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}

// Leverage edit control proc
LRESULT CALLBACK LeverageProc(HWND hWnd, UINT uMsg, WPARAM wParam,
	LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	switch (uMsg)
	{
	case WM_CHAR:
	{
		if (!EditControlCharHandler(hWnd, wParam, 4, 0, false))
			return FALSE;
		break;
	}
	case WM_CONTEXTMENU:
		return FALSE;
	}
	return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}

// In Lots edit control proc
LRESULT CALLBACK InLotsProc(HWND hWnd, UINT uMsg, WPARAM wParam,
	LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	switch (uMsg)
	{
	case WM_KEYDOWN:
	{
		if (wParam == VK_DELETE && !EditControlKeyDownHandler(hWnd, wParam, 3, 2, false))
			return FALSE;
		break;
	}
	case WM_CHAR:
	{
		
		if (!EditControlCharHandler(hWnd, wParam, 3, 2, false))
			return FALSE;
		break;
	}
	case WM_CONTEXTMENU:
		return FALSE;
	}
	return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}

// In Per Cent edit control proc
LRESULT CALLBACK InPerCentProc(HWND hWnd, UINT uMsg, WPARAM wParam,
	LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	switch (uMsg)
	{
	case WM_KEYDOWN:
	{
		if (wParam == VK_DELETE && !EditControlKeyDownHandler(hWnd, wParam, 3, 2, true))
			return FALSE;
		break;
	}
	case WM_CHAR:
	{

		if (!EditControlCharHandler(hWnd, wParam, 3, 2, true))
			return FALSE;
		break;
	}
	case WM_CONTEXTMENU:
		return FALSE;
	}
	return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}

// Stop Loss/Take Profit edit control proc
LRESULT CALLBACK SLTPProc(HWND hWnd, UINT uMsg, WPARAM wParam,
	LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	switch (uMsg)
	{
	case WM_CHAR:
	{
		if (!EditControlCharHandler(hWnd, wParam, 6, 0, false))
			return FALSE;
		break;
	}
	case WM_CONTEXTMENU:
		return FALSE;
	}
	return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}



// validates char input in edit controls
BOOL EditControlCharHandler(HWND hWnd, WPARAM wParam, int i, int d, bool b)
{
	wstring s;
	int n = GetWindowTextLength(hWnd);
	s.resize(n + 1);						//+1 for \0
	GetWindowText(hWnd, &s[0], n + 1);
	s.resize(n);
	int f = (int)s.find('.');
	int wp, lp;
	SendMessage(hWnd, EM_GETSEL, (WPARAM)&wp, (LPARAM)&lp);
	double k = -1;
	if (n > 0 && f != 0) k = stod(s, NULL);

	if (wParam == VK_BACK)
	{
		if (f == string::npos ? true : (wp == lp ? (wp != f + 1 ? true : n - 1 <= i) : 
			((f < wp || f >= lp) ? true : n - lp + wp <= i)))
			return TRUE;
		else
			return FALSE;
	}

	// enable selecting all by CTRL+A
	if (wParam == 1)
	{
		SendMessage(hWnd, EM_SETSEL, 0, (LPARAM)GetWindowTextLength(hWnd));
		return TRUE;
	}

	// prohibit inputing more than 100 for percent input fields
	if (b)
	{
		if (k == 100) return FALSE;
		if (k == 10 && !((wParam == '0' && wp != 0) || wParam == '.' || (f != string::npos && wp > f)))
			return FALSE;
		if ((f == string::npos ? n >= i : f >= i) && wp == lp - 1 && wp == 0 && 
			(k > 0 || (k == 0 && wParam != '1')))
			return FALSE;
		if (k > 10) --i;
	}
	// only allow specific characters
	if (!((wParam >= '0' && wParam <= '9')
		|| (wParam == '.' && d != 0 && n != 0 && f == string::npos)))
		return FALSE;

	// limit number of characters
	if (wp == lp && ((wParam != '.' && f == string::npos && n >= i) ||
		(f != string::npos && ((wp <= f && f >= i) || (wp > f && n - f >= d + 1)))))
		return FALSE;

	return TRUE;
}

// validates keydown messages in edit controls
BOOL EditControlKeyDownHandler(HWND hWnd, WPARAM wParam, int i, int d, bool b)
{
	wstring s;
	int n = GetWindowTextLength(hWnd);
	s.resize(n + 1);						//+1 for \0
	GetWindowText(hWnd, &s[0], n + 1);
	s.resize(n);
	int f = (int)s.find('.');
	int wp, lp;
	SendMessage(hWnd, EM_GETSEL, (WPARAM)&wp, (LPARAM)&lp);

	if (wParam == VK_DELETE)
	{
		if (f == string::npos ? true : (wp != f ? true : n - 1 <= i))
			return TRUE;
		else
			return FALSE;
	}

	return TRUE;
}
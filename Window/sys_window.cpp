//
//Includes
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

//Main Includes
#include "tglfw.h"
#include "sys_gl.h"
#include "rawinput.h"
#include <string>
#include "log.h"
#include "simpletimer.h"
#include "utf8conv.h"

//Library Includes
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")


// Function Declarations
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


//Local Variables.
WNDCLASS wc;
HWND hWnd;
HDC hDC;
HGLRC hRC;
MSG msg;
int 	SCREEN_W = 0;              // Screen Width
int 	SCREEN_H = 0;             // Screen Height
BOOL quit = FALSE;
BOOL Minimized = 0;
DWORD dwStyle;
BOOL focused = TRUE;
BOOL iconified = FALSE;

//StickyKeys
STICKYKEYS g_StartupStickyKeys = { sizeof(STICKYKEYS), 0 };
TOGGLEKEYS g_StartupToggleKeys = { sizeof(TOGGLEKEYS), 0 };
FILTERKEYS g_StartupFilterKeys = { sizeof(FILTERKEYS), 0 };

//My fake main
extern int main(int argc, char *argv[]);

//========================================================================
// Return value showing if window is in focus
//========================================================================
bool get_focus()
{
	return focused != 0; //turns warning off
}

//========================================================================
// Return value showing if window is iconified
//========================================================================
bool get_iconified()
{
	return iconified != 0;
}

//========================================================================
// Get physical mouse cursor position
//========================================================================
POINT get_mouse_pos()
{
	POINT p;
	if (GetCursorPos(&p))
	{
		//cursor position now in p.x and p.y
	}
	if (ScreenToClient(hWnd, &p))
	{
		//p.x and p.y are now relative to hwnd's client area
	}
	return p;
}


//========================================================================
// Set physical mouse cursor position
//========================================================================
void set_mouse_pos(int x, int y)
{
	POINT pos;

	// Convert client coordinates to screen coordinates
	pos.x = x;
	pos.y = y;
	ClientToScreen(hWnd, &pos);

	SetCursorPos(pos.x, pos.y);
}


//========================================================================
// Hide mouse cursor (lock it)
//========================================================================
void scare_mouse()
{
	// Get the window client area.
	RECT rc;
	GetClientRect(hWnd, &rc);

	// Convert the client area to screen coordinates.
	POINT pt = { rc.left, rc.top };
	POINT pt2 = { rc.right, rc.bottom };
	ClientToScreen(hWnd, &pt);
	ClientToScreen(hWnd, &pt2);
	SetRect(&rc, pt.x, pt.y, pt2.x, pt2.y);

	// Confine the cursor.
	ClipCursor(&rc);

	//Turn off the cursor pointer
	//ShowCursor(FALSE);
	while (ShowCursor(false) >= 0);

	// Capture cursor to user window
	SetCapture(hWnd);
}


//========================================================================
// Show mouse cursor (unlock it)
//========================================================================
void show_mouse()
{
	// Un-capture cursor
	ReleaseCapture();

	// Release the cursor from the window
	ClipCursor(NULL);

	ShowCursor(true);
}


//========================================================================
// Disable / Enable ShortCut Keys
//========================================================================
void AllowAccessibilityShortcutKeys(int bAllowKeys)
{

	if (bAllowKeys)
	{
		// Restore StickyKeys/etc to original state and enable Windows key      
		STICKYKEYS sk = g_StartupStickyKeys;
		TOGGLEKEYS tk = g_StartupToggleKeys;
		FILTERKEYS fk = g_StartupFilterKeys;

		SystemParametersInfo(SPI_SETSTICKYKEYS, sizeof(STICKYKEYS), &g_StartupStickyKeys, 0);
		SystemParametersInfo(SPI_SETTOGGLEKEYS, sizeof(TOGGLEKEYS), &g_StartupToggleKeys, 0);
		SystemParametersInfo(SPI_SETFILTERKEYS, sizeof(FILTERKEYS), &g_StartupFilterKeys, 0);
	}

	else
	{
		// Disable StickyKeys/etc shortcuts but if the accessibility feature is on, 
		// then leave the settings alone as its probably being usefully used

		FILTERKEYS fkOff = g_StartupFilterKeys;
		STICKYKEYS skOff = g_StartupStickyKeys;
		TOGGLEKEYS tkOff = g_StartupToggleKeys;

		if ((skOff.dwFlags & SKF_STICKYKEYSON) == 0)
		{
			// Disable the hotkey and the confirmation
			skOff.dwFlags &= ~SKF_HOTKEYACTIVE;
			skOff.dwFlags &= ~SKF_CONFIRMHOTKEY;

			SystemParametersInfo(SPI_SETSTICKYKEYS, sizeof(STICKYKEYS), &skOff, 0);
		}


		if ((tkOff.dwFlags & TKF_TOGGLEKEYSON) == 0)
		{
			// Disable the hotkey and the confirmation
			tkOff.dwFlags &= ~TKF_HOTKEYACTIVE;
			tkOff.dwFlags &= ~TKF_CONFIRMHOTKEY;

			SystemParametersInfo(SPI_SETTOGGLEKEYS, sizeof(TOGGLEKEYS), &tkOff, 0);
		}


		if ((fkOff.dwFlags & FKF_FILTERKEYSON) == 0)
		{
			// Disable the hotkey and the confirmation
			fkOff.dwFlags &= ~FKF_HOTKEYACTIVE;
			fkOff.dwFlags &= ~FKF_CONFIRMHOTKEY;

			SystemParametersInfo(SPI_SETFILTERKEYS, sizeof(FILTERKEYS), &fkOff, 0);
		}
	}

}


//========================================================================
// Check for running instance of this program
//========================================================================
BOOL IsFirstInstance()
{
	HANDLE hMutex = CreateMutex(NULL, FALSE, L"TGL_SYSTEM");
	if (hMutex && GetLastError() == ERROR_ALREADY_EXISTS)
	{
		ReleaseMutex(hMutex);
		return FALSE;
	}
	ReleaseMutex(hMutex);
	return TRUE;
}


//========================================================================
// Set the window title
//========================================================================
void set_window_title(const char *title)
{
	(void)SetWindowTextA(hWnd, title);
}


//========================================================================
// Return the Window Width
//========================================================================
int get_window_width()
{
	return SCREEN_W;
}


//========================================================================
// Return the Window Height
//========================================================================
int get_window_height()
{
	return SCREEN_H;
}


//========================================================================
// Return the Window Handle
//========================================================================
HWND win_get_window()
{
	return hWnd;
}


//========================================================================
// Popup a Windows Error Message, Allegro Style
//========================================================================
void allegro_message(const char *title, const char *message)
{
	MessageBoxA(NULL, message, title, MB_ICONEXCLAMATION | MB_OK | MB_TOPMOST);
}


//========================================================================
// Return a std string with last error message
// https://www.codeproject.com/Tips/479880/GetLastError-as-std-string
//========================================================================
std::string GetLastErrorStdStr()
{
	DWORD error = GetLastError();
	if (error)
	{
		LPVOID lpMsgBuf;
		DWORD bufLen = FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			error,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&lpMsgBuf,
			0, NULL);
		if (bufLen)
		{
			LPCSTR lpMsgStr = (LPCSTR)lpMsgBuf;
			std::string result(lpMsgStr, lpMsgStr + bufLen);

			LocalFree(lpMsgBuf);

			return result;
		}
	}
	return std::string();
}


//========================================================================
// Set the Icon file for the Window
//========================================================================
void set_icon(const char * icon)  
{
	HANDLE hIcon = LoadImage(0, win32::Utf8ToUtf16(icon).c_str(), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
	if (hIcon)
	{
		//Change both icons to the same icon handle.
		SendMessage(hWnd, WM_SETICON, ICON_SMALL, reinterpret_cast<LPARAM>(hIcon));
		SendMessage(hWnd, WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(hIcon));

		//This will ensure that the application icon gets changed too.
		SendMessage(GetWindow(hWnd, GW_OWNER), WM_SETICON, ICON_SMALL, reinterpret_cast<LPARAM>(hIcon));
		SendMessage(GetWindow(hWnd, GW_OWNER), WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(hIcon));
	}
	else
	{
		std::string thiserror = GetLastErrorStdStr();
		wrlog("Error loading requested icon file. Error: %s", thiserror.c_str());
	}
}


//========================================================================
// Main Window Creation Code
//========================================================================
int tglCreate_Window(int x, int y, int parm)
{

	// register window class
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = wc.hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = L"TGL_SYSTEM";
	RegisterClass(&wc);

	//Only allow one instance of the progam to be ran at a time.
	if (IsFirstInstance() == 0)
		exit(1);

	if ((parm & TGL_BORDERLESS) || (parm & TGL_FULLSCR))
		dwStyle = WS_POPUP | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	else
	{
		if (parm & TGL_RESIZEABLE)
		{
			dwStyle = WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
			//WS_CAPTION | WS_POPUPWINDOW | WS_VISIBLE |WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
		}
		else
		{
			dwStyle = WS_CAPTION | WS_POPUPWINDOW | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
		}
	}

	// Calculate the proper size for the window given a client 
	// Not going to use all of these, maybe eventually
	DWORD dwFrameWidth = GetSystemMetrics(SM_CXSIZEFRAME);
	DWORD dwFrameHeight = GetSystemMetrics(SM_CYSIZEFRAME);
	DWORD dwMenuHeight = GetSystemMetrics(SM_CYMENU);
	DWORD gdwWindowXPos = (GetSystemMetrics(SM_CXSCREEN) - x) / 2; 
	DWORD gdwWindowYPos = (GetSystemMetrics(SM_CYSCREEN) - y) / 2; 
	DWORD gdwWindowWidth = x + dwFrameWidth * 2;
	DWORD gdwWindowHeight = y + dwFrameHeight * 2 + dwMenuHeight;

	if (parm & TGL_FULLSCR)
	{ 
		SCREEN_W = GetSystemMetrics(SM_CXSCREEN);
		SCREEN_H = GetSystemMetrics(SM_CYSCREEN);
	}
	else
	{
		SCREEN_W = x;
		SCREEN_H = y;
	}

	RECT wr = { 0, 0, SCREEN_W, SCREEN_H };
	AdjustWindowRect(&wr, dwStyle, FALSE);

	//Adjust
	int xpos = 0;
	int ypos = 0;

	if ((parm & TGL_FULLSCR) == 0)
	{
		if (parm & TGL_CENTERX)
			xpos = gdwWindowXPos;
		if (parm & TGL_CENTERY)
			ypos = gdwWindowYPos;
	}

	// create main window
	hWnd = CreateWindow(L"TGL_SYSTEM", L"System Window", dwStyle, xpos, ypos, wr.right - wr.left, wr.bottom - wr.top, NULL, NULL, wc.hInstance, NULL);

	if (hWnd == NULL)
	{
		MessageBox(NULL, L"Window Creation Failed!", L"Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	
	if (EnableOpenGL(hWnd, &hDC, &hRC) == 0)
	{
		wrlog("OPENGL Initalization Error, could not create context");
		exit(1);
	}


	// Save the current sticky/toggle/filter key settings so they can be restored them later
	SystemParametersInfo(SPI_GETSTICKYKEYS, sizeof(STICKYKEYS), &g_StartupStickyKeys, 0);
	SystemParametersInfo(SPI_GETTOGGLEKEYS, sizeof(TOGGLEKEYS), &g_StartupToggleKeys, 0);
	SystemParametersInfo(SPI_GETFILTERKEYS, sizeof(FILTERKEYS), &g_StartupFilterKeys, 0);

	// Disable when full screen
	//AllowAccessibilityShortcutKeys( true );

	// Restore back when going to windowed or shutting down
	AllowAccessibilityShortcutKeys(false);


	HRESULT i = RawInput_Initialize(hWnd);

	ShowWindow(hWnd, SW_SHOW);

	::SetWindowPos(win_get_window(),       // handle to window
		HWND_TOPMOST,  // placement-order handle
		0,     // horizontal position
		0,      // vertical position
		0,  // width
		0, // height
		SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE// window-positioning options
	);

	return 1;
}


//========================================================================
//  Fast Message Handler Process
//========================================================================
void msg_proc()
{
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{

		// handle or dispatch messages
		if (msg.message == WM_QUIT)
		{
			//exit_proc();
			exit(1); //Go to exit handler...
		}
		else
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

	}
}


//========================================================================
// Swap OpenGl Buffers and call the message Handler.
//========================================================================
void tglSwapBuffers()
{
	msg_proc();
	SwapBuffers(hDC);
}


//========================================================================
// WinMain Function
//========================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow)
{

	LogOpen("syslog.log");

	int(*mainfunc) (int argc, char *argv[]) = (int(*)(int, char *[]))  main;

	mainfunc(__argc, __argv);

	wrlog("Exit Log Called");
	LogClose();
	// shutdown OpenGL
	AllowAccessibilityShortcutKeys(true);
	DisableOpenGL(hWnd, hDC, hRC);

	// destroy the window explicitly
	DestroyWindow(hWnd);

	return msg.wParam;
}


//========================================================================
// Windows Process Handler
//========================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{

	case WM_CREATE:
		return 0;

	case WM_SIZE:                           // Resize The OpenGL Window
	{

		gl_resize(LOWORD(lParam), HIWORD(lParam));       // LoWord=Width, HiWord=Height
		return 0;                       // Jump Back
	}

	case WM_ACTIVATE:
	{
		// Window was (de)focused and/or (de)iconified

		focused = LOWORD(wParam) != WA_INACTIVE;
		iconified = HIWORD(wParam) ? TRUE : FALSE;

		if (focused && iconified)
		{
			// This is a workaround for window iconification using the
			// taskbar leading to windows being told they're focused and
			// iconified and then never told they're defocused
			focused = FALSE;
		}

		return 0;
	}

	case WM_CLOSE:
	{
		quit = 1;
		PostQuitMessage(0);
		return 0;
	}

	case WM_DESTROY:
		return 0;

	case WM_INPUT: {return RawInput_ProcessInput(hWnd, wParam, lParam); return 0; }

	case WM_SYSCOMMAND:
	{
		switch (wParam & 0xfff0)
		{
		case SC_SCREENSAVE:
		case SC_MONITORPOWER:
		{
			return 0;
		}

		case SC_CLOSE:
		{
			//I can add a close hook here to trap close button
			quit = 1;
			PostQuitMessage(0);
			break;
		}
		// User trying to access application menu using ALT?
		case SC_KEYMENU:
			return 0;
		}
		DefWindowProc(hWnd, message, wParam, lParam);
	}

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;

	}
	return 0;
}


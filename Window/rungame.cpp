#include "log.h"
#include "rungame.h"
#include <string>
#include "ini.h"
#include "romscan.h"
#include "globals.h"
#include "config.h"
#include "tglfw.h"
#include <Windows.h>
#include <chrono>
#include <thread>
#include "utf8conv.h"
#include "audio.h"

std::string removeExtension(const std::string& filename) {
	std::string extension = ".exe";
	if (filename.size() >= extension.size() &&
		filename.compare(filename.size() - extension.size(), extension.size(), extension) == 0) {
		return filename.substr(0, filename.size() - extension.size());
	}
	return filename; // Return the original string if ".exe" is not found at the end
}

void save_settings()
{
	wrlog("Writing Config Changes", fesetup.volume);
	set_config_int("main", "volume", fesetup.volume);
}

void DisplayError(LPCTSTR errorDesc, DWORD errorCode)
{
	TCHAR errorMessage[1024] = TEXT("");

	DWORD flags = FORMAT_MESSAGE_FROM_SYSTEM
		| FORMAT_MESSAGE_IGNORE_INSERTS
		| FORMAT_MESSAGE_MAX_WIDTH_MASK;

	FormatMessage(flags,
		NULL,
		errorCode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		errorMessage,
		sizeof(errorMessage) / sizeof(TCHAR),
		NULL);
	/*
	#ifdef _UNICODE
		wcerr << L"Error : "   << errorDesc    << endl;
		wcerr << L"Code    = " << errorCode    << endl;
		wcerr << L"Message = " << errorMessage << endl;
	#else
	  */
	  //wrlog("Error: %s", errorDesc);
	  //wrlog("Error Code: %d", errorCode);
	wrlog("Error Message: %s", errorMessage);
	//cerr << "Error : "   << errorDesc    << endl;
	//cerr << "Code    = " << errorCode    << endl;
	//cerr << "Message = " << errorMessage << endl;
//#endif
}

void run_game(void)
{
	int b = 0;
	int stopped = 0;
	int errorlvl = 0;
	const char* defops;
	const char* extopts;
	//const char* tempselect;
	//const char* rootpath;

	std::string path;
	std::string sep = "\\";

	char* rot_list[] = { "","-ror","-rol","-flipx -flipy","NULL" };

	// I don't want the .exe at the end of the name to search for in the ini file, so I do this. 
    std::string exename = removeExtension(gamelist[curr_game].exename);

	defops = get_config_string("main", "defops", NULL);
	extopts = get_config_string("extopts", (char*)gamelist[curr_game].gsname.c_str(), "NONE");
	
	mamepath = get_config_string(exename.c_str(), "exepath", "NONE");
	wrlog("Rootpath found for this executable, %s is %s", exename.c_str(), mamepath);

	if (!strcmp(mamepath, "NONE") == 0)
	{
		path.assign(mamepath);
	}
	else
	{
		path.assign("c:\\mame");
		wrlog("Executable path passed to run_game not valid, ie %s", mamepath);
	}

	if (!(path[path.size() - 1] == '\\'))
	{
		path.append("\\");
	}

	path.append(gamelist[curr_game].exename.c_str()); //Add the exe name.

	path.append(" "); //Add a space

	path.append(gamelist[curr_game].gsname.c_str()); //add the game name

	if (rotation & fesetup.autorotate)
	{
		path.append(" "); //Add a space
		path.append(rot_list[rotation]);
	}

	path.append("\0");
	wrlog("New mame path %s", path.c_str());

	/*
	 if (rotation & fesetup.autorotate)
			  {
				strcat(args[0]," ");
				strcat(args[0],rot_list[rotation]);
			  }

	if (strcmp(extopts,"NONE")!=0)
		{
			strcat(args[0]," ");
			strcat(args[0],extopts);
			wrlog ("Extra options Used %s",extopts);
		}
	  if (defops){
			strcat(args[0]," ");
			strcat(args[0],defops);
				   }

	  strcat(args[0],"\0");
	 */
	 //save_settings();

	 //clear_bitmap(tempbg);

	 //screen_draw_loading(rotation);

	audio_play(A_RUNGAME, 64, 0);

	std::this_thread::sleep_for(std::chrono::milliseconds(500));

	//wrlog("ARGS 0:%s",args[0]);
	run_process((char*)path.c_str(), mamepath);
}

void OpenCmdWindow()
{
	STARTUPINFO          si = { sizeof(si) };
	PROCESS_INFORMATION  pi;
	char               szExe[] = "cmd.exe";
	si.wShowWindow = SW_SHOWMAXIMIZED;

	if (CreateProcess(0, (LPWSTR)win32::Utf8ToUtf16(szExe).c_str(), 0, 0, FALSE, 0, 0, 0, &si, &pi))
	{
		// Wait for process to finish
		WaitForSingleObject(pi.hProcess, INFINITE);

		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
}

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
	DWORD dwPID;

	GetWindowThreadProcessId(hwnd, &dwPID);

	//wrlog("LPARAM %d dwpid %d", lParam, dwPID);

	int a = dwPID;
	int b = lParam;

	//if( dwPID == lParam ) {
	if (a == b) {
		SetWindowPos(hwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		wrlog("Found !!!!!!!!!!!!!! Doing Something!!!");
		// Or just SetFocus( hwnd );
		//::SetForegroundWindow(hwnd);

		return false;
	}

	return true;
}

void run_process(char* fullpath, const char* dirpath)
{
	//int have_error=0;
	HWND hWnd;
	int dwExitCode = -1;
	DWORD exitstatus = -5;
	DWORD exitcode = -5;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOWNORMAL;//SW_HIDE; //SW_SHOWNORMAL

	hWnd = win_get_window(); //Get Current HWND Before leaving this prog.

	if (!CreateProcess(NULL,

		(LPWSTR)win32::Utf8ToUtf16(fullpath).c_str(),
		NULL,             // Process handle not inheritable.
		NULL,             // Thread handle not inheritable.
		FALSE,            // Set handle inheritance to FALSE.
		NORMAL_PRIORITY_CLASS | CREATE_NO_WINDOW | DETACHED_PROCESS,   // + DETACHED_PROCESS,//CREATE_NO_WINDOW ,  CREATE_NEW_CONSOLE,    CREATE_NEW_PROCESS_GROUP + // No creation flags.
		0,             // Use parent's environment block.
		(LPWSTR)win32::Utf8ToUtf16(dirpath).c_str(),
		&si,              // Pointer to STARTUPINFO structure.
		&pi)             // Pointer to PROCESS_INFORMATION structure.
		)
	{
		LPVOID lpMsgBuf;
		DWORD dw = GetLastError();
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, dw, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);

		// Display the error
		std::wstring strError = (LPTSTR)lpMsgBuf;
		wrlog("CreateProcess failed. Error #(%d).\n", strError.c_str());
		//sample_start(0,4, 128, 0);
		audio_play(4, 128, 0);
		have_error = 1;
	}
	else
	{
		wrlog("Starting Program");

		SetWindowPos(win_get_window(),       // handle to window
			HWND_BOTTOM,  // placement-order handle
			0,     // horizontal position
			0,      // vertical position
			0,  // width
			0, // height
			//SW_HIDE
			SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE// window-positioning options
		);

		bool ret = !!EnumWindows(EnumWindowsProc, (LPARAM)(pi.dwProcessId));
		wrlog("Process ID %d", pi.dwProcessId);
		if (ret == true) wrlog("return is true");
		else wrlog("return is false");

		while (WAIT_OBJECT_0 != MsgWaitForMultipleObjects(1, &pi.hProcess, FALSE, INFINITE, QS_ALLINPUT))
		{
			while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				DispatchMessage(&msg);
			}
		}
	}

	wrlog("Run Game exit code %d", dwExitCode);
	if (dwExitCode == 5) { wrlog(" Game Doesn't exist: %d", dwExitCode); have_error = 3; } //exit code 5 rom not found
	if (dwExitCode == 2) { wrlog(" Rom Bad or Missing: %d", dwExitCode); have_error = 2; } //Missing piece of rom

	// Close process and thread handles.
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	Sleep(10);

	ShowWindow(hWnd, SW_RESTORE); //Restore window?
	SetFocus(hWnd);
}

struct EnumWindowsCallbackArgs {
	EnumWindowsCallbackArgs(DWORD p) : pid(p) {}
	const DWORD pid;
	std::vector<HWND> handles;
};

static BOOL CALLBACK EnumWindowsCallback(HWND hnd, LPARAM lParam)
{
	EnumWindowsCallbackArgs* args = (EnumWindowsCallbackArgs*)lParam;

	DWORD windowPID;
	(void)::GetWindowThreadProcessId(hnd, &windowPID);
	if (windowPID == args->pid) {
		args->handles.push_back(hnd);
	}

	return TRUE;
}

std::vector<HWND> getToplevelWindows()
{
	EnumWindowsCallbackArgs args(::GetCurrentProcessId());
	if (::EnumWindows(&EnumWindowsCallback, (LPARAM)&args) == FALSE) {
		// XXX Log error here
		return std::vector<HWND>();
	}
	return args.handles;
}
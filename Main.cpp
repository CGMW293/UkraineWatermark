#include <Windows.h> 
#include <gdiplus.h>
#pragma comment(lib,"gdiplus.lib")
#include <iostream>
void drawText(HDC hdc, int x, int y, const char* text) //HDC is a handle
{
	// Create a device context.

	// Create a font object.
	LPCSTR god = "help";
	LPCWSTR dammit = L"MADE IN UKRAINE";
	HFONT font = CreateFontA(100, 0, 0, 0, 0, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH, god);

	// Select the font into the device context.
	SelectObject(hdc, font);

	SetTextColor(hdc, RGB(200, 200, 200));	// Set the text color.

	SetBkMode(hdc, TRANSPARENT); //Make the text transparent
	TextOut(hdc, 0, 0, dammit, strlen("MADE IN UKRAINE")); 	// Draw the text.

	// Delete the font object.
	DeleteObject(font);

	// Release the device context.
	ReleaseDC(NULL, hdc);
}
WNDCLASSEX hai;
LPCWSTR ClassName;
LRESULT CALLBACK Wndproc(HWND Handle, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;

	PAINTSTRUCT ps;
	switch (msg) {
	case WM_CREATE:
		// Start a timer to wait for 15 seconds
		SetTimer(Handle, 1, 15000, NULL);
		break;
	case WM_TIMER: // Timer with ID 1 has elapsed (15 seconds passed)
		if (wParam == 1)
		{
			COLORREF redColor = RGB(255, 0, 0);
			KillTimer(Handle, 1); // Stop the timer
			PostMessage(Handle, WM_CLOSE, 0, 0); // Close the window
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(Handle, &ps);
		drawText(hdc, 10, 10, "Made in Ukraine");
		InvalidateRect(Handle, NULL, TRUE); // Invalidates the entire window (idk what that means)
		UpdateWindow(Handle); // Updates the window
		EndPaint(Handle, &ps);
		break;
	case WM_CLOSE:
		DestroyWindow(Handle);
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(Handle, msg, wParam, lParam);
}
// Handle             Always NULL            CMD arguments    How the window looks on creation.
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	TCHAR szPath[MAX_PATH];
	GetModuleFileName(NULL, szPath, MAX_PATH);

	HKEY hKey;
	if (RegOpenKeyEx(HKEY_CURRENT_USER, TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Run"), 0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS)
	{
		RegSetValueEx(hKey, TEXT("MyProgram"), 0, REG_SZ, (LPBYTE)szPath, sizeof(TCHAR) * (lstrlen(szPath) + 1));

		RegCloseKey(hKey);
	}
	else
	{
		std::cout << "Failed to open registry key!" << std::endl;
	}
	//add gdiplus
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);

	const LPCWSTR ClassName = L"Haii!!!!";
	WNDCLASSEX hai = { 0 }; //Zeros out the hai structure, contains information for a window class
	//Defines all hai parameters for the window structure https://learn.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-wndclassexa
	hai.cbSize = sizeof(hai);
	hai.style = CS_HREDRAW | CS_VREDRAW;
	hai.lpfnWndProc = Wndproc;
	hai.cbClsExtra = 0;
	hai.cbWndExtra = 0;
	hai.hInstance = hInstance;
	hai.hCursor = nullptr;
	hai.hIcon = nullptr;
	hai.hbrBackground = nullptr;
	hai.lpszClassName = ClassName;
	hai.hIconSm = nullptr;
	RegisterClassEx(&hai);
	int screenWidth = GetSystemMetrics(SM_CXSCREEN); int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	HWND Handle = CreateWindowEx(WS_EX_TOOLWINDOW, ClassName, L"made in ukraine", WS_POPUP, screenWidth-785, screenHeight-125, 800, 80, nullptr, nullptr, hInstance, nullptr);
	SetWindowLong(Handle, GWL_EXSTYLE, GetWindowLong(Handle, GWL_EXSTYLE) | WS_EX_LAYERED);
	SetLayeredWindowAttributes(Handle, RGB(0, 0, 0), 0, LWA_COLORKEY); //Makes the window transparent
	BOOL did_it_run = ShowWindow(Handle, LWA_COLORKEY);//shows the window
	SetWindowPos(Handle, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);//Makes sure the window is always the topmost
	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	Gdiplus::GdiplusShutdown(gdiplusToken);
	return 0;
}

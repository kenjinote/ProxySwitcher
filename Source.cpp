#pragma comment(lib, "Wininet")

#include <windows.h>
#include <wininet.h>

#define ID_CHECK (100)
TCHAR szClassName[] = TEXT("Window");

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:
		CreateWindow(TEXT("BUTTON"), TEXT("プロキシサーバーを使用する"), WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 10, 10, 256, 32, hWnd, (HMENU)ID_CHECK, ((LPCREATESTRUCT)lParam)->hInstance, 0);
		break;
	case WM_COMMAND:
		if (LOWORD(wParam) == ID_CHECK)
		{
			const BOOL bProxyEnable = (BOOL)SendDlgItemMessage(hWnd, ID_CHECK, BM_GETCHECK, 0, 0);
			INTERNET_PER_CONN_OPTION Option = { 0 };
			Option.dwOption = INTERNET_PER_CONN_FLAGS;
			Option.Value.dwValue = bProxyEnable ? PROXY_TYPE_PROXY : PROXY_TYPE_DIRECT;
			INTERNET_PER_CONN_OPTION_LIST OptionList = { 0 };
			OptionList.dwSize = sizeof(INTERNET_PER_CONN_OPTION_LIST);
			OptionList.dwOptionCount = 1;
			OptionList.pOptions = &Option;
			if (InternetSetOption(0, INTERNET_OPTION_PER_CONNECTION_OPTION, &OptionList, sizeof(INTERNET_PER_CONN_OPTION_LIST)))
			{
				InternetSetOption(0, INTERNET_OPTION_REFRESH, NULL, NULL);
			}
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInst, LPSTR pCmdLine, int nCmdShow)
{
	MSG msg;
	WNDCLASS wndclass = {
		CS_HREDRAW | CS_VREDRAW,
		WndProc,
		0,
		0,
		hInstance,
		0,
		LoadCursor(0,IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		0,
		szClassName
	};
	RegisterClass(&wndclass);
	HWND hWnd = CreateWindow(
		szClassName,
		TEXT("Window"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		0,
		CW_USEDEFAULT,
		0,
		0,
		0,
		hInstance,
		0
		);
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);
	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

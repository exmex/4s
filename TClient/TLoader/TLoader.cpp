#include "stdafx.h"
#include "TLoader.h"


LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_DESTROY	: PostQuitMessage(0); break;
	default			: return DefWindowProc( hWnd, message, wParam, lParam);
	}

	return 0;
}

int __stdcall WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX vCLASS;
	HWND hWND = NULL;
	MSG msg;

	vCLASS.cbSize = sizeof(WNDCLASSEX);
	vCLASS.cbWndExtra = 0;
	vCLASS.cbClsExtra = 0;

	vCLASS.hIconSm = LoadIcon( hInstance, (LPCSTR) IDI_TLOADER);
	vCLASS.hCursor = LoadCursor( hInstance, IDC_ARROW);
	vCLASS.hIcon = vCLASS.hIconSm;

	vCLASS.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
	vCLASS.style = CS_HREDRAW|CS_VREDRAW;

	vCLASS.lpfnWndProc = WndProc;
	vCLASS.hInstance = hInstance;

	vCLASS.lpszClassName = "TLoaderClass";
	vCLASS.lpszMenuName = NULL;

	if(!hWND)
		return 0;

	RegisterClassEx(&vCLASS);
	hWND = CreateWindow(
		"TLoaderClass",
		"TLoader",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0,
		CW_USEDEFAULT, 0,
		NULL,
		NULL,
		hInstance,
		NULL);

	ShowWindow( hWND, nCmdShow);
	UpdateWindow(hWND);

	while(GetMessage( &msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

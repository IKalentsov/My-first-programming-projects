#include <windows.h>

const wchar_t windowClass[] = L"win32app";
const wchar_t windowTitle[] = L"Win32API - ������ 1";


long __stdcall WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

	HDC hdc;

	switch (message) 
	{
		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		case WM_LBUTTONDOWN:
		{
			WORD x, y, nSize;
			TCHAR szBuf[80];
			x = LOWORD(lParam);
			y = HIWORD(lParam);

			hdc = GetDC(hWnd);

			nSize = wsprintf(szBuf, TEXT("(%d, %d)"), x, y);
			MessageBox(hWnd, szBuf, L"hello", MB_OK);

			ReleaseDC(hWnd, hdc);
			break;
		}
		case WM_RBUTTONUP:
			MessageBox(hWnd, L"Clicked_R", L"hello_r", MB_OK | MB_ICONEXCLAMATION);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		return 0;
	}
}

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

  WNDCLASSEX wcex;

  wcex.cbSize = sizeof(WNDCLASSEX);
  wcex.style = CS_HREDRAW | CS_VREDRAW;
  wcex.lpfnWndProc = WndProc;
  wcex.cbClsExtra = 0;
  wcex.cbWndExtra = 0;
  wcex.hInstance = hInstance;
  wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
  wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
  wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
  wcex.lpszMenuName = NULL;
  wcex.lpszClassName = windowClass;
  wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

  if (!RegisterClassEx(&wcex)) {
    MessageBox(NULL, L"Can�t register window class!", L"Win32 API Test", NULL);
    return 1;
  }

  HWND hWnd = CreateWindow(windowClass, windowTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 500, 400, NULL, NULL, hInstance, NULL);

  if (!hWnd) {
    MessageBox(NULL, L"Can�t create window!", L"Win32 API Test", NULL);
    return 1;
  }

  ShowWindow(hWnd, nCmdShow);
  UpdateWindow(hWnd);

  MSG msg;

  while (GetMessage(&msg, NULL, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  return msg.wParam;
}

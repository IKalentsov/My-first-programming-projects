#include <windows.h>

const wchar_t windowClass[] = L"win32app";
const wchar_t windowTitle[] = L"Win32API - ������ 2";

long __stdcall WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

  PAINTSTRUCT ps;
  HDC hdc;

  HPEN pen, old_pen;
  HBRUSH brush, old_brush;

  RECT r;
  GetClientRect(hWnd, &r);

  switch (message) 
  {
      case WM_DESTROY:
          PostQuitMessage(0);
          break;

      case WM_RBUTTONDOWN: {

          wchar_t str[] = L"Hello!";

          HWND hW = FindWindow(NULL, L"1234.txt");
          if (hW != NULL)
              SendMessage(hW, WM_SETTEXT, NULL, (LPARAM)str); //original

          HMENU popup = CreatePopupMenu();

          AppendMenu(popup, MF_STRING, 4131, L"Exit");

          POINT point = { LOWORD(lParam), HIWORD(lParam) };

          ClientToScreen(hWnd, &point);

          TrackPopupMenu(popup, TPM_LEFTBUTTON, point.x, point.y, 0, hWnd, NULL);

          DestroyMenu(popup);
          break;
      }
      case WM_PAINT: {
          hdc = BeginPaint(hWnd, &ps);
          const wchar_t mess[] = L"Hello";
          TextOut(hdc, 0, 0, mess, 5);
          SetPixel(hdc, 150, 150, RGB(0, 0, 0));

          pen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
          old_pen = (HPEN)SelectObject(hdc, pen);

          brush = CreateSolidBrush(RGB(0, 50, 255));
          old_brush = (HBRUSH)SelectObject(hdc, brush);

          MoveToEx(hdc, 50, 50, NULL);

          //resistior
          LineTo(hdc, 100, 50);
          LineTo(hdc, 100, 70);
          LineTo(hdc, 50, 70);
          LineTo(hdc, 50, 50);

          MoveToEx(hdc, 50, 60, NULL);
          LineTo(hdc, 30, 60);
          MoveToEx(hdc, 100, 60, NULL);
          LineTo(hdc, 120, 60);

          EndPaint(hWnd, &ps);

          SelectObject(hdc, old_pen);
          SelectObject(hdc, old_brush);
          DeleteObject(pen);
          DeleteObject(brush);
          break;
      }
      case WM_COMMAND: {
        if (LOWORD(wParam) == 4131)
        //SendMessage(hWnd, WM_SYSCOMMAND, SC_MAXIMIZE, 0); //��������� ����
        SendMessage(hWnd, WM_SYSCOMMAND, SC_CLOSE, 0);  // ��������� ����
        break;
      }

    default: return DefWindowProc(hWnd, message, wParam, lParam);
  }
  return 0;
}

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
  WNDCLASSEX wcex; //������� ��������� ��� ��������� � ������ ������ WNDCLASSEX

  wcex.cbSize = sizeof(WNDCLASSEX); //������ ��������� � ������
  wcex.style = CS_HREDRAW | CS_VREDRAW; // ����� ������ ������
  wcex.lpfnWndProc = WndProc; // ��������� �� ���������������� �������
  wcex.cbClsExtra = 0; // ����� ������������� ������ � ����� ���������
  wcex.cbWndExtra = 0; // ����� ������������� ������ ��� �������� ���������� ����������
  wcex.hInstance = hInstance; // ��������� �� ������, ���������� ��� ����, ������������ ������
  wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION)); // ���������� ���������
  wcex.hCursor = LoadCursor(NULL, IDC_ARROW); // ���������� �������
  wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); // ���������� ����� ��� �������� ���� ����
  wcex.lpszMenuName = NULL; // ��������� �� ��� ����
  wcex.lpszClassName = windowClass; //��������� �� ��� ������
  wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION)); //���������� ��������� ����������� � ����

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

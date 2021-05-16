#include <windows.h>
#include "resource.h"
#include <string>
#include <fstream>
 
const wchar_t windowClass[] = L"win32app";
const wchar_t windowTitle[] = L"Win32API - Пример 3";

enum {
	resistor = +1,
	npnBJT,
	CurrentSources,
};


int flagselement;
short Buf[5000];
int x, y;
BOOL bDataOK = FALSE; //Если TRUE, данные прочитаны в буфер Buf
long __stdcall WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;

	HPEN pen, old_pen;
	HBRUSH brush, old_brush;
	
	wchar_t str[64];

	PAINTSTRUCT ps;
	RECT r;
	GetClientRect(hWnd, &r); //узнать текущее положение 

	switch (message)
	{

		case WM_MOUSEMOVE:
		{

			int xPos = LOWORD(lParam);
			int yPos = HIWORD(lParam);
			wsprintf(str, TEXT("(%d, %d)"), xPos, yPos);
			SetWindowText(hWnd, str);
			break;
		}

		case WM_PAINT: 
		{
			hdc = BeginPaint(hWnd, &ps); //начало обработки Paint

			if (flagselement == resistor) {
				POINT pt[10] = { 0, 0, 50, 0, 50, -25, 150, -25, 150, 0, 200, 0, 150, 0, 150, 25, 50, 25, 50, 0 };

				MoveToEx(hdc, x + pt[0].x, y + pt[0].y, NULL);
				for (int i = 1; (i < 10); i++)
					LineTo(hdc, x + pt[i].x, y + pt[i].y); //для остальных точек
			}

			if (flagselement == npnBJT) {
				Ellipse(hdc, x, y, x + 100, y + 100);
				POINT pt[17] = { 50, -15, 50, 20, 30, 30, 30, 15, 30, 45, -30, 45, 30, 45, 30, 80, 30, 70, 40, 75, 45, 70, 50, 80, 35, 80, 45, 70, 50, 80, 55, 83, 55, 125 };
				MoveToEx(hdc, x + pt[0].x, y + pt[0].y, NULL);
				for (int i = 1; i < 17; i++)
					LineTo(hdc, x + pt[i].x, y + pt[i].y);
			}

			if (flagselement == CurrentSources) {
				Ellipse(hdc, x, y, x + 100, y + 100);
				MoveToEx(hdc, x + 50, y - 30, NULL);
				LineTo(hdc, x + 50, y);
				MoveToEx(hdc, x + 50, y + 100, NULL);
				LineTo(hdc, x + 50, y + 130);
				MoveToEx(hdc, x + 50, y + 80, NULL);
				LineTo(hdc, x + 50, y + 10);
				MoveToEx(hdc, x + 40, y + 30, NULL);
				LineTo(hdc, x + 50, y + 10);
				MoveToEx(hdc, x + 60, y + 30, NULL);
				LineTo(hdc, x + 50, y + 10);
			}

			EndPaint(hWnd, &ps);
			break;
		}

		case WM_LBUTTONDOWN:
		{
			x = LOWORD(lParam);
			y = HIWORD(lParam);
			InvalidateRect(hWnd, NULL, true);
			UpdateWindow(hWnd);
			break;
		}

		case WM_COMMAND:
		{
			switch (LOWORD(wParam))
			{
			case 9002:
				if (MessageBox(hWnd, L"Вы действительно хотите выйти?", L"Предупреждение", MB_YESNO) == IDYES)
					SendMessage(hWnd, WM_SYSCOMMAND, SC_CLOSE, 0);
				break;

			case 9001:   //open
			{
				OPENFILENAME ofn;
				wchar_t szFileName[MAX_PATH] = L"";
				ZeroMemory(&ofn, sizeof(ofn));
				ofn.lStructSize = sizeof(ofn); //длина структуры в байтах
				ofn.hwndOwner = hWnd; //дескриптор окна, которое владеет блоком диалога
				ofn.lpstrFile = szFileName; //буфер для имени файла
				ofn.lpstrFilter = L"Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
				ofn.nMaxFile = MAX_PATH; //размер буфера жля имени файла
				ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY; //создание диалогового окна, ввод с клавы имён только существ файлов, скрывается окно флажка только для чтения
				ofn.lpstrDefExt = L"txt";
				if (GetOpenFileName(&ofn)) { //Если имя файла выбрано
					HANDLE FileHandle;
					DWORD nCnt;

					FileHandle = CreateFile(szFileName,
						GENERIC_READ, 0, NULL, OPEN_EXISTING,
						FILE_ATTRIBUTE_NORMAL, NULL);
					if (FileHandle == INVALID_HANDLE_VALUE)
					{
						MessageBox(hWnd, L"Error", L"Error", MB_OK);
						return 0;
					}

					ReadFile(FileHandle, Buf, 2*500, &nCnt, NULL); //чтение из файла

					x = (Buf[0]);
					y = (Buf[1]);
					flagselement = Buf[2];

					CloseHandle(FileHandle);
					bDataOK = TRUE; //Данные имеются, можно выводить
					InvalidateRect(hWnd, NULL, TRUE);
					break;
				}
				break;
			}

			case 9003: //save as
			{
				OPENFILENAME ofn;
				wchar_t szFileName[MAX_PATH] = L"";
				ZeroMemory(&ofn, sizeof(ofn));

				ofn.lStructSize = sizeof(ofn); //длина структуры в байтах
				ofn.hwndOwner = hWnd; //дескриптор окна, которое владеет блоком диалога
				ofn.lpstrFile = szFileName;
				ofn.lpstrFilter = L"Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
				ofn.nMaxFile = MAX_PATH; //размер буфера
				ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY; //создание диалогового окна, ввод с клавы имён только существ файлов, скрывается окно флажка только для чтения
				ofn.lpstrDefExt = L"txt";

				if (GetSaveFileName(&ofn))
				{
					HANDLE FileHandle;
					DWORD nCnt;

					Buf[0] = x;
					Buf[1] = y;
					Buf[2] = flagselement;

					FileHandle = CreateFile(szFileName,
						GENERIC_WRITE, 0, NULL, OPEN_ALWAYS,
						FILE_ATTRIBUTE_NORMAL, NULL);

					if (FileHandle == INVALID_HANDLE_VALUE)
					{
						MessageBox(hWnd, L"Error", L"Error", MB_OK);
						return 0;
					}
				
					WriteFile(FileHandle, Buf, 2*500, &nCnt, NULL);
					CloseHandle(FileHandle);
					return 0;
				}
				break;
			}

			case 9004: //resistor
			{
				flagselement = resistor;
				break;
			}

			case 9005:
			{
				flagselement = CurrentSources;
				break;
			}

			case 9006: 
			{
				flagselement = npnBJT;
				break;
			}
			}
			break;

		}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam); //<-обрабатывает все сообщения
	}														//которые не обрабатывает оконная процедура
	return 0;
}


//точка входа в программу
int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW; //при изменении размера окна вся раб обл становится недействительной
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); //фон рабочей области
	wcex.lpszMenuName = MAKEINTRESOURCE(IDR_MYMENU);
	wcex.lpszClassName = windowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	if (!RegisterClassEx(&wcex)) {
		MessageBox(NULL, L"CanТt register window class!", L"Win32 API Test", NULL);
		return 1;
	}

	HWND hWnd = CreateWindow(windowClass, windowTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 500, 400, NULL, NULL, hInstance, NULL);

	if (!hWnd) {
		MessageBox(NULL, L"CanТt create window!", L"Win32 API Test", NULL);
		return 1;
	}

	ShowWindow(hWnd, nCmdShow); //ждём пока все элементы окна прорисуются
	UpdateWindow(hWnd); //вызов окна функции, вызывает перерисовку рабочей области

	MSG msg;

	while (GetMessage(&msg, NULL, 0, 0)) { //цикл обработки сообщений
		TranslateMessage(&msg); //команда для работы с клавиатурой
		DispatchMessage(&msg); //сообщение виндовс для вызова оконной процедуры
	}

	return msg.wParam;
}

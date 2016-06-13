#include <windows.h>
#include "Header.h"

#define Max 1000

#define EDIT1 11001
#define EDIT2 11002
#define BUTTON1 11003
#define EDIT3 11004
#define BUTTON2 11005

int mat[Max][Max];

HINSTANCE hInst;
HBITMAP hBitmap;
HBITMAP hBitmapStock;
int Length = 0;
int Height = 0;
bool create = false;
const wchar_t g_szClassName[] = L"myWindowClass";


void Desen(HWND hwnd) {

	HBITMAP hBitmap2 = (HBITMAP)LoadImage(hInst, L"plaja.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	HDC hdc = GetDC(hwnd);
	BITMAP 			bitmap;
	HDC 			hdcMem;
	HGDIOBJ 		oldBitmap;

	hdcMem = CreateCompatibleDC(hdc);
	oldBitmap = SelectObject(hdcMem, hBitmap2);

	GetObject(hBitmap2, sizeof(bitmap), &bitmap);
	BitBlt(hdc, 280, 50, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);

	SelectObject(hdcMem, oldBitmap);
	DeleteDC(hdcMem);

	ReleaseDC(hwnd, hdc);
}

void CreateTable(HWND hwnd) {
	const int xOffset = 20;
	const int yOffset = 120;
	for (int i = 0; i < Length; i++) {
		for (int j = 0; j < Height; j++) {
			mat[i][j] = 1;
			CreateWindow(L"BUTTON", L" ", WS_CHILD | WS_VISIBLE | BS_BITMAP,
				xOffset + (i * 20), yOffset + (j * 20), 20, 20,
				hwnd, (HMENU)(2000 * i + j), hInst, NULL);
			SendDlgItemMessage(hwnd, 2000 * i + j, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hBitmapStock);
		}
	}
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_CREATE:
		CreateWindow(L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_NUMBER | WS_BORDER,
			100, 20, 60, 20,
			hwnd, (HMENU)(EDIT1), hInst, NULL);
		CreateWindow(L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_NUMBER | WS_BORDER,
			100, 50, 60, 20,
			hwnd, (HMENU)(EDIT2), hInst, NULL);
		CreateWindow(L"BUTTON", L"HARTA", WS_CHILD | WS_VISIBLE | BS_FLAT,
			20, 80, 140, 30,
			hwnd, (HMENU)(BUTTON1), hInst, NULL);
		CreateWindow(L"BUTTON", L"START", WS_CHILD | WS_VISIBLE | BS_FLAT,
			170, 20, 100, 90,
			hwnd, (HMENU)(BUTTON2), hInst, NULL);
		CreateWindow(L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_NUMBER | WS_BORDER | ES_READONLY,
			280, 20, 150, 20,
			hwnd, (HMENU)(EDIT3), hInst, NULL);


	case WM_COMMAND:
		if (Length != 0)
			for (int i = 0; i < Length; i++) {
				for (int j = 0; j < Height; j++) {
					if (LOWORD(wParam) == 2000 * i + j) {
						if (mat[i][j] == 1) {
							mat[i][j] = 0;
							SendDlgItemMessage(hwnd, 2000 * i + j, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hBitmap);
						}
						else {
							mat[i][j] = 1;
							SendDlgItemMessage(hwnd, 2000 * i + j, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hBitmapStock);
						}
						break;
					}
				}
			}
		if (LOWORD(wParam) == BUTTON1) {
			if (!create) {
				Length = GetDlgItemInt(hwnd, EDIT1, NULL, NULL);
				Height = GetDlgItemInt(hwnd, EDIT2, NULL, NULL);
				if (Length <= 0 || Height <= 0) {
					MessageBox(hwnd, L"Introduceti valorile!", L"Error", MB_ICONERROR);
					break;
				}
				else {
					CreateTable(hwnd);
				}
				create = true;
			}
			else {
				MessageBox(hwnd, L"Harta a fost deja desenata!", L"Error", MB_ICONERROR);
			}
		}
		
		if (LOWORD(wParam) == BUTTON2) {
			Desen(hwnd);
			int **matrice = (int**)malloc(sizeof(int*) * (Length + 1));
			for (int i = 0; i < Length; i++) {
				matrice[i] = (int*)malloc(sizeof(int) * (Height + 1));
			}
			for (int i = 0; i < Length; i++) {
				for (int j = 0; j < Height; j++) {
					matrice[i][j] = mat[i][j];
				}
			}
			creare_histograma(matrice, Length, Height);
			int A = aflare_arie(matrice, Length, Height);
			SetDlgItemInt(hwnd, EDIT3, A, NULL);
			for (int i = 0; i < Length; i++) {
				free(matrice[i]);
			}
			free(matrice);
		}
		break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);

		TextOut(hdc, 20, 22, L"Latime:", 8);
		TextOut(hdc, 20, 52, L"Lungime:", 9);
		

		HDC hMemDC = CreateCompatibleDC(hdc);
		HDC hMemDCStock = CreateCompatibleDC(hdc);

		hBitmap = CreateCompatibleBitmap(hdc, 55, 25);
		hBitmapStock = CreateCompatibleBitmap(hdc, 55, 25);

		SelectObject(hMemDC, hBitmap);
		SelectObject(hMemDCStock, hBitmapStock);

		SetDCBrushColor(hMemDC, RGB(0, 0, 255));
		SetDCBrushColor(hMemDCStock, RGB(255, 216, 55));

		RECT r = { 0 };
		r.left = 0;
		r.right = 55;
		r.top = 0;
		r.bottom = 25;

		FillRect(hMemDC, &r, (HBRUSH)GetStockObject(DC_BRUSH));
		FillRect(hMemDCStock, &r, (HBRUSH)GetStockObject(DC_BRUSH));

		DeleteDC(hMemDC);
		DeleteDC(hMemDCStock);

		ReleaseDC(hwnd, hdc);

		EndPaint(hwnd, &ps);
	}
	break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASSEX wc;
	HWND hwnd;
	MSG Msg;

	
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = g_szClassName;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wc)) {
		MessageBox(NULL, L"Window Registration Failed!", L"Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}


	hwnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		g_szClassName,
		L"Gasirea dreptunghiului de arie maxima",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 500, 400,
		NULL, NULL, hInstance, NULL);

	if (hwnd == NULL) {
		MessageBox(NULL, L"Window Creation Failed!", L"Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	hInst = hInstance;

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);


	while (GetMessage(&Msg, NULL, 0, 0) > 0) {
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return Msg.wParam;
}

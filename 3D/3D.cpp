#include <windows.h>
#include <gdiplus.h>
#include <utility>
#include <string>
#include <sstream>
#include <math.h>

#include "observer.h"
#include "drawable.h"
//#include "resource.h"
using namespace Gdiplus;
#pragma comment(lib, "gdiplus")

const int WIDTH = 800;
const int HEIGHT = 600;


HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("GdiPlusStart");

void OnPaint(HDC hdc, int ID, int x, int y);
void OnPaintA(HDC hdc, int ID, int x, int y, double alpha);
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	HWND     hWnd;
	MSG		 msg;
	WNDCLASS WndClass;

	g_hInst = hInstance;

	ULONG_PTR gpToken;
	GdiplusStartupInput gpsi;
	if (GdiplusStartup(&gpToken, &gpsi, NULL) != Ok)
	{
		MessageBox(NULL, TEXT("GDI+ 라이브러리를 초기화할 수 없습니다."), TEXT("알림"), MB_OK);
		return 0;
	}


	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = L"3D";
	RegisterClass(&WndClass);
	hWnd = CreateWindow(
		L"3D",
		L"3D",
		WS_OVERLAPPEDWINDOW,
		GetSystemMetrics(SM_CXFULLSCREEN) / 2 - WIDTH/2,
		GetSystemMetrics(SM_CYFULLSCREEN) / 2 - HEIGHT/2,
		WIDTH,
		HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL
		);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc, MemDC;

	HBITMAP hBit, OldBit;
	RECT crt;

	static HPEN hPen, oldPen;

	static Cube cube[6][10][10];
	static Function func;

	switch (iMsg)
	{
	case WM_CREATE:
		Observer::Observer();
		hPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
		
		/*for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				cube[0][i][j] = Cube(Point3D(850, -400 + 80 * i, -400 + 80 * j), 20.0);
			}
		}

		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				cube[1][i][j] = Cube(Point3D(-850, -400 + 80 * i, -400 + 80 * j), 20.0);
			}
		}

		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				cube[2][i][j] = Cube(Point3D(-400 + 80 * i, 850, -400 + 80 * j), 20.0);
			}
		}

		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				cube[3][i][j] = Cube(Point3D(-400 + 80 * i, -850, -400 + 80 * j), 20.0);
			}
		}

		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				cube[4][i][j] = Cube(Point3D(-400 + 80 * i, -400 + 80 * j, -850), 20.0);
			}
		}

		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				cube[5][i][j] = Cube(Point3D(-400 + 80 * i, -400 + 80 * j, 850), 20.0);
			}
		}*/
		
		func = Function(Point3D(0, 0, 0));
		InvalidateRect(hWnd, NULL, FALSE);
		//SetTimer(hWnd, 1, 10, 0);
		break;

	case WM_TIMER:
		InvalidateRect(hWnd, NULL, FALSE);
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		GetClientRect(hWnd, &crt);

		MemDC = CreateCompatibleDC(hdc);
		hBit = CreateCompatibleBitmap(hdc, crt.right, crt.bottom);
		OldBit = (HBITMAP)SelectObject(MemDC, hBit);
		oldPen = (HPEN)SelectObject(MemDC, hPen);
		Observer::Axis(MemDC);
		
		/*
		for(int k=0; k<6; k++)
		{
			for (int i = 0; i < 10; i++)
			{
				for (int j = 0; j < 10; j++)
				{
					cube[k][i][j].Update(MemDC);
				}
			}
		}
		*/
		
		
		//func.Update(MemDC);
		func.RayTrace(MemDC);

		BitBlt(hdc, 0, 0, crt.right, crt.bottom, MemDC, 0, 0, SRCCOPY);
		SelectObject(MemDC, OldBit);
		DeleteDC(MemDC);
		SelectObject(MemDC, oldPen);
		//DeleteObject(hPen);
		DeleteObject(hBit);

		EndPaint(hWnd, &ps);
		break;

	case WM_SIZE:
		Observer::Width = LOWORD(lParam);
		Observer::Height = HIWORD(lParam);
		InvalidateRect(hWnd, NULL, FALSE);//
		break;

	case WM_KEYDOWN:
		Observer::Transform(wParam);
		
		/*
		for(int k=0; k<6; k++)
		{
			for (int i = 0; i < 10; i++)
			{
				for (int j = 0; j < 10; j++)
				{
					cube[k][i][j].Rotate(wParam);
				}
			}
		}
		*/
		func.Rotate(wParam);
		func.TranslateLight(wParam);
		InvalidateRect(hWnd, NULL, FALSE);//
		break;

	case WM_MOUSEWHEEL:
		Observer::Scroll(GET_WHEEL_DELTA_WPARAM(wParam));
		InvalidateRect(hWnd, NULL, FALSE);//
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, iMsg, wParam, lParam);
}

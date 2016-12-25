#ifdef THIS_NEVER_HAPPENS
#include <windows.h>
#include <gdiplus.h>
#include <utility>
#include <string>
#include <sstream>
#include <math.h>
//#include "resource.h"

#define LL long long
#define LD long double

using namespace Gdiplus;
using namespace std;

#pragma comment(lib, "gdiplus")



HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("GdiPlusStart");

void OnPaint(HDC hdc, int ID, int x, int y);
void OnPaintA(HDC hdc, int ID, int x, int y, double alpha);
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

LL D=350;
LD ox=1,oy=0,oz=0;
LD delta=10,theta=0,phi=90;
LD origin_x = -100, origin_y, origin_z;
LD v_theta = 0, v_phi = -90;
//LD rad;

const double PI = 3.141592653589793;
const int WIDTH=1200, HEIGHT=800;

pair<double,double> transform(LL Px, LL Py, LL Pz)
{
	Px -= origin_x;
	Py -= origin_y;
	Pz -= origin_z;

	double A = ox*Px + oy*Py+ oz*Pz;
	double n = D*(oz-Pz/A)/sqrt(ox*ox+oy*oy);
	double m = D*(Px*oy-Py*ox)/(A*sqrt(ox*ox+oy*oy));

	return make_pair(m+WIDTH/2,n+HEIGHT/2);
}

pair<double,double> transformL(LD Px, LD Py, LD Pz)
{
	Px -= origin_x;
	Py -= origin_y;
	Pz -= origin_z;

	double A = ox*Px + oy*Py+ oz*Pz;
	double n = D*(oz-Pz/A)/sqrt(ox*ox+oy*oy);
	double m = D*(Px*oy-Py*ox)/(A*sqrt(ox*ox+oy*oy));

	return make_pair(m+WIDTH/2,n+HEIGHT/2);
}

class point3
{
	public:
		LL x;
		LL y;
		LL z;
};

class pointL3
{
	public:
		LD x;
		LD y;
		LD z;
};

class Cube
{
	private:
		pointL3 p;
		pointL3 points[8];
		LD size;
	public:
		void Set(LL x, LL y, LL z)
		{
			this->p.x = x;
			this->p.y = y;
			this->p.z = z;
			for(int i=0; i<8; i++)
			{
				points[i].x=x+(i%2==0)? size/2 : -size/2;
				points[i].y=y+(i/2%2==0)? size/2 : -size/2;
				points[i].z=z+(i/4%2==0)? size/2 : -size/2;
			}
		}
		void Set(LL x, LL y, LL z, LL size)
		{
			this->p.x = x;
			this->p.y = y;
			this->p.z = z;
			this->size = size;

			for(int i=0; i<8; i++)
			{
				points[i].x= x + ((i%2==0)? (size/2) : (-size/2));
				points[i].y= y + (((i/2)%2==0)? (size/2) : (-size/2));
				points[i].z= z + (((i/4)%2==0)? (size/2) : (-size/2));
			}
		}
		void Set(LD x, LD y, LD z)
		{
			this->p.x = x;
			this->p.y = y;
			this->p.z = z;
			for(int i=0; i<8; i++)
			{
				points[i].x=x+(i%2==0)? size/2 : -size/2;
				points[i].y=y+(i/2%2==0)? size/2 : -size/2;
				points[i].z=z+(i/4%2==0)? size/2 : -size/2;
			}
		}
		void Set(LD x, LD y, LD z, LD size)
		{
			this->p.x = x;
			this->p.y = y;
			this->p.z = z;
			this->size = size;

			for(int i=0; i<8; i++)
			{
				points[i].x= x + ((i%2==0)? (size/2) : (-size/2));
				points[i].y= y + (((i/2)%2==0)? (size/2) : (-size/2));
				points[i].z= z + (((i/4)%2==0)? (size/2) : (-size/2));
			}
		}
		void Draw(HDC MemDC)
		{
			int connect1[12]={0,0,1,2,4,4,6,5,0,1,2,3};
			int connect2[12]={1,2,3,3,5,6,7,7,4,5,6,7};

			for(int i=0; i<8; i++)
			{
				double m = transform(points[i].x,points[i].y,points[i].z).first;
				double n = transform(points[i].x,points[i].y,points[i].z).second;
				SetPixel(MemDC, m, n, RGB(255,255,255));
			}
			for(int i=0; i<12; i++)
			{
				double m1 = transform(points[connect1[i]].x, points[connect1[i]].y, points[connect1[i]].z).first;
				double n1 = transform(points[connect1[i]].x, points[connect1[i]].y, points[connect1[i]].z).second;

				double m2 = transform(points[connect2[i]].x, points[connect2[i]].y, points[connect2[i]].z).first;
				double n2 = transform(points[connect2[i]].x, points[connect2[i]].y, points[connect2[i]].z).second;

				MoveToEx(MemDC, m1, n1, NULL);
				LineTo(MemDC, m2, n2);
			}
		}
};


class Function
{
	private:
		pointL3 points[101][101];
	public:
		int theta_rot;
		int X,Y,scale;
		Function(void)
		{
			X = 101;
			Y = 101;
			scale = 4;
			theta_rot = 0;
		}
		LD func(LD x, LD y)
		{
			//return (x*x+y*y)/12;
			//return (x*x-16.0)*(x*x-64.0)/1000.0 + (y*y-16.0)*(y*y-64.0)/1000.0;
			/*LD l0 = floor(x-1+sqrt(8*x*x-8*x+0.5));
			LD ret = 0;
			for(int l=1; l<=l0; l++)
			{
				LD temp = 1;
				for(int k=l; k<=l0; k++)
					temp = temp*pow((1-pow(y,k)),2);
				ret = ret+pow(-1,y)*((pow(y,l*(l+3)/2-(x+1)*l-x*(x-1)/2))/(1-pow(y,l)))*temp;
			}
			ret = ret*1/(1-pow(y,l0));
			return ret;*/
			//return sqrt(100-x*x-y*y);
			return (x*x*x+y*y*y-3*x*y)/100;
			//return 10*sin(sqrt(x*x+y*y))/sqrt(x*x+y*y);
		}
		LD solve(LD D_, LD X_, LD Y_)
		{
			int sign;
			LD alpha = 1;
			LD error = 987654321;
			error = (Y_+alpha) - this->func(D_+alpha , X_+alpha);
			if(error>0)
				sign = 1;
			else if(error<0)
				sign = -1;

			while(abs(error) > 0.1)
			{
				error = (Y_+alpha) - this->func(D_+alpha , X_+alpha);
				if(sign*error > 0)
					alpha *= 2;
				else if(sign*error<0)
					alpha /= 2;
			}
		}
		/*int GetRot()
		{
			return theta_rot;
		}
		void SetRot(int rot)
		{
			theta_rot = rot;
			if(theta_rot>180)
				theta_rot = theta_rot - 360;
			else if(theta_rot < -180)
				theta_rot = theta_rot + 360;
		}*/
		void Set()
		{
			for(int i=0; i<X; i++)
				for(int j=0; j<Y; j++)
				{
					points[i][j].x = (i-X/2)/scale;
					points[i][j].y = (j-Y/2)/scale;
					points[i][j].z = this->func((i-X/2)/scale,(j-Y/2)/scale);
				}
		}
		void Draw(HDC MemDC)
		{
			for(int i=0; i<X; i++)
				for(int j=0; j<Y; j++)
				{
					double m = transformL(points[i][j].x,points[i][j].y,points[i][j].z).first;
					double n = transformL(points[i][j].x,points[i][j].y,points[i][j].z).second;
					SetPixel(MemDC, m, n, RGB(255,255,0));
				}
			for(int i=0; i<X; i++)
				for(int j=0; j<Y-1; j++)
				{
				
					double m1 = transformL(points[i][j].x,points[i][j].y,points[i][j].z).first;
					double n1 = transformL(points[i][j].x,points[i][j].y,points[i][j].z).second;

					double m2 = transformL(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z).first;
					double n2 = transformL(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z).second;

					MoveToEx(MemDC, m1, n1, NULL);
					LineTo(MemDC, m2, n2);
				}

			for(int i=0; i<X-1; i++)
				for(int j=0; j<Y; j++)
				{
				
					double m1 = transformL(points[i][j].x,points[i][j].y,points[i][j].z).first;
					double n1 = transformL(points[i][j].x,points[i][j].y,points[i][j].z).second;

					double m2 = transformL(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z).first;
					double n2 = transformL(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z).second;

					MoveToEx(MemDC, m1, n1, NULL);
					LineTo(MemDC, m2, n2);
				}
		}

};

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
	WndClass.lpszClassName = L"Window Class Name";
	RegisterClass(&WndClass);
	hWnd = CreateWindow(
		L"Window Class Name",
		L"Window Title Name",
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
	HDC hdc, MemDC;
	PAINTSTRUCT ps;

	HBRUSH hBrush, oldBrush;
	HPEN hPen, oldPen;
	HBITMAP hBit, OldBit;
	RECT crt;

	static Cube cube[20];
	static Function function;
	pointL3 old;

	switch (iMsg)
	{
	case WM_CREATE:
		SetTimer(hWnd, 1, 10, 0);
		function.Set();
		cube[0].Set((LD)0.0,(LD)0.0,(LD)0.0,(LD)40.0);
		
		/*cube[1].Set(400,50,50,40);
		cube[2].Set(400,-50,50,40);
		cube[3].Set(400,50,-50,40);
		cube[4].Set(400,-50,-50,40);
		
		cube[5].Set(400,100,0,40);
		cube[6].Set(400,-100,0,40);
		cube[7].Set(400,0,100,40);
		cube[8].Set(400,0,-100,40);

		cube[9].Set(300,50,0,40);
		cube[10].Set(300,-50,0,40);
		cube[11].Set(300,0,50,40);
		cube[12].Set(300,0,-50,40);*/
		break;

	case WM_TIMER:
		InvalidateRect(hWnd, NULL, FALSE);
		ox = sin(phi*PI/180)*cos(theta*PI/180);
		oy = sin(phi*PI/180)*sin(theta*PI/180);
		oz = cos(phi*PI/180);

		/*rad = sqrt(origin_x*origin_x+origin_y*origin_y+origin_z*origin_z);
		origin_x = -rad*sin(v_phi*PI/180)*cos(v_theta*PI/180);
		origin_y = -rad*sin(v_phi*PI/180)*sin(v_theta*PI/180);
		origin_z = -rad*cos(v_phi*PI/180);*/


		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
			case 'H':
				theta+=delta;
				old.x = origin_x;
				old.y = origin_y;
				old.z = origin_z;

				origin_x = cos(delta*PI/180)*old.x - sin(delta*PI/180)*old.y;
				origin_y = sin(delta*PI/180)*old.x + cos(delta*PI/180)*old.y;
				origin_z = old.z;
				break;
			case 'L':
				theta-=delta;
				old.x = origin_x;
				old.y = origin_y;
				old.z = origin_z;

				origin_x = cos(-delta*PI/180)*old.x - sin(-delta*PI/180)*old.y;
				origin_y = sin(-delta*PI/180)*old.x + cos(-delta*PI/180)*old.y;
				origin_z = old.z;
				break;
			case 'J':
				//phi+=delta;
				old.x = origin_x;
				old.y = origin_y;
				old.z = origin_z;

				origin_x = old.x;
				origin_y = cos(delta*PI/180)*old.y - sin(delta*PI/180)*old.z;
				origin_z = sin(delta*PI/180)*old.y + cos(delta*PI/180)*old.z;
				break;
			case 'K':
				//phi-=delta;
				old.x = origin_x;
				old.y = origin_y;
				old.z = origin_z;

				origin_x = old.x;
				origin_y = cos(-delta*PI/180)*old.y - sin(-delta*PI/180)*old.z;
				origin_z = sin(-delta*PI/180)*old.y + cos(-delta*PI/180)*old.z;
				break;
			case 'W':
				phi-=delta;
				break;
			case 'S':
				phi+=delta;
				break;
			case 'A':
				theta+=delta;
				break;
			case 'D':
				theta-=delta;
				break;
			case VK_UP:
				origin_x += ox*5;
				origin_y += oy*5;
				origin_z += oz*5;
				break;
			case VK_DOWN:
				origin_x -= ox*5;
				origin_y -= oy*5;
				origin_z -= oz*5;
				break;
		}
		break;



	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		GetClientRect(hWnd, &crt);

		MemDC = CreateCompatibleDC(hdc);
		hBit = CreateCompatibleBitmap(hdc, crt.right, crt.bottom);
		OldBit = (HBITMAP)SelectObject(MemDC, hBit);
		hBrush = CreateSolidBrush(RGB(255, 255, 255));
		oldBrush = (HBRUSH)SelectObject(MemDC, hBrush);
		hPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
		oldPen = (HPEN)SelectObject(MemDC, hPen);

		SetBkColor(MemDC, RGB(255, 255, 255));


		//for(int i=0; i<13; i++)
		//	cube[i].Draw(MemDC);
		function.Draw(MemDC);
		//cube[0].Draw(MemDC);


		BitBlt(hdc, 0, 0, crt.right, crt.bottom, MemDC, 0, 0, SRCCOPY);
		SelectObject(MemDC, OldBit);
		DeleteDC(MemDC);
		SelectObject(MemDC, oldPen);
		DeleteObject(hPen);
		SelectObject(MemDC, oldBrush);
		DeleteObject(hBrush);
		DeleteObject(hBit);

		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, iMsg, wParam, lParam);
}

/*void OnPaint(HDC hdc, int ID, int x, int y)
{
	Graphics G(hdc);
	HRSRC hResource = FindResource(g_hInst, MAKEINTRESOURCE(ID), TEXT("PNG"));
	if (!hResource)
		return;

	DWORD imageSize = SizeofResource(g_hInst, hResource);
	HGLOBAL hGlobal = LoadResource(g_hInst, hResource);
	LPVOID pData = LockResource(hGlobal);

	HGLOBAL hBuffer = GlobalAlloc(GMEM_MOVEABLE, imageSize);
	LPVOID pBuffer = GlobalLock(hBuffer);
	CopyMemory(pBuffer, pData, imageSize);
	GlobalUnlock(hBuffer);

	IStream *pStream;
	HRESULT hr = CreateStreamOnHGlobal(hBuffer, TRUE, &pStream);

	Image I(pStream);
	pStream->Release();
	if (I.GetLastStatus() != Ok) return;

	G.DrawImage(&I, x, y, I.GetWidth(), I.GetHeight());
}


void OnPaintA(HDC hdc, int ID, int x, int y, double alpha)
{
	Graphics G(hdc);
	HRSRC hResource = FindResource(g_hInst, MAKEINTRESOURCE(ID), TEXT("PNG"));
	if (!hResource)
		return;

	ColorMatrix ClrMatrix =
	{
		1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, alpha, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 1.0f
	};

	ImageAttributes ImgAttr;
	ImgAttr.SetColorMatrix(&ClrMatrix, ColorMatrixFlagsDefault, ColorAdjustTypeBitmap);

	DWORD imageSize = SizeofResource(g_hInst, hResource);
	HGLOBAL hGlobal = LoadResource(g_hInst, hResource);
	LPVOID pData = LockResource(hGlobal);

	HGLOBAL hBuffer = GlobalAlloc(GMEM_MOVEABLE, imageSize);
	LPVOID pBuffer = GlobalLock(hBuffer);
	CopyMemory(pBuffer, pData, imageSize);
	GlobalUnlock(hBuffer);

	IStream *pStream;
	HRESULT hr = CreateStreamOnHGlobal(hBuffer, TRUE, &pStream);

	Image I(pStream);
	pStream->Release();
	if (I.GetLastStatus() != Ok) return;

	RectF destination(0, 0, I.GetWidth(), I.GetHeight());
	G.DrawImage(&I, destination, x, y, I.GetWidth(), I.GetHeight(), UnitPixel, &ImgAttr);
}*/

#endif
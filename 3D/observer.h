#ifndef __OBSERVER__
#define __OBSERVER__

#include <windows.h>
#include "point3d.h"
#include "matrix2by2.h"

#define LD long double
#define PI 3.14159265

class Observer
{
private:
    static Observer::Point3D O;
    static Point3D N;
    static Point3D U;
    static Point3D R;
    static LD D;
    static LD dtheta;

    static const LD step;

    static LD Width;
    static LD Height;

    static Matrix2by2 XY;
    static Matrix2by2 YZ;
    static Matrix2by2 XZ;

    static Matrix2by2 XYi;
    static Matrix2by2 YZi;
    static Matrix2by2 XZi;
	
	enum InversibleFlag { XYf, YZf, ZXf };
    static InversibleFlag flag;

	static Point3D* RotateVec(Point3D A, Point3D B, LD dtheta);
	static LD max3(LD a, LD b, LD c);
	
public:
    Observer();
	static void Axis(HDC MemDC);
	//static void Update(HDC MemDC);
    static void Scroll(int delta);
    static void Transform(WPARAM wParam);
    static Point2D* Projection(Point3D P);
	static Point2D* ProjectionAxis(Point3D P);
};

#endif
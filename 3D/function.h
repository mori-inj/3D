#include "drawee.h"

class Function : public Drawee
{
private:
	int size;
	int scale;
	Point3D L;
public:
	Function() {};
	Function(Point3D origin);
    LD eval(LD x, LD y);
	LD f(Point3D);
	LD diff_X(Point3D t);
	LD diff_Y(Point3D t);
	LD diff_Z(Point3D t);
	LD diff(Point3D t, Point3D T);
	void RayTrace(HDC MemDC);
	void TranslateLight(WPARAM wParam);
};
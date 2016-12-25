#ifndef __DRAWEE__
#define __DRAWEE__

#include <math.h>
#include <vector>
#include <Windows.h>
#include "point3d.h"
#include "edge.h"
#include "observer.h"

using namespace std;

class Drawee
{
protected:
	Point3D origin;
    vector<Point3D*> dots;
    vector<Edge*> edges;

public:
	Drawee() {};
    Drawee(Point3D origin);
    void Update(HDC MemDC);
	void Rotate(WPARAM wParam);
};

#endif
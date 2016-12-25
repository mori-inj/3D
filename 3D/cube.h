#ifndef __CUBE__
#define __CUBE__

#include "drawee.h"

class Cube : public Drawee
{
public:
	Cube() {};
	Cube(Point3D origin, LD r);
};

#endif
#include "drawee.h"

class Function : public Drawee
{
private:
	int size;
	int scale;
public:
	Function() {};
	Function(Point3D origin);
    LD eval(LD x, LD y);
};
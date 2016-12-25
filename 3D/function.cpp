#include "function.h"

Function::Function(Point3D origin) : Drawee(origin)
{
    size = 20;
	scale = 100.0/size;
    int cnt = 0;
    for (int i = -size / 2; i <= size / 2; i++)
    {
        for (int j = -size / 2; j <= size / 2; j++)
        {
            dots.push_back(new Point3D(i*scale+origin.X, j*scale+origin.Y, eval(i*scale+origin.X, j*scale+origin.Y)));
            if (j > -size / 2)
            {
				edges.push_back(new Edge(dots[cnt], dots[cnt - 1]));
            }
            if (i > -size / 2)
            {
				edges.push_back(new Edge(dots[cnt], dots[cnt - size-1]));
            }
            cnt++;
        }
    }
}

LD Function::eval(LD x, LD y)
{
	x -= origin.X;
	y -= origin.Y;
    return (LD)0.01 * (x * x + y * y);
    //return (LD)Math.Sqrt((double)(x * x + y * y));
}
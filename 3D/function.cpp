#include "function.h"

Function::Function(Point3D origin) : Drawee(origin)
{
    size = 40;
	scale = 1;
	L = Point3D(-1300, 0, 2000);
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
    //return (LD)0.01 * (x * x + y * y);
    //return (LD)sqrt(100-(double)(x * x + y * y));
	//return -x-y;
	//return (x*x-16.0)*(x*x-64.0)/100.0 + (y*y-16.0)*(y*y-64.0)/100.0;
	return ((x+y)*(x+y)-16.0)*((x+y)*(x+y)-64.0)/100.0 + ((x-y)*(x-y)-16.0)*((x-y)*(x-y)-64.0)/100.0;
}

LD Function::f(Point3D t)
{
	//return eval(t.X, t.Y)-t.Z;
	//return (t.X-origin.X)*(t.X-origin.X)+(t.Y-origin.Y)*(t.Y-origin.Y)+(t.Z-origin.Z)*(t.Z-origin.Z)-100;
	//return t.X+t.Y+t.Z;
	//return eval(t.X, t.Y)-t.Z; 
	return eval(t.X, t.Y)-t.Z; 
}
LD Function::diff_X(Point3D t)
{
	//return (LD)0.01 * (2*t.X - 2*origin.X);
	//return 2*t.X - 2*origin.X;
	//return 1;
	//return (4* t.X * t.X * t.X - 160 * t.X)/100.0;
	return (8*t.X*(t.X*t.X + 3*t.Y*t.Y-48))/100.0;
}
LD Function::diff_Y(Point3D t)
{
	//return (LD)0.01 * (2*t.Y - 2*origin.Y);
	//return 2*t.Y - 2*origin.Y;
	//return 1;
	//return (4* t.Y * t.Y * t.Y - 160 * t.Y)/100.0;
	return (8*t.Y*(3*t.X*t.X + t.Y*t.Y-48))/100.0;
}
LD Function::diff_Z(Point3D t)
{
	//return -1;//(LD)0.01 * (2*t.Z - 2*origin.Z);
	//return 2*t.Z - 2*origin.Z;
	//return 1;
	//return -1;
	return -1;
}
LD Function::diff(Point3D t, Point3D T)
{
	return T.X * diff_X(t) + T.Y * diff_Y(t) + T.Z * diff_Z(t);
}

void Function::RayTrace(HDC MemDC)
{
	int Width = Observer::Width;
	int Height = Observer::Height;
	HPEN hPen, oldPen;

	for(int i=0; i<(int)dots.size(); i++)
	{
		try
		{
			Point2D dot = *Observer::Projection(*dots[i]);
			LD m = (double)(dot.X);
			LD n = (double)(dot.Y);
			if(0 <= m && m <= Observer::Width && 0 <= n && n <= Observer::Height)
			{
				if(maxX < m)
				{
					maxX = m;
				}
				if(minX > m)
				{
					minX = m;
				}
				if(maxY < n)
				{
					maxY = n;
				}
				if(minY > n)
				{
					minY = n;
				}
			}
		}
		catch(int e) {}
	}

	/*LD** screen = new LD*[Height];
	for(int i=0; i<Height; i++)
	{
		screen[i] = new LD[Width];
		for(int j=0; j<Width; j++)
			screen[i][j] = -1e9;
	}*/

	int shade_scale = 1;
	int loop = 20;
	Point3D O = Observer::O;	
	LD error = 2.0;
	LD dist1 = 10*sqrt( (origin - O) * (origin - O) );
	LD dist2 = -10*sqrt( (origin - O) * (origin - O) );

	for(int i=((int)minY/shade_scale)*shade_scale; i<=600-minY; i+=shade_scale)
	{
		for(int j=((int)minX/shade_scale)*shade_scale; j<=maxX; j+=shade_scale)
		{
			Point3D Q = *Observer::InverseProjection(Point2D(j-Width/2,-(i-Height/2)));
			Point3D T = *Observer::InverseProjectionVector(Point2D(j-Width/2,-(i-Height/2)));
			int k1,k2;

			Point3D temp1 = Q+dist1*T;
			LD t1 = dist1 - f(temp1) / diff(temp1,T);
			for(k1 = 0; k1<loop && abs(f(temp1)) > error; k1++)
			{
				temp1 = Q+t1*T;
				t1 -= f(temp1) / diff(temp1,T);
			}

			Point3D temp2 = Q+dist2*T;
			LD t2 = dist2 - f(temp2) / diff(temp2,T);
			for(k2 = 0; k2<loop && abs(f(temp2)) > error; k2++)
			{
				temp2 = Q+t2*T;
				t2 -= f(temp2) / diff(temp2,T);
			}

			Point3D W;
			if (k1 == loop && k2 == loop)
			{
				continue;
			}
			else if (k1 == loop)
				W = temp2;
			else if (k2 == loop)
				W = temp1;
			else
			{
				if((O-temp1)*(O-temp1) < (O-temp2)*(O-temp2))
					W = temp1;
				else
					W = temp2;
			}


			Point3D V = W - L;
			Point3D grad = Point3D(diff_X(W), diff_Y(W), diff_Z(W));
			grad.norm();
			Point3D _V = V - 2* ((V*grad) / (grad*grad)) * grad;
			LD cross = _V * (O-W);
			LD light = cross / (sqrt(_V*_V)*sqrt((O-W)*(O-W)));
			light = (light+1)/2;
			//light *= light;

			/*LD cross = _V*grad;
			LD light = cross / (sqrt(V*V)*sqrt(grad*grad));*/

			hPen = CreatePen(PS_SOLID, 1, RGB(0*light, 255*light, 255*light));
			oldPen = (HPEN)SelectObject(MemDC, hPen);
			MoveToEx(MemDC, (int)j, (int)i, NULL);
			LineTo(MemDC, (int)j+1, (int)i+1);
			DeleteObject(hPen);
		}

		/*for(int i=((int)minY/shade_scale)*shade_scale; i<=600-minY; i++)
		{
			for(int j=((int)minX/shade_scale)*shade_scale; j<=maxX; j++)
			{
				LD light = screen[i][j];
				if(light<0)
				{
					int x1 = j/shade_scale*shade_scale;
					int x2 = j/shade_scale*shade_scale + shade_scale;
					int y1 = i/shade_scale*shade_scale;
					int y2 = i/shade_scale*shade_scale + shade_scale;

					light = ( screen[y1][x1]*(x2-j)*(y2-i)
							+ screen[y1][x2]*(j-x1)*(y2-i)
							+ screen[y2][x1]*(x2-j)*(i-y1)
							+ screen[y2][x2]*(j-x1)*(i-y1) ) / ((x2-x1)*(y2-y1));
				}

				hPen = CreatePen(PS_SOLID, 1, RGB(0*light, 255*light, 255*light));
				oldPen = (HPEN)SelectObject(MemDC, hPen);
				MoveToEx(MemDC, (int)j, (int)i, NULL);
				LineTo(MemDC, (int)j+1, (int)i+1);
				DeleteObject(hPen);
			}
		}*/
		
	}

}

void Function::TranslateLight(WPARAM wParam)
{	
	LD delta = 500;
	switch (wParam)
    {
        case 'T':
            L.Z+=delta;
            break;
        case 'G':
            L.Z-=delta;
            break;
        case 'F':
            L.Y-=delta;
            break;
        case 'H':
			L.Y+=delta;
            break;
        case 'R':
            L.X+=delta;
            break;
        case 'Y':
            L.X-=delta;
            break;
		default:
			return;
	}

}
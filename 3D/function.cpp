#include "function.h"

Function::Function(Point3D origin) : Drawee(origin)
{
    size = 400;
	scale = (int)4000.0/size;
	L = Point3D(-1300, 0,2000);
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
    return (LD)sqrt(40000-(double)(x * x + y * y));
	//return -x-y;
}

LD Function::f(Point3D t)
{
	//return eval(t.X, t.Y)-t.Z;
	return (t.X-origin.X)*(t.X-origin.X)+(t.Y-origin.Y)*(t.Y-origin.Y)+(t.Z-origin.Z)*(t.Z-origin.Z)-40000;
	//return t.X+t.Y+t.Z;
}
LD Function::diff_X(Point3D t)
{
	//return (LD)0.01 * (2*t.X - 2*origin.X);
	return 2*t.X - 2*origin.X;
	//return 1;
}
LD Function::diff_Y(Point3D t)
{
	//return (LD)0.01 * (2*t.Y - 2*origin.Y);
	return 2*t.Y - 2*origin.Y;
	//return 1;
}
LD Function::diff_Z(Point3D t)
{
	//return -1;//(LD)0.01 * (2*t.Z - 2*origin.Z);
	return 2*t.Z - 2*origin.Z;
	//return 1;
}
LD Function::diff(Point3D t, Point3D T)
{
	return T.X * diff_X(t) + T.Y * diff_Y(t) + T.Z * diff_Z(t);
}

void Function::RayTrace(HDC MemDC)
{
	printf("start!!\n");
	fflush(stdout);
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

	/*int** screen = new int*[Height];
	for(int i=0; i<Height; i++)
		screen[i] = new int[Width];*/
	for(int i=minY; i<600; i+=1)
	{
		for(int j=minX; j<maxX; j+=1)
		{
			Point3D Q = *Observer::InverseProjection(Point2D(j-Width/2,-(i-Height/2)));
			Point3D T = *Observer::InverseProjectionVector(Point2D(j-Width/2,-(i-Height/2)));
			Point3D O = Observer::O;


			LD dist1 = 10*sqrt( (origin - O) * (origin - O) );
			Point3D temp1 = Q+dist1*T;
			LD t1 = dist1 - f(temp1) / diff(temp1,T);
			//printf("Q: (%Lf, %Lf, %Lf)\n",Q.X, Q.Y, Q.Z);
			//printf("bf: (%d, %d): %Lf (%Lf,%Lf,%Lf)\n",j,i,f(Q+t0*T),(Q+t0*T).X,(Q+t0*T).Y,(Q+t0*T).Z);
			for(int k = 0; k<20 && abs(f(temp1)) > 1; k++)
			{
				temp1 = Q+t1*T;
				t1 -= f(temp1) / diff(temp1,T);
			}

			LD dist2 = -10*sqrt( (origin - O) * (origin - O) );
			Point3D temp2 = Q+dist2*T;
			LD t2 = dist2 - f(temp2) / diff(temp2,T);
			//printf("Q: (%Lf, %Lf, %Lf)\n",Q.X, Q.Y, Q.Z);
			//printf("bf: (%d, %d): %Lf (%Lf,%Lf,%Lf)\n",j,i,f(Q+t0*T),(Q+t0*T).X,(Q+t0*T).Y,(Q+t0*T).Z);
			for(int k = 0; k<20 && abs(f(temp2)) > 1; k++)
			{
				temp2 = Q+t2*T;
				t2 -= f(temp2) / diff(temp2,T);
			}

			Point3D temp;

			if (abs(f(temp1)) > 1 &&  abs(f(temp2)) > 1)
			{
				//printf("!!!!!!!!!!!!!!!!!!!!!\n");
				continue;
			}
			else if (abs(f(temp1)) > 1)
				temp = temp2;
			else if (abs(f(temp2)) > 1)
				temp = temp1;
			else
			{
				if((O-temp1)*(O-temp1) < (O-temp2)*(O-temp2))
					temp = temp1;
				else
					temp = temp2;
			}

			//printf("at: (%d, %d): %Lf (%Lf,%Lf,%Lf)\n\n",j,i,f(Q+t0*T),(Q+t0*T).X,(Q+t0*T).Y,(Q+t0*T).Z);
			if(abs(f(temp)) <= 1)
			{
				//printf("yes\n");
				//fflush(stdout);
				Point3D W = temp;
				Point3D V = W - L;
				Point3D grad = Point3D(diff_X(W), diff_Y(W), diff_Z(W));
				grad.norm();
				Point3D _V = V - 2* ((V*grad) / (grad*grad)) * grad;
				LD cross = _V * (O-W);
				if(cross < 0)
				{
					printf("|_V|: %Lf\n|O-W|:%Lf\n|_V|*|O-W|:%Lf\ncross:%Lf\ncos:%Lf\n%Lf %Lf %Lf -> %d %d\n\n",
						sqrt(_V*_V),
						sqrt((O-W)*(O-W)),
						sqrt(_V*_V)*sqrt((O-W)*(O-W)),
						cross,
						cross / (sqrt(_V*_V)*sqrt((O-W)*(O-W))),
						W.X, W.Y, W.Z,i,j);
					fflush(stdout);
				}
				LD light = cross / (sqrt(_V*_V)*sqrt((O-W)*(O-W)));
				light = abs(light);

				hPen = CreatePen(PS_SOLID, 1, RGB(0*light, 255*light, 255*light));
				oldPen = (HPEN)SelectObject(MemDC, hPen);
				MoveToEx(MemDC, (int)j, (int)i, NULL);
				LineTo(MemDC, (int)j+1, (int)i+1);
				DeleteObject(hPen);
			}
		}
		
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
#include "cube.h"

Cube::Cube(Point3D origin, LD r) : Drawee(origin)
{
    dots.push_back(new Point3D(origin.X + r, origin.Y - r, origin.Z - r));
    dots.push_back(new Point3D(origin.X + r, origin.Y + r, origin.Z - r));
    dots.push_back(new Point3D(origin.X - r, origin.Y + r, origin.Z - r));
    dots.push_back(new Point3D(origin.X - r, origin.Y - r, origin.Z - r));

    dots.push_back(new Point3D(origin.X + r, origin.Y - r, origin.Z + r));
    dots.push_back(new Point3D(origin.X + r, origin.Y + r, origin.Z + r));
    dots.push_back(new Point3D(origin.X - r, origin.Y + r, origin.Z + r));
    dots.push_back(new Point3D(origin.X - r, origin.Y - r, origin.Z + r));

    edges.push_back(new Edge(dots[0], dots[1]));
    edges.push_back(new Edge(dots[1], dots[2]));
    edges.push_back(new Edge(dots[2], dots[3]));
    edges.push_back(new Edge(dots[3], dots[0]));

    edges.push_back(new Edge(dots[4], dots[5]));
    edges.push_back(new Edge(dots[5], dots[6]));
    edges.push_back(new Edge(dots[6], dots[7]));
    edges.push_back(new Edge(dots[7], dots[4]));

    edges.push_back(new Edge(dots[0], dots[4]));
    edges.push_back(new Edge(dots[1], dots[5]));
    edges.push_back(new Edge(dots[2], dots[6]));
    edges.push_back(new Edge(dots[3], dots[7]));
}

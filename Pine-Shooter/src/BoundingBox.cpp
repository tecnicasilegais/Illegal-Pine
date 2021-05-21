
#include "../headers/BoundingBox.h"

// Initialize with default Coordinates
BoundingBox::BoundingBox()
{
    Coordinates[LOWER_LEFT] = Point(-1, -1);
    Coordinates[UPPER_LEFT] = Point(-1, 1);
    Coordinates[UPPER_RIGHT] = Point(1, 1);
    Coordinates[LOWER_RIGHT] = Point(1, -1);
    mid = Point(0,0);

}

void BoundingBox::draw()
{
    glLineWidth(2);
    glPointSize(5);
    glBegin(GL_LINE_LOOP);
    for (auto &vertex : Coordinates)
    {
        glVertex3f(vertex.x, vertex.y, vertex.z);
    }
    glEnd();
    glBegin(GL_POINTS);
    glVertex3f(mid.x, mid.y, mid.z);
    glEnd();
}

void BoundingBox::print()
{
    for (auto &vertice : Coordinates)
    {
        vertice.print();
    }
}

Point BoundingBox::get_min()
{
    return Coordinates[LOWER_LEFT];
}

Point BoundingBox::get_max()
{
    return Coordinates[UPPER_RIGHT];
}

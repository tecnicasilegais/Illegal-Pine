
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
    glLineWidth(1.5);
    glPointSize(3);
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
    GLfloat x = mid.x;
    GLfloat y = mid.y;
    for(auto &coord : Coordinates)
    {
        if(coord.x < x){x = coord.x;}
        if(coord.y < y){y = coord.y;}
    }
    return Point(x,y);
}

Point BoundingBox::get_max()
{
    GLfloat x = mid.x;
    GLfloat y = mid.y;
    for(auto &coord : Coordinates)
    {
        if(coord.x > x){x = coord.x;}
        if(coord.y > y){y = coord.y;}
    }
    return Point(x,y);
}

void BoundingBox::update(BoundingBox& origin)
{
    for(int i=0; i<4; i++)
    {
        calc_point(origin.Coordinates[i], this->Coordinates[i]);
    }
    calc_point(origin.mid, this->mid);
    calc_point(origin.projectile_origin, this->projectile_origin);
    min = this->get_min();
    max = this->get_max();
}

void calc_point(Point& p, Point &out) {

    GLfloat ponto_novo[4];
    GLfloat matriz_gl[4][4];
    int  i;

    glGetFloatv(GL_MODELVIEW_MATRIX,&matriz_gl[0][0]);

    for(i=0; i<4; i++) {
        ponto_novo[i] = matriz_gl[0][i] * p.x +
                        matriz_gl[1][i] * p.y +
                        matriz_gl[2][i] * p.z +
                        matriz_gl[3][i];
    }
    out.x = ponto_novo[0];
    out.y = ponto_novo[1];
    out.z = ponto_novo[2];

}

bool BoundingBox::collision_detect(BoundingBox &other)
{
    if(any_of(other.Coordinates, other.Coordinates+4,
              [&](Point p){return p <= max && p>= min;})
              ||
       any_of(this->Coordinates, this->Coordinates+4,
                   [&](Point p){return p <= other.max && p>= other.min;}))
    {
        return true;
    }
    return false;
}

GLfloat z_vector_product(Point &v1, Point &v2)
{
    GLfloat res = v1.x * v2.y - (v1.y * v2.x);
    return res;
}
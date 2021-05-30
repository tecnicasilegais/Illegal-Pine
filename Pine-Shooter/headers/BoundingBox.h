
#ifndef BoundingBox_hpp
#define BoundingBox_hpp

#include <iostream>

using namespace std;

#ifdef WIN32

#include <windows.h>
#include <GL/glut.h>

#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#endif

#ifdef __linux__
#include <GL/freeglut.h>
#include<bits/stdc++.h>
#endif

#include "Point.h"
#include <vector>

//include order in vector!!!
#define LOWER_LEFT 0
#define UPPER_LEFT 1
#define UPPER_RIGHT 2
#define LOWER_RIGHT 3

class BoundingBox
{
    Point Coordinates[4];
    Point mid;
public:
    BoundingBox();
    void draw();
    void print();
    Point get_min();
    Point get_max();
    Point projectile_origin = Point(0, 1);
    Point min, max;
    void update(BoundingBox& origin);
/*
 * Uses simple bounding box logic
 */
    bool collision_detect(BoundingBox &other);
/*
 * vector product
 * use for rotated objects
 */
    bool rotated_collision_detect(BoundingBox &other);
};

void calc_point(Point &p, Point &out);

/*
 * calculates Z of vector product.
 * This is the only result needed for collision detection
 */
GLfloat z_vector_product(Point &v1, Point &v2);
#endif 

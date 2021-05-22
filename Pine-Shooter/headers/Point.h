//
// Initial class given by professor Marcio Sarroglia Pinho (PUCRS)
// Modified by Eduardo Andrade and Marcelo Heredia
//

#ifndef Point_hpp
#define Point_hpp

#include <iostream>
#include <cmath>

#ifdef WIN32

#include <windows.h>
#include <GL/glut.h>

#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#endif

#ifdef __linux__
#include <GL/glut.h>
#include<bits/stdc++.h>
#endif


using namespace std;

class Point
{
public:
    GLfloat x, y, z;
    Point();
    Point(float x, float y, float z = 0);
    bool operator==(Point other) const;
    void print() const;
    void multiply(double x, double y, double z);
    void sum(double x, double y, double z);
};

Point get_min(Point p1, Point p2);
Point get_max(Point p1, Point p2);

#endif /* Ponto_hpp */

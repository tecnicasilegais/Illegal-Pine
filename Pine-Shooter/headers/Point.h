//
// Initial class given by professor Marcio Sarroglia Pinho (PUCRS)
// Modified by Eduardo Andrade and Marcelo Heredia
//

#ifndef Point_hpp
#define Point_hpp

#include <iostream>
#include <cmath>


using namespace std;

class Point
{
public:
    float x, y, z;
    Point();
    Point(float x, float y, float z = 0);
    bool operator==(Point other) const;
    void print() const;
    void multiply(double x, double y, double z);
    void set(float x, float y, float z = 0);
    void sum(double x, double y, double z);
};

Point get_min(Point p1, Point p2);
Point get_max(Point p1, Point p2);

#endif /* Ponto_hpp */

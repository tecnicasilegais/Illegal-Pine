
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
#include <GL/glut.h>
#include<bits/stdc++.h>
#endif

#include "Point.h"
#include <vector>

class BoundingBox
{
    vector<Point> Vertices;
    Point min, max;
public:
    int menorPosicao{};
    BoundingBox();
    unsigned long size();
    int index(Point p);
    void desenha_poligono();
    void desenha_vertice(int r, int g, int b, int pos);
    void desenha_vertices();
    void imprime();
    void initialize(Point initial);
    void insere_vertice(Point);
    void insere_vertice(int index, Point p);
    Point get_min();
    Point get_max();
    Point get_vertice(int);
};


#endif 


#include "../headers/BoundingBox.h"
#include "../headers/Point.h"

#ifdef __linux__
#include<bits/stdc++.h>
#endif

#include <chrono>

BoundingBox::BoundingBox()
= default;

void BoundingBox::insere_vertice(int index, Point p)
{
    if (p.x < min.x)
    {
        min.x = p.x;
    }
    if (p.x > max.x)
    {
        max.x = p.x;
    }
    if (p.y < min.y)
    {
        min.y = p.y;
    }
    if (p.y > max.y)
    {
        max.y = p.y;
    }

    Vertices.insert(Vertices.begin() + index, p);
}

void BoundingBox::insere_vertice(Point p)
{
    if (p.x < min.x)
    {
        min.x = p.x;
    }
    if (p.x > max.x)
    {
        max.x = p.x;
    }
    if (p.y < min.y)
    {
        min.y = p.y;
    }
    if (p.y > max.y)
    {
        max.y = p.y;
    }

    Vertices.push_back(p);
}

Point BoundingBox::get_vertice(int i)
{
    return Vertices[i];
}

int BoundingBox::index(Point p)
{
    auto it = std::find(Vertices.begin(), Vertices.end(), p);

    if (it != Vertices.end())
    {
        return it - Vertices.begin();
    }
    return -1;
}

void BoundingBox::desenha_poligono()
{
    glBegin(GL_LINE_LOOP);
    for (auto &Vertice : Vertices)
    {
        glVertex3f(Vertice.x, Vertice.y, Vertice.z);
    }
    glEnd();
}

void BoundingBox::desenha_vertices()
{
    glBegin(GL_POINTS);
    for (auto &vertice : Vertices)
    {
        glVertex3f(vertice.x, vertice.y, vertice.z);
    }
    glEnd();
}

void BoundingBox::desenha_vertice(int r, int g, int b, int pos)
{
    glColor3f(r, g, b); // R, G, B  [0..1]
    glBegin(GL_POINTS);
    glVertex3f(Vertices[pos].x, Vertices[pos].y, Vertices[pos].z);
    glEnd();
}

void BoundingBox::imprime()
{
    for (auto &vertice : Vertices)
    {
        vertice.imprime();
    }
}

unsigned long BoundingBox::size()
{
    return Vertices.size();
}

void BoundingBox::initialize(Point initial)
{
    min = initial;
    max = initial;
    Vertices.push_back(initial);
}

Point BoundingBox::get_min()
{
    return min;
}

Point BoundingBox::get_max()
{
    return max;
}

void insere_unico(BoundingBox &pol, Point p)
{
    if (pol.index(p) != -1)
    {
        return;
    }
    pol.insere_vertice(p);
}

bool insere_unico(BoundingBox &pol, int index, Point p)
{
    if (pol.index(p) != -1)
    {
        return false;
    }
    pol.insere_vertice(index, p);
    return true;
}


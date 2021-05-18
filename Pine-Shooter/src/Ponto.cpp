//
// Initial class given by professor Marcio Sarroglia Pinho
// Modified by Eduardo Andrade and Marcelo Heredia
//

#include "math.h"
#include "../headers/Ponto.h"

Ponto::Ponto()
{
    x = y = z = 0;
}

Ponto::Ponto(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

void Ponto::set(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

void Ponto::imprime()
{
    cout << "(" << x << ", " << y << ")" << flush;
}

void Ponto::multiply(double x, double y, double z)
{
    this->x *= x;
    this->y *= y;
    this->z *= z;
}

void Ponto::sum(double x, double y, double z)
{
    this->x += x;
    this->y += y;
    this->z += z;
}

Ponto get_max(Ponto p1, Ponto p2)
{
    Ponto max;

    max.x = (p2.x > p1.x) ? p2.x : p1.x;
    max.y = (p2.y > p1.y) ? p2.y : p1.y;
    max.z = (p2.z > p1.x) ? p2.z : p1.z;
    return max;
}

Ponto get_min(Ponto p1, Ponto p2)
{
    Ponto min;

    min.x = (p2.x < p1.x) ? p2.x : p1.x;
    min.y = (p2.y < p1.y) ? p2.y : p1.y;
    min.z = (p2.z < p1.x) ? p2.z : p1.z;

    return min;
}

bool Ponto::operator==(const Ponto other) const
{
    return (x == other.x) && (y == other.y);
}

/**
 * Calcula a intersecao entre 2 retas, KL e MN (no plano "XY" Z = 0)
 * @param k - ponto inicial da reta KL
 * @param l - ponto final da reta KL
 * @param m - ponto inicial da reta MN
 * @param n - ponto final da reta MN
 * @param p - ponto de intersecao, caso haja
 * @returns false quando nao ha intersecao, true quando ha
 */
bool intersec2d(Ponto k, Ponto l, Ponto m, Ponto n, Ponto &p)
{
    double det;
    Ponto s1 = Ponto(l.x - k.x, l.y - k.y);
    Ponto s2 = Ponto(n.x - m.x, n.y - m.y);
    det = -(s2.x) * (s1.y) + (s1.x) * (s2.y);

    if (det == 0.0)
    {
        return false;
    } //nao ha intersecao

    float s = (-(s1.y) * (k.x - m.x) + (s1.x) * (k.y - m.y)) / det;
    float t = ((s2.x) * (k.y - m.y) - (s2.y) * (k.x - m.x)) / det;

    if (s >= 0.0 && s <= 1.0 && t >= 0.0 && t <= 1.0)
    {
        p = Ponto(k.x + (t * s1.x), k.y + (t * s1.y));
        return true;
    }
    return false;
}

/**
 * Calcula o produto escalar entre os vetores V1 e V2
 */
double prod_escalar(Ponto v1, Ponto v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

/**
 * Calcula o produto vetorial entre os vetores V1 e V2 e guarda em x,y,z de vResult
 */
void prod_vetorial(Ponto v1, Ponto v2, Ponto &vResult)
{
    vResult.x = v1.y * v2.z - (v1.z * v2.y);
    vResult.y = v1.z * v2.x - (v1.x * v2.z);
    vResult.z = v1.x * v2.y - (v1.y * v2.x);
}

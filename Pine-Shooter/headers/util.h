
#ifndef util_hpp
#define util_hpp

#ifdef WIN32

#include <windows.h>
#include <GL/glut.h>

#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#endif

#ifdef __linux__
#include <GL/glut.h>
#endif

using namespace std;

#include <iostream>
#include <image_libs/TextureClass.h>

#define P_FILE "img/fatsquirrel.png"


GLuint initTexture ()
{
    return LoadTexture (P_FILE);
}

#endif
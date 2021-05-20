#include "headers/Engine.h"



void displayBackground(ImageClass &bg)
{
    glMatrixMode(GL_PROJECTION);//Define os limites logicos da area OpenGL dentro da Janela
    glLoadIdentity();
    glOrtho(0, ORTHO_X, 0, ORTHO_Y,-1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    float zoomH = (glutGet(GLUT_WINDOW_WIDTH))/(double)bg.SizeX();
    float zoomV = (glutGet(GLUT_WINDOW_HEIGHT))/(double)bg.SizeY();
    bg.SetZoomH(zoomH);
    bg.SetZoomV(zoomV);
    bg.SetPos(0, 0);
    bg.Display();
}


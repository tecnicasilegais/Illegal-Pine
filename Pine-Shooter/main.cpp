// **********************************************************************
// Pine Shooter
// Eduardo Andrade e Marcelo Heredia
// **********************************************************************


#include <iostream>
#include <cmath>
#include <ctime>
#include <fstream>

using namespace std;
#ifdef WIN32

#include <windows.h>
#include <GL/glut.h>
#else

#include <sys/time.h>

#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#endif

#ifdef __linux__
#include <GL/glut.h>
#endif

#include "image_libs/ImageClass.h"
#include "headers/Point.h"
#include "headers/BoundingBox.h"
#include "headers/Temporizador.h"
#include "headers/Engine.h"

// Global Variables
Temporizador T;
double accum_delta_t = 0;

//declarando poligonos que serao utilizados
BoundingBox A;
vector<Point> B, binit;

//Limites logicos da area de desenho

float angle = 0.0;
float walk = 0.0;
float scale = 1;
double n_frames = 0;
double tempo_total = 0;
GLfloat acum = 0;


ImageClass bg;


GLfloat AspectRatio, AngY=0;
GLuint PLAYER;


void DesenhaCubo ()
{
    glBegin ( GL_QUADS );
    // Front Face
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
    glEnd();
    /*
    glBegin ( GL_QUADS );
    // Front Face
    glTexCoord2f(acum/12.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
    glTexCoord2f((acum+1)/12.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
    glTexCoord2f((acum+1)/12.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
    glTexCoord2f(acum/12.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
    glEnd();*/

}

void CalculaPonto(Point p, Point &out) {

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


void init_textures()
{
    if(!bg.Load(BG_FILE)){exit(666);} //load BG image

    PLAYER = LoadTexture(PLAYER_FILE);


}

void init()
{
    //allow transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    init_textures();
    // Define a cor do fundo da tela (AZUL)
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);


    A.insere_vertice(Point(20,20, -0.5));
    A.insere_vertice(Point(20,25, -0.5));
    A.insere_vertice(Point(25,25, -0.5));
    A.insere_vertice(Point(22.5,22.5, -0.5));
    A.insere_vertice(Point(25,20, -0.5));


    binit.emplace_back(-1,-1);
    binit.emplace_back(1,-1);
    binit.emplace_back(1,1);
    binit.emplace_back(-1,1);

    B.emplace_back(20,20);
    B.emplace_back(20,25);
    B.emplace_back(25,25);
    B.emplace_back(25,20);

}


void animate()
{
    double dt;
    dt = T.get_delta_t();
    accum_delta_t += dt;
    tempo_total += dt;
    n_frames++;

    if (accum_delta_t > 1.0 / 30) // fixa a atualizacao da tela em 30
    {
        accum_delta_t = 0;
        glutPostRedisplay();
    }
    if (tempo_total > 5.0)
    {
        cout << "Tempo Acumulado: " << tempo_total << " segundos. ";
        cout << "Nros de Frames sem desenho: " << n_frames << endl;
        cout << "FPS(sem desenho): " << n_frames / tempo_total << endl;
        tempo_total = 0;
        n_frames = 0;
    }
}

/**
 * trata o redimensionamento da janela OpenGL
 * @param w - largura
 * @param h - altura
 */
void reshape(int w, int h)
{
    // Reset the coordinate system before modifying
    AspectRatio = 1.0f * w / h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Define a area a ser ocupada pela area OpenGL dentro da Janela
    glViewport(0, 0, w, h);
    // Define os limites logicos da area OpenGL dentro da Janela
    glOrtho(0, ORTHO_X,
            0, ORTHO_Y,
            -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void display(void)
{
    // Limpa a tela coma cor de fundo
    glClear(GL_COLOR_BUFFER_BIT);

    displayBackground(bg);


    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glColor3f(1,1,1);
    //glTranslatef ( -2.0f, 2.0f, -5.0f );
    glTranslatef ( 25, 25.0f, -1);
    //glScalef(2*scale,4*scale,1);
    glScalef(2, 2,1);
    glRotatef(angle, 0,0,1);
    glBindTexture (GL_TEXTURE_2D, PLAYER);
    DesenhaCubo();

    glDisable( GL_TEXTURE_2D);

    glLineWidth(0.5);
    glColor3f(1,0,0); // R, G, B  [0..1]

    glBegin(GL_LINE_LOOP);
    for (auto &Vertice : binit)
    {
        glVertex3f(Vertice.x, Vertice.y, Vertice.z);
    }
    glEnd();

    glPopMatrix();

    glBegin(GL_LINES);
    glVertex3f(0,7.6,0);
    glVertex3f(50,7.6,0);
    glEnd();
/*

    glPushMatrix();
    glLoadIdentity();
    glColor3f(1,0,0); // R, G, B  [0..1]
    //glTranslatef(22.5+walk, 22.5, 0);
    //glRotatef(angle, 0,0,1);
    //glTranslatef(-22.5, -22.5, 0);
    glLineWidth(3);
    A.desenha_poligono();
    glPopMatrix();

    for(int i=0; i<binit.size(); i++)
    {
        Point p;
        CalculaPonto(binit[i], p);
        B[i].x = p.x;
        B[i].y = p.y;
    }

    glPopMatrix();
    glLineWidth(0.5);
    glColor3f(1,0,0); // R, G, B  [0..1]
    glBegin(GL_LINE_LOOP);
    for (auto &Vertice : B)
    {
        glVertex3f(Vertice.x, Vertice.y, Vertice.z);
    }
    glEnd();

    glLineWidth(0.5);
    glColor3f(0,0,1); // R, G, B  [0..1]
    glBegin(GL_LINE_LOOP);
    for (auto &Vertice : binit)
    {
        glVertex3f(Vertice.x, Vertice.y, Vertice.z);
    }
    glEnd();
    //print p1_new e p2_new*/
    glutSwapBuffers();
}

/**
 * Informa quantos frames se passaram no tempo informado.
 * @param tempo - Tempo em segundos
 */
void conta_tempo(double tempo)
{
    Temporizador T;

    unsigned long cont = 0;
    cout << "Inicio contagem de " << tempo << "segundos ..." << flush;
    while (true)
    {
        tempo -= T.get_delta_t();
        cont++;
        if (tempo <= 0.0)
        {
            cout << "fim! - Passaram-se " << cont << " frames." << endl;
            break;
        }
    }

}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27:        // Termina o programa qdo
            exit(0);   // a tecla ESC for pressionada
            break;
        case 't':
            conta_tempo(3);
            break;
        case ' ':
            acum +=1;
            if(acum >= 11){acum = 0;}
            glutPostRedisplay();
            break;
        case 'a':
            walk += -1;
            break;
        case 'd':
            walk += 1;
            break;
        default:
            break;
    }
}

void arrow_keys(int a_keys, int x, int y)
{
    switch (a_keys)
    {
        case GLUT_KEY_UP:       // Se pressionar UP
            //glutFullScreen(); // Vai para Full Screen
            scale += 0.5;
            break;
        case GLUT_KEY_DOWN:     // Se pressionar UP
            // Reposiciona a janela
            //glutPositionWindow(50, 50);
            //glutReshapeWindow(700, 500);
            scale -= 0.5;
            break;
        case GLUT_KEY_RIGHT:
            angle -= 1;
            break;
        case GLUT_KEY_LEFT:
            angle += 1;
            break;
        default:
            break;
    }
}

int main(int argc, char** argv)
{
    cout << "Programa OpenGL" << endl;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
    glutInitWindowPosition(0, 0);

    glutInitWindowSize(1200, 650);

    glutCreateWindow("Pine Shooter");

    init();

    glutDisplayFunc(display);

    glutIdleFunc(animate);

    glutReshapeFunc(reshape);

    glutKeyboardFunc(keyboard);

    glutSpecialFunc(arrow_keys);

    glutMainLoop();

    return 0;
}

// **********************************************************************
// Pine Shooter
// Eduardo Andrade e Marcelo Heredia
// **********************************************************************
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
#endif

#include <iostream>
#include "image_libs/ImageClass.h"
#include "headers/Point.h"
#include "headers/BoundingBox.h"
#include "headers/Temporizador.h"
#include "headers/Engine.h"

// Global Variables
Temporizador T;
double accum_delta_t = 0;

//defines where ALL TEXTURES are initialized (-1,-1) (1,1) square.
BoundingBox INIT_POSITION;

bool debug = false;

float angle = 0.0;
double n_frames = 0;
double tempo_total = 0;
GLfloat acum = 0;


ImageClass bg;
GameTextures* gt;
vector<Building> buildings;
vector<Enemy> enemies;
Player* player;





void init_textures()
{
    if(!bg.Load(BG_FILE)){exit(666);} //load BG image
    gt = new GameTextures;

}

void init_buildings()
{
    auto b1 = Building(BUILD1);
    b1.position = Point(175,FLOOR_H+10);
    b1.scale = Point(7,10);
    buildings.emplace_back(b1);

    auto b2 = Building(BUILD2);
    b2.position = Point(130,FLOOR_H+10);
    b2.scale = Point(7,10);
    buildings.emplace_back(b2);

    auto b3 = Building(BUILD3);
    b3.position = Point(210,FLOOR_H+10);
    b3.scale = Point(7,10);
    buildings.emplace_back(b3);

    auto b4 = Building(BUILD4);
    b4.position = Point(100,FLOOR_H+10);
    b4.scale = Point(7,10);
    buildings.emplace_back(b4);


    auto stk = Building(PW_STICK, 0);
    stk.position = Point(150, FLOOR_H+7);
    stk.scale = Point(1,7);
    buildings.emplace_back(stk);

    auto pin = Building(PW_SPIRAL, 0);
    pin.position = Point(150, FLOOR_H+15);
    pin.scale = Point(7,7);
    pin.rotation_incr = 15;
    buildings.emplace_back(pin);
}

void init_game_objects()
{
    init_buildings();
    //TODO init enemies
    player = new Player();
}

void init()
{
    //allow transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    init_textures();
    init_game_objects();
    // Define a cor do fundo da tela (AZUL)
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

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
    {/*
        cout << "Tempo Acumulado: " << tempo_total << " segundos. ";
        cout << "Nros de Frames sem desenho: " << n_frames << endl;
        cout << "FPS(sem desenho): " << n_frames / tempo_total << endl;
        */tempo_total = 0;
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

    display_background(bg);

    if(debug)
        draw_floor();

    // display buildings
    for(auto & building : buildings)
    {
        building.draw((*gt), debug);
    }

    player->display_health((*gt));

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
        case 27:       //esc
            delete gt;
            exit(0);
        case '1':
            debug = !debug;
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
            for(auto & b : buildings)
            {
                b.health -= 1;
            }
            //walk += -1;
            break;
        case 'd':
            player->lives -=1;
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
            //scale += 0.5;
            break;
        case GLUT_KEY_DOWN:     // Se pressionar UP
            // Reposiciona a janela
            //glutPositionWindow(50, 50);
            //glutReshapeWindow(700, 500);
            //scale -= 0.5;
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

    delete gt;
    return 0;
}

//
// Created by Eduardo Andrade e Marcelo Heredia on 20/05/2021.
//

#ifndef PINE_SHOOTER_ENGINE_H
#define PINE_SHOOTER_ENGINE_H


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
#include <map>
#include "Point.h"

//Ortho
#define ORTHO_X 50
#define ORTHO_Y 50

#define BG_FILE "img/Pine-BG.png"
#define EXPLOSION_T "img/Explosion.png"
#define BUILD1_T "img/build1.png"
#define BUILD2_T "img/build2.png"
#define BUILD3_T "img/build3.png"
#define BUILD4_T "img/build4.png"
#define PW_STICK_T "img/stick.png"
#define PW_SPIRAL_T "img/pinwheel.png"
#define PLAYER_FILE "img/fatsquirrel.png"
#define PLAYER_AMMO_T "img/pine2.png"
#define EAGLE_T "img/eagle.png"
#define RAVEN_T "img/raven.png"
#define OWL_T "img/owl.png"
#define ENEMY_AMMO_T "img/bird_poop.png"

#define PLAYER 1
#define PLAYER_AMMO 2
#define EAGLE 3
#define RAVEN 4
#define OWL 5
#define ENEMY_AMMO 6
#define EXPLOSION 7
#define BUILD1 8
#define BUILD2 9
#define BUILD3 10
#define BUILD4 11
#define PW_STICK 12
#define PW_SPIRAL 13


class GameTextures
{
private:
    map<int, GLuint> textures;
public:
    GameTextures();
    GLuint owl;
    void init();
    void draw_texture(int n);
    GLuint get(int n);
};
void DesenhaCubo ();
void displayBackground(ImageClass &bg);


#endif //PINE_SHOOTER_ENGINE_H

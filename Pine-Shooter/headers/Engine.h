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
#include "Point.h"

//Ortho
#define ORTHO_X 50
#define ORTHO_Y 50

#define BG_FILE "img/Pine-BG.png"
#define EXPLOSION "img/Explosion.png"

#define BUILD1 "img/build1.png"
#define BUILD2 "img/build2.png"
#define BUILD3 "img/build3.png"
#define BUILD4 "img/build4.png"
#define PW_STICK "img/stick.png"
#define PW_SPIRAL "img/pinwheel.png"

#define PLAYER_FILE "img/fatsquirrel.png"
#define PLAYER_AMMO "img/pine2.png"

#define EAGLE "img/eagle.png"
#define RAVEN "img/raven.png"
#define OWL "img/owl.png"
#define ENEMY_AMMO "bird-poop.png"




void displayBackground(ImageClass &bg);


#endif //PINE_SHOOTER_ENGINE_H

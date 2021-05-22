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
#include "BoundingBox.h"

//Ortho
#define ORTHO_X 240
#define ORTHO_Y 130
#define FLOOR_H 19.6
#define BB_GL_COLOR glColor3f(1,0,0)

#define HORIZONTAL_SPRITE 0
#define VERTICAL_SPRITE 1

#define BG_FILE "img/background.png"
#define EXPLOSION_T "img/Explosion.png"
#define BUILD1_T "img/build1.png"
#define BUILD2_T "img/build2.png"
#define BUILD3_T "img/build3.png"
#define BUILD4_T "img/build4.png"
#define PW_STICK_T "img/stick.png"
#define PW_SPIRAL_T "img/pinwheel.png"
#define PLAYER_FILE "img/squirrel.png"
#define PLAYER_AMMO_T "img/pine.png"
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
    void draw_texture(int n);
    GLuint get(int n);
};


class GameObject
{
public:
    BoundingBox bb;
    Point position;
    Point scale=Point(1,1);
    int model;
};

class Building: public GameObject
{
public:
    Building(int model, int n_sprites, int s_orientation);
    bool is_destroyed();
    void draw(GameTextures &gt);
private:
    bool destroyed;
    int n_sprites, s_orientation, sprite, lives;
};

class Player: public GameObject
{
public:
    int direction, speed;
    int lives = 3;
    float rotation;
};

class Enemy: public GameObject
{
public:
    int direction, speed;

private:


};


void DesenhaCubo ();
void draw_texture(GLuint& n);
void draw_sprite();

void display_background(ImageClass &bg);
void draw_floor();

#endif //PINE_SHOOTER_ENGINE_H

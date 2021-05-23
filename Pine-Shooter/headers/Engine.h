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
#define O_TIME 15

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
#define HEALTH_T "img/life.png"

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
#define HEALTH 14

class GameTextures
{
private:
    map<int, GLuint> textures;
public:
    GameTextures();
    void draw_texture(int n);
    void draw_sprite(int n, int orientation, GLfloat sprite, GLfloat total_sprites);
    GLuint get(int n);

};


class GameObject
{
public:
    BoundingBox bb, root;
    GLfloat rotation;
    GLfloat rotation_incr;
    Point position, speed;
    Point scale = Point(1,1);
    bool active = true;
    bool moving = false;
    int model, sprite=0, n_sprites=0, s_orientation;
    void handle_rotation() const;
    void walk_mru(double dt, Point& direction);
    void walk_bezier(double dt);
    virtual void draw(GameTextures &gt, bool debug);
};

class Building: public GameObject {
public:
    int health;
    explicit Building(int model, int n_sprites = 3, int s_orientation = VERTICAL_SPRITE);
    void draw(GameTextures &gt, bool debug) override;
};

class Player: public GameObject
{
public:
    explicit Player(int model);
    Point aim_direction, move_dir;
    int health = 3;
    GLfloat max_rotation = 80.0f;

    void display_health(GameTextures& gt) const;
    void rotate_l();
    void rotate_r();
    void walk_mru(double dt);
    void walk_l();
    void walk_r();
};

class Enemy: public GameObject
{
public:
    void walk_mru(double dt);
};

class Explosion: public GameObject
{
public:
    explicit Explosion(int id);
    int id_target;
    int slowness = 3;
    int repetitions = 0;
    bool ended=false;
    void draw(GameTextures& gt);
};

void display_background(ImageClass &bg);
void draw_floor();
void draw_square(const Point &min, const Point &max);

#endif //PINE_SHOOTER_ENGINE_H

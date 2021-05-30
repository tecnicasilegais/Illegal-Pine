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
#include <GL/freeglut.h>
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

double n_frames = 0;
double tempo_total = 0;

int active_buildings;
int alive_enemies;
int spin_hits = 0;
bool game_over = false;

GLfloat win_animation_time = 0.0f;
GLfloat total_animation_time = 4.0f;

ImageClass bg;
GameTextures* gt;
vector<Building> buildings;
vector<Enemy> enemies;
list<Explosion> explosions; //maybe use list
list<Projectile> projectiles;

//TODO projectile
Player* player;

void init_textures()
{
    if(!bg.Load(BG_FILE)){exit(666);} //load BG image
    gt = new GameTextures;
}

void init_buildings()
{
    auto b1 = Building(BUILD1, Point(175,FLOOR_H+10), Point(7,10));
    buildings.emplace_back(b1);

    auto b2 = Building(BUILD2, Point(130,FLOOR_H+10), Point(7,10));
    buildings.emplace_back(b2);

    auto b3 = Building(BUILD3, Point(210,FLOOR_H+10), Point(7,10));
    buildings.emplace_back(b3);

    auto b4 = Building(BUILD4, Point(100,FLOOR_H+10), Point(7,10));
    buildings.emplace_back(b4);

    active_buildings = buildings.size(); //next two are indestructible

    auto stk = Building(PW_STICK, Point(150, FLOOR_H+7), Point(1,7), 0);
    stk.health = -1; //infinite health
    buildings.emplace_back(stk);

    auto pin = Building(PW_SPIRAL, Point(150, FLOOR_H+15), Point(7,7), 0);
    pin.health = -1; //infinite health
    pin.rotation_incr = 7.5;
    buildings.emplace_back(pin);
}

void init_enemies()
{

    auto positions = enemy_positions();
    srand(time(nullptr));
    for(int i=0; i<12; i++)
    {
        auto index = rand() % positions.size();
        auto pos = positions[index];
        positions.erase(positions.begin() + index);

        int model = 3 + i % 3; // 3 -> EAGLE, 4 -> RAVEN, 5 -> OWL

        auto e = Enemy(model, pos, gt->get_scaled(model, 4));
        e.moving = true;
        enemies.emplace_back(e);
    }
    alive_enemies = enemies.size();
}

void init_game_objects()
{
    init_buildings();
    init_enemies();
    player = new Player(PLAYER, Point(50, FLOOR_H+20, -0.8), gt->get_scaled(PLAYER, 4));
    player->rotation_incr = 10;
}

void explode_here(Point position)
{
    auto ex = Explosion(position);
    ex.scale = gt->get_scaled(EXPLOSION, 5);
    explosions.emplace_back(ex);
}

//remove inactive enemies and projectiles
void clean()
{
    auto end = remove_if(
            enemies.begin(),
            enemies.end(),
            [](Enemy const &e)
            {
                return !e.active;
            });
    enemies.erase(end, enemies.end());

    auto itr = projectiles.cbegin();

    while(itr != projectiles.cend())
    {
        auto curr = itr++;
        if(!curr->active)
        {
            projectiles.erase(curr);
        }
    }
}

void handle_collisions()
{
    for(auto &p : projectiles)
    {
        //todo verify player collision

        if(p.model == PLAYER_AMMO)
        {
            for(auto &enemy : enemies)
            {
                if(enemy.active && enemy.collided(p))
                {
                    enemy.active = false;
                    p.active = false;
                    alive_enemies--;
                    explode_here(enemy.position);
                    continue;
                }
            }
        }
        for(auto &build : buildings)
        {
            if(build.active && build.collided(p))
            {
                p.active = false;
                if(!build.active)//died
                    active_buildings--;
                explode_here(build.position);
                continue;
            }
        }
    }
    clean();
}

void start_end_animation()
{
    if(!game_over)
    {
        player->position = Point((GLfloat)ORTHO_X/2, (GLfloat)ORTHO_Y/2);
        player->rotation = 0;
        game_over = true;
    }
}

void win_animation(GLfloat t)
{
    win_animation_time += t;
    if(win_animation_time < total_animation_time)
    {
        player->scale += gt->get_scaled(player->model, 0.5);
    }
}

bool win_criteria()
{
    if(alive_enemies == 0)
    {
        debug = false;
        player->aiming = false;
        start_end_animation();
        return true;
    }
    return false;
}
bool lose_criteria()
{
    if(active_buildings == 0 || player->health == 0)
    {
        game_over = true;
        return true;
    }
    return false;
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

        if(!lose_criteria())
        {
            if(win_criteria())
            {
                win_animation(1.0f/30);
            }
            else
            {
                handle_collisions();
            }
        }

        for(auto & enemy : enemies)
        {
            if(enemy.moving)
            {
                enemy.walk_mru(1.0/30);
            }
        }
        for(auto & proj : projectiles)
        {
            if(proj.moving && proj.active)
            {
                proj.oblique_throw(1.0/30);
            }
        }

        if(player->moving)
        {
            player->walk_mru(1.0/30);
        }

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
    player->display_health((*gt));

    if(debug)
        draw_floor();

    // display buildings
    for(auto & building : buildings)
    {
        building.draw((*gt), debug);
    }

    for(auto & enemy : enemies)
    {
        enemy.draw((*gt), debug);
    }

    player->draw((*gt), debug);

    for(auto & proj : projectiles)
    {
        proj.draw((*gt), debug);
    }

    for(auto & explosion : explosions)
    {
        explosion.draw((*gt));
    }

    if(game_over)
    {
        glPushMatrix();
        glRasterPos2f((GLfloat)ORTHO_X/2, (GLfloat)ORTHO_Y/2);
        glColor3f(0,0,0);
        glScalef(0.3,0.3,1);
        if(lose_criteria())
        {
            glutStrokeString(GLUT_STROKE_ROMAN, (unsigned char*) "YOU LOSE!");
        }
        else if(win_animation_time >= total_animation_time)
        {
            glutStrokeString(GLUT_STROKE_ROMAN, (unsigned char*) "YOU WIN!");
        }
        glPopMatrix();
    }

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
    if(win_criteria() || lose_criteria())
    {
        if(key == 27)
        {
            delete gt;
            exit(0);
        }
    }
    else
    {
        switch (key)
        {
            case 27:       //esc
                delete gt;
                exit(0);
            case '1':
                debug = !debug;
                break;
            case '2':
                player->aiming = !player->aiming;
                break;
            case 't':
                conta_tempo(3);
                break;
            case ' ':
                projectiles.push_back(player->shoot((*gt)));
                break;
            case 'a':
                player->walk_l();
                break;
            case 'd':
                player->walk_r();
                break;
            case 'q': //rotate left
                player->rotate_l();
                break;
            case 'e':
                player->rotate_r();
                break;
            case 'w':
                player->increase_str();
                break;
            case 's':
                player->decrease_str();
                break;
            default:
                break;
        }
    }
}

void arrow_keys(int a_keys, int x, int y)
{
    switch (a_keys)
    {
        case GLUT_KEY_UP:
            //glutFullScreen(); // Vai para Full Screen
            break;
        case GLUT_KEY_DOWN:
            // Reposiciona a janela
            //glutPositionWindow(50, 50);
            //glutReshapeWindow(700, 500);
            break;
        case GLUT_KEY_RIGHT:
            //player->walk_r();
            break;
        case GLUT_KEY_LEFT:
            //player->walk_l();
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

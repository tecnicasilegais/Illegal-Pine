#include "headers/Engine.h"

/*
 * Game Texture class and methods
 */
GameTextures::GameTextures()
{
    textures[PLAYER] = LoadTexture(PLAYER_FILE);
    textures[PLAYER_AMMO] = LoadTexture(PLAYER_AMMO_T);
    textures[RAVEN] = LoadTexture(RAVEN_T);
    textures[EAGLE] = LoadTexture(EAGLE_T);
    textures[OWL] = LoadTexture(OWL_T);
    textures[ENEMY_AMMO] = LoadTexture(ENEMY_AMMO_T);
    textures[BUILD1] = LoadTexture(BUILD1_T);
    textures[BUILD2] = LoadTexture(BUILD2_T);
    textures[BUILD3] = LoadTexture(BUILD3_T);
    textures[BUILD4] = LoadTexture(BUILD4_T);
    textures[PW_STICK] = LoadTexture(PW_STICK_T);
    textures[PW_SPIRAL] = LoadTexture(PW_SPIRAL_T);
    textures[EXPLOSION] = LoadTexture(EXPLOSION_T);
}
GLuint GameTextures::get(int n)
{
    return textures[n];
}

/*
 * Game Object classes, subclasses and methods for each of them
 */
Building::Building(int model, int n_sprites, int s_orientation)
{
    this->model = model;
    this->n_sprites = n_sprites;
    this->s_orientation = s_orientation;
    destroyed = false;
    lives = 2;
    sprite = 0;
}
bool Building::is_destroyed()
{
    return destroyed;
}
void Building::draw(GameTextures &gt)
{
    GLuint tex = gt.get(model);
}

void draw_texture(GLuint& tex)
{
    glPushMatrix();
    glColor3f(1,1,1);
    glEnable(GL_TEXTURE_2D);
    glTranslatef ( 15, 15.0f, -1);
    glBindTexture (GL_TEXTURE_2D, tex);
    DesenhaCubo();

    glDisable( GL_TEXTURE_2D);
    glPopMatrix();
}
void DesenhaCubo ()
{
    glBegin ( GL_QUADS );
    // Front Face
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  0.0f);
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



/*
 * draw background using given ImageClass img
 */
void display_background(ImageClass &bg)
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

/*
 * Draw a line on the floor
 * Should only be used on debug mode
 */
void draw_floor()
{
    glBegin(GL_LINES);
    BB_GL_COLOR;
    glVertex3f(0, FLOOR_H, -1);
    glVertex3f(ORTHO_X, FLOOR_H,  -1);
    glEnd();
}

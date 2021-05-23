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
    textures[HEALTH] = LoadTexture(HEALTH_T);
}
GLuint GameTextures::get(int n)
{
    return textures[n];
}
void GameTextures::draw_texture(int n)
{
    Point min = Point(0,0);
    Point max = Point(1,1);

    glColor3f(1,1,1);
    glEnable(GL_TEXTURE_2D);
    glBindTexture (GL_TEXTURE_2D, textures[n]);

    draw_square(min, max);

    glDisable( GL_TEXTURE_2D);
}
void GameTextures::draw_sprite(int n, int orientation, GLfloat sprite, GLfloat total_sprites)
{
    Point min = Point(0,0);
    Point max = Point(1,1);
    if(orientation == VERTICAL_SPRITE)
    {
        min.y = sprite/total_sprites;
        max.y = (sprite+1)/total_sprites;
    }
    else //HORIZONTAL_SPRITE
    {
        min.x = sprite/total_sprites;
        max.x = (sprite+1)/total_sprites;
    }

    glColor3f(1,1,1);
    glEnable(GL_TEXTURE_2D);
    glBindTexture (GL_TEXTURE_2D, textures[n]);

    draw_square(min, max);

    glDisable( GL_TEXTURE_2D);
}

/*
 * Handles a GameObject sequential rotation
 * Increments the rotation everytime it gets displayed
 * Subtracts 360 to keep it low
 */
void GameObject::handle_rotation() const
{
    if(rotation != 0)
    {
        glRotatef(rotation,0,0,1);
    }
}

void GameObject::draw(GameTextures &gt, bool debug)
{
    glPushMatrix();
    glTranslatef(position.x, position.y, position.z);
    glScalef(scale.x, scale.y, 1);

    this->handle_rotation();

    if(n_sprites > 0)
    {
        gt.draw_sprite(model, s_orientation, (GLfloat)sprite, (GLfloat) n_sprites);
    }
    else
    {
        gt.draw_texture(model);
    }
    bb.update(root);

    glPopMatrix();
    if(debug && active)
    {
        BB_GL_COLOR;
        bb.draw();
    }

}

/*
 * Calculates movement MRU of given object
 * If object is outside the GL Ortho, it will not be updated
 */
void GameObject::walk_mru(double dt, Point& direction)
{
    // Position = Position0 + speed * time (* direction)
    Point S;
    S.x = dt * this->speed.x * direction.x;
    this->position.x += S.x;
    //position += speed * direction * dt;
}
/*
 * Building Constructor
 * @param model Number of the model stored in GameTextures
 * @param n_sprites Number of sprites in the texture (default = 3)
 * @param s_orientation VERTICAL_SPRITE or HORIZONTAL_SPRITE (default = Vertical)
 */
Building::Building(int model, int n_sprites, int s_orientation)
{
    this->model = model;
    this->n_sprites = n_sprites;
    this->s_orientation = s_orientation;
    health = 2;
    sprite = health;
    rotation = 0;
    rotation_incr = 0;
}
void Building::draw(GameTextures &gt, bool debug)
{
    if(rotation_incr > 0)
    {
        rotation += rotation_incr;
        if(rotation > 360)
        {
            rotation = rotation - 360;
        }
    }
    sprite = health;
    GameObject::draw(gt, debug);
}

void Enemy::walk_mru(double dt) {
    if(bb.get_min() >= Point(ORTHO_X, ORTHO_Y) ||
       bb.get_max() <= Point(0,0))
    {
        cout << "outside limits object" << endl;
        return;
    }
    auto dir = Point(-1,0);
    GameObject::walk_mru(dt, dir);
}

Explosion::Explosion(int id)
{
    model = EXPLOSION;
    id_target = id;
    s_orientation = HORIZONTAL_SPRITE;
    n_sprites = 12;
    sprite = 0;
}
/*
 * Explosion drawing method
 * Does a single explosion animation
 * stops by setting ended to true.
 */
void Explosion::draw(GameTextures& gt)
{
    if(!ended)
    {
        GameObject::draw(gt, false);
        repetitions++;
        if(repetitions >= slowness)
        {
            sprite++;
            repetitions = 0;
        }
        if(sprite == 12)
            ended = true;
    }
}

Player::Player(int model)
{
    this->model = model;
    aim_direction = Point(0,1);
}
void Player::display_health(GameTextures& gt) const
{
    glPushMatrix();
    glTranslatef(30,10,-1);
    glScalef(10,2,1);
    gt.draw_sprite(14, VERTICAL_SPRITE, (GLfloat)health, 4);

    glPopMatrix();
}
/*
 * Rotates player in counter-clockwise
 * Uses the predefined rotation_incr
 * Limits to max_rotation
 */
void Player::rotate_l()
{
    rotation += rotation_incr;
    if(rotation > max_rotation)
        rotation = max_rotation;
}
/*
 * Rotates player in clockwise
 * Uses the predefined rotation_incr
 * Limits to max_rotation
 */
void Player::rotate_r()
{
    rotation -= rotation_incr;
    if(rotation < -max_rotation)
        rotation = -max_rotation;
}

void Player::walk_mru(double dt) {
    if(move_dir.x == -1 && bb.get_min().x <= 0)
    {
        cout << "Player is touching left limit" << endl;
        moving = false;
        return;
    }
    if(move_dir.x == 1 && bb.get_max().x >= ORTHO_X)
    {
        cout << "Player is touching right limit" << endl;
        moving = false;
        return;
    }
    GameObject::walk_mru(dt, move_dir);
    moving = false;
}

void Player::walk_l()
{
    this->move_dir = Point(-1,0);
    this->moving = true;
}
void Player::walk_r()
{
    this->move_dir = Point(1,0);
    this->moving = true;
}


/*
 * draw background using given ImageClass img
 */
void display_background(ImageClass &bg)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, ORTHO_X, 0, ORTHO_Y,-1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    float zoomH = (glutGet(GLUT_WINDOW_WIDTH))/(float)bg.SizeX();
    float zoomV = (glutGet(GLUT_WINDOW_HEIGHT))/(float)bg.SizeY();
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

void draw_square(const Point &min, const Point &max)
{
    //min.print();
    //max.print();
    glBegin (GL_QUADS );
    // Front Face
    glTexCoord2f(min.x, min.y); //image coords
    glVertex3f(-1.0f, -1.0f, 0.0f); //gl coords
    glTexCoord2f(max.x, min.y);
    glVertex3f(1.0f, -1.0f, 0.0f);
    glTexCoord2f(max.x, max.y);
    glVertex3f(1.0f, 1.0f, 0.0f);
    glTexCoord2f(min.x, max.y);
    glVertex3f(-1.0f, 1.0f, 0.0f);
    glEnd();
}

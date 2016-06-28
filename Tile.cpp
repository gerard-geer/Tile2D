#include "Tile.h"

Tile::Tile()
{
}

Tile::~Tile()
{
    delete this->m;
}

void Tile::init(GLfloat x, GLfloat y, tile_plane plane, GLfloat width, GLfloat height, bool trans)
{
    this->m = new BasicMatrix(3,3);
    this->m->set(0,2, x);
    this->m->set(1,2, y);
    this->m->set(0,0, width);
    this->m->set(1,1, height);
    this->plane = plane;
    this->trans = trans;
    this->texFlip = 0;
    // Oh why look at that our unique identifier is already figured out for us.
    this->id = (unsigned long) this;
}

GLfloat Tile::getParallaxFactor(tile_plane plane)
{
    switch(plane)
    {
        case PLANE_BG: return .1;
        case PLANE_NEG_4: return .2;
        case PLANE_NEG_3: return .4;
        case PLANE_NEG_2: return .6;
        case PLANE_NEG_1: return .8;
        case PLANE_PLAYFIELD_A: return 1.0;
        case PLANE_PLAYFIELD_B: return 1.0;
        case PLANE_PLAYFIELD_C: return 1.0;
        case PLANE_POS_1: return 2.0;
        case PLANE_POS_2: return 4.0;
    }
}

GLfloat Tile::getTileDepth(tile_plane plane)
{
    switch(plane)
    {
        case PLANE_BG: return .999;
        case PLANE_NEG_4: return .9;
        case PLANE_NEG_3: return .8;
        case PLANE_NEG_2: return .7;
        case PLANE_NEG_1: return .6;
        case PLANE_PLAYFIELD_A: return .5;
        case PLANE_PLAYFIELD_B: return .4;
        case PLANE_PLAYFIELD_C: return .3;
        case PLANE_POS_1: return .2;
        case PLANE_POS_2: return .101;
    }
}

GLfloat Tile::getX() const
{
    return this->m->get(0,2);
}

GLfloat Tile::getY() const
{
    return this->m->get(1,2);
}

tile_plane Tile::getPlane() const
{
    return this->plane;
}

GLfloat Tile::getWidth() const
{
    return this->m->get(0,0);
}

GLfloat Tile::getHeight() const
{
    return this->m->get(1,1);
}

bool Tile::hasTrans() const
{
    return this->trans;
}

GLuint Tile::getTextureFlip() const
{
    return this->texFlip;
}

BasicMatrix * Tile::getMatrix() const
{
    return this->m;
}

unsigned long Tile::getID() const
{
    return this->id;
}

void Tile::setX(GLfloat x)
{
    this->m->set(0,2, x);
}

void Tile::setY(GLfloat y)
{
    this->m->set(1,2, y);
}

void Tile::setPlane(tile_plane plane)
{
    this->plane = plane;
}

void Tile::setWidth(GLfloat width)
{
    this->m->set(0,0, width);
}

void Tile::setHeight(GLfloat height)
{
    this->m->set(1,1, height);
}

void Tile::setTransparency(bool trans)
{
    this->trans = trans;
}

void Tile::setTextureFlip(GLuint flip)
{
    this->texFlip = flip;
}

void Tile::destroy()
{
    delete this->m;
}
#include "Tile.h"
#include <iostream>

// Initialize the scrolling coefficients.
float Tile::scrollCoeffs[10] = {1.5, 1.25, 1.0, 1.0, 1.0, .85, .70, .525, .3, .05};

Tile::Tile()
{
}

Tile::~Tile()
{
    delete this->pd;
}

void Tile::init(GLfloat x, GLfloat y, tile_plane plane, GLfloat width, GLfloat height, bool trans)
{
    // Create the position and dimension matrix and initialize it. (The modelview matrix).
    this->pd = new BasicMatrix(3,3);
    this->pd->set(0,2, x);
    this->pd->set(1,2, y);
    this->pd->set(0,0, width);
    this->pd->set(1,1, height);
    
    // Create and initialize the rotation matrix. Since BasicMatrices are initialized to
    // an identity matrix, we don't need to set any values.
    this->r = new BasicMatrix(3,3);
    this->rotation = 0.0; // We do need to store the current rotation.
    
    // Create the swap-space matrix.
    this->mult = new BasicMatrix(3,3);
    
    // Store the other stuff.
    this->plane = plane;
    this->trans = trans;
    this->texFlip = 0;
    this->ignoreScroll = false;
    
    // Oh why look at that our unique identifier is already figured out for us.
    this->id = (unsigned long) this;
}

GLfloat Tile::getParallaxFactor(tile_plane plane)
{
    return Tile::scrollCoeffs[plane%NUM_PLANES];
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
    return this->pd->get(0,2);
}

GLfloat Tile::getY() const
{
    return this->pd->get(1,2);
}

tile_plane Tile::getPlane() const
{
    return this->plane;
}

GLfloat Tile::getWidth() const
{
    return this->pd->get(0,0);
}

GLfloat Tile::getHeight() const
{
    return this->pd->get(1,1);
}

bool Tile::hasTrans() const
{
    return this->trans;
}

bool Tile::ignoresScroll() const
{
	return this->ignoreScroll;
}

GLuint Tile::getTextureFlip() const
{
    return this->texFlip;
}

GLfloat Tile::getRotation() const
{
    return this->rotation;
}

BasicMatrix * Tile::getCompoundMat()
{
    // Plop the position and dimension matrix into
    // the swap space matrix.
    this->mult->set(this->pd);
    // Multiply that matrix by the rotation matrix.
    this->mult->multiplyBy(this->r);
    
    // Return the swap space matrix. This way neither
    // of the two important matrices are modified by
    // this function, and no extraneous matrices are
    // made.
    return this->mult;
}

BasicMatrix * Tile::getPositionMat()
{
    return this->pd;
}

BasicMatrix * Tile::getRotationMat()
{
    return this->r;
}

unsigned long Tile::getID() const
{
    return this->id;
}

float Tile::getScrollCoeff(tile_plane plane) const
{
    return Tile::scrollCoeffs[plane%NUM_PLANES];
}

void Tile::setX(GLfloat x)
{
    this->pd->set(0,2, x);
}

void Tile::setY(GLfloat y)
{
    this->pd->set(1,2, y);
}

void Tile::setPlane(tile_plane plane)
{
    this->plane = plane;
}

void Tile::setWidth(GLfloat width)
{
    this->pd->set(0,0, width);
}

void Tile::setHeight(GLfloat height)
{
    this->pd->set(1,1, height);
}

void Tile::setTransparency(bool trans)
{
    this->trans = trans;
}

void Tile::setIgnoreScroll(bool ignoreScroll)
{
	this->ignoreScroll = ignoreScroll;
}

void Tile::setRotation(GLfloat rotation)
{
    this->rotation = rotation;
    this->r->set(0,0,  cos(this->rotation));
    this->r->set(0,1, -sin(this->rotation));
    this->r->set(1,0,  sin(this->rotation));
    this->r->set(1,1,  cos(this->rotation));
}

void Tile::setTextureFlip(GLuint flip)
{
    this->texFlip = flip;
}

void Tile::setScrollCoeff(tile_plane plane, float coeff)
{
    scrollCoeffs[plane%NUM_PLANES] = coeff;
}

void Tile::report()
{
    std::cout << "Tile: " << this->id  << " trans: " << this->trans << " plane: " << this->plane << std::endl;
}

void Tile::destroy()
{
    delete this->pd;
}

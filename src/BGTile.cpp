#include "BGTile.h"

BGTile::BGTile()
: Tile()
{
}

BGTile::~BGTile()
{
}

void BGTile::init(GLfloat x, GLfloat y, GLfloat width, GLfloat height, char * texture)
{
    Tile::init(x, y, PLANE_BG, width, height, false);
    this->texture = texture;
}

void BGTile::setPlane(tile_plane plane)
{
    return;
}

void BGTile::render(Renderer * r)
{
    // Pull the BGTile's shader program out of retirement.
    Shader * program = (Shader*) r->vitalAssets->get((char*)"bg_tile_shader");
    
    // Oh also get its texture.
    Texture * tex = (Texture*) r->getAssetManager()->get(this->texture);
    
    // Use the shader program we pulled out the AssetManager.
    glUseProgram(program->getID());
    
    // Alright, now we've got to do some hinky stuff with the matrix.
    // Originally the first two entries of the last column are the
    // position of the Tile: Pt. We need to make them (Pt+Ct) * (parallax factor)
    
    // Store the original values so we can put them back.
    GLfloat x = this->getX();
    GLfloat y = this->getY();
    
    // Get the parallax factor.
    float Fp = this->getParallaxFactor(this->getPlane());
    
	// Now we set up the matrix. There's documentation on how this works.
    BasicMatrix *pm = this->getPositionMat();
    if( this->ignoresScroll() )
    {
		pm->set(0,2, x );
		pm->set(1,2, y );
	}
    else
    {
        Camera * c = r->getCamera();
		pm->set(0,2, ( x - c->getX() )*Fp - c->getOffX()*(1.0-Fp) );
		pm->set(1,2, ( y - c->getY() )*Fp - c->getOffY()*(1.0-Fp) );
	}
    
    // Alrighty! Now that that's done, we can feed the matrix to the shader.
    float * lm = this->getCompoundMat()->getLinear();
    program->setUniform("transform", &lm);
    
    // Reset the BasicMatrix.
    this->setX(x);
    this->setY(y);
    
    // Feed this tile's depth information to the shader.
    float depth = Tile::getTileDepth(this->getPlane());
    program->setUniform("depth", &depth);

    // Now we set up this texture.
    program->setTextureUniform("texture", tex->getID(), 0); 
    
    // Let's not forget about texture flip!
    GLuint hFlip = (GLuint)(this->getTextureFlip() & Tile::FLIP_HORIZ);
    GLuint vFlip = (GLuint)(this->getTextureFlip() & Tile::FLIP_VERT);
    program->setUniform("hFlip", &hFlip);
    program->setUniform("vFlip", &vFlip);
    
    // Draw the vertex arrays. We want the primitives drawn to be
    // triangles, and to start at the 0th vertex, and to draw a
    // total of 6 vertices.
    // Sure, this is the same array used for each Tile, but the
    // shaders have access to the position and size by means
    // of a passed-in matrix.
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

char * BGTile::getTexture()
{
    return this->texture;
}

void BGTile::report()
{
    std::cout << "BGTile:\t\t" << this->getID()  
              << " trans: " << this->hasTrans() 
              << " plane: " << this->getPlane()
              << " tex: " << this->texture << std::endl;
}
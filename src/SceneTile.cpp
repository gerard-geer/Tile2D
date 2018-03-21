#include "SceneTile.h"

SceneTile::SceneTile()
: Tile()
{
}

SceneTile::~SceneTile()
{
}

void SceneTile::init(GLfloat x, GLfloat y, tile_plane plane, GLfloat width, GLfloat height, bool trans, char * texture)
{
    Tile::init(x, y, plane, width, height, trans);
    this->texture = texture;
}

void SceneTile::render(Renderer * r)
{
    Shader * program = (Shader*) r->vitalAssets->get("scene_tile_shader");

    Texture * tex = (Texture*) r->getAssetManager()->get(this->texture);
    
    program->use();
    
    // Store the original values so we can put them back.
    GLfloat x = this->getX();
    GLfloat y = this->getY();
    
    // Get the parallax factor.
    float Fp = this->getParallaxFactor(this->getPlane());
    
	// Now we set up the matrix. There's documentation on how this works.
    BasicMatrix * pm = this->getPositionMat();

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

char * SceneTile::getTexture()
{
    return this->texture;
}

void SceneTile::report()
{
    std::cout << "SceneTile:\t" << this->getID()  
              << " trans: " << this->hasTrans() 
              << " plane: " << this->getPlane()
              << " tex: " << this->texture << std::endl;
}
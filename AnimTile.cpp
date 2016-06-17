#include "AnimTile.h"

AnimTile::AnimTile()
{
}

AnimTile::~AnimTile()
{
}

void AnimTile::init(GLfloat x, GLfloat y, tile_plane plane, GLfloat width, 
                    GLfloat height, bool trans, char* texture, unsigned int numFrames,
                    unsigned int frameWidth, unsigned int frameHeight, 
                    float frameTime)
{
    // Initialize the parent Tile.
    Tile::init(x, y, plane, width, height, trans);
    
    // Initialize all the other stuff.
    this->curFrame = 0;
    this->numFrames = numFrames;
    this->frameWidth = frameWidth;
    this->frameHeight = frameHeight;
    this->frameTime = frameTime;
    this->lastChange = glfwGetTime();
    this->texture = texture;
}

void AnimTile::render(Renderer* r)
{
        
    // First things first: Let's make sure we're drawing the correct frame.
    if( glfwGetTime() > this->lastChange+this->frameTime )
    {
        this->lastChange = glfwGetTime();
        this->curFrame = (this->curFrame + 1)%this->numFrames;
    }
    
    // Now let's get some stuff from the asset Manager.
    Shader * program = (Shader*)r->getAssetManager()->get("anim_tile_shader");
    Texture * frames = (Texture*)r->getAssetManager()->get(texture);
    
    // Start using the program.
    glUseProgram(program->getID());
    
    // Send the fractional dimensions of each frame.
    GLfloat * fractFrameDim = (GLfloat*) malloc(sizeof(GLfloat)*2);
    fractFrameDim[0] = ((GLfloat)this->frameWidth/(GLfloat)frames->getWidth());
    fractFrameDim[1] = ((GLfloat)this->frameHeight/(GLfloat)frames->getHeight());
    program->setUniform("fractFrameDim", &fractFrameDim);
    free(fractFrameDim);
    
    // Send the current frame index.
    program->setUniform("curFrame", &this->curFrame);

    // Now let's get the parallax factor.
    float Fp = Tile::getParallaxFactor(this->getPlane());
    
    // Now habitually preserve the current X and Y position of the AnimTile.
    GLfloat x = this->getX();
    GLfloat y = this->getY();
    
    // Set up the transformation matrix as usual.
    //std::cout << r->getCamera()->getX() << std::endl;
    //std::cout << "Made it here" << std::endl;
    this->getMatrix()->set(0,2, ( this->getX() - r->getCamera()->getX() )*Fp );
    this->getMatrix()->set(1,2, ( this->getY() - r->getCamera()->getY() )*Fp );
    
    // Hand it over to the GPU.
    float * lm = this->getMatrix()->getLinear();
    program->setUniform((char*)"transform", &lm);
    
    // Reset the BasicMatrix.
    this->setX(x);
    this->setY(y);
    
    // Feed this tile's depth information to the shader.
    float depth = Tile::getTileDepth(this->getPlane());
    program->setUniform((char*)"depth", &depth);
    
    // Now we set up this texture.
    program->setTextureUniform((char*)"texture", frames->getID(), 0);   
    
    // Let's not forget about texture flip!
    GLuint hFlip = (GLuint)(this->getTextureFlip() & Tile::FLIP_HORIZ);
    GLuint vFlip = (GLuint)(this->getTextureFlip() & Tile::FLIP_VERT);
    std::cout << "Vflip? " << this->getTextureFlip() << std::endl;
    program->setUniform((char*)"hFlip", &hFlip);
    program->setUniform((char*)"vFlip", &vFlip);
    
    
    // Draw the vertex arrays. We want the primitives drawn to be
    // triangles, and to start at the 0th vertex, and to draw a
    // total of 6 vertices.
    // Sure, this is the same array used for each Tile, but the
    // shaders have access to the position and size by means
    // of a passed-in matrix.
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

float AnimTile::getFrameTime() const 
{
    return this->frameTime;
}

unsigned int AnimTile::getCurFrame() const 
{
    return this->curFrame;
}

unsigned int AnimTile::getFrameHeight() const 
{
    return this->frameHeight;
}

unsigned int AnimTile::getFrameWidth() const 
{
    return this->frameWidth;
}

unsigned int AnimTile::getNumFrames() const 
{
    return this->numFrames;
}

char* AnimTile::getTexture() 
{
    return this->texture;
}

void AnimTile::setFrameTime(float frameTime) 
{
    this->frameTime = frameTime;
}

void AnimTile::setCurFrame(unsigned int curFrame) 
{
    this->curFrame = curFrame;
}

void AnimTile::setNumFrames(unsigned int numFrames) 
{
    this->numFrames = numFrames;
}
#include "FwdTile.h"

FwdTile::FwdTile()
{
}

FwdTile::~FwdTile()
{
}

void FwdTile::init(GLfloat x, GLfloat y, tile_plane plane, GLfloat width, 
                    GLfloat height, char * texA, char * texB,
                    char * texC, char * texD, char * shader)
{
    Tile::init(x, y, plane, width, height, true);
    this->texA = (char*)( (texA == NULL)?"EMPTY_TEX":texA );
    this->texB = (char*)( (texB == NULL)?"EMPTY_TEX":texB );
    this->texC = (char*)( (texC == NULL)?"EMPTY_TEX":texC );
    this->texD = (char*)( (texD == NULL)?"EMPTY_TEX":texD );
    this->shader = shader;
}

void FwdTile::setShader(char* shader)
{
    this->shader = shader;
}
void FwdTile::setTexA(char* texA)
{
    this->texA = texA;
}
void FwdTile::setTexB(char* texB)
{
    this->texB = texB;
}
void FwdTile::setTexC(char* texC)
{
    this->texC = texC;
}
void FwdTile::setTexD(char* texD)
{
    this->texD = texD;
}
char * FwdTile::getShader()
{
    return shader;
}
char * FwdTile::getTexA()
{
    return texA;
}

char * FwdTile::getTexB()
{
    return texB;
}
char * FwdTile::getTexC()
{
    return texC;
}

char * FwdTile::getTexD()
{
    return texD;
}

void FwdTile::render(Renderer * r)
{
	// Get all the stuff we need out of the AssetManager.
    Shader * program = (Shader*)r->getAssetManager()->get(this->shader);
    Texture * a = (Texture*)r->getAssetManager()->get(this->texA);
    Texture * b = (Texture*)r->getAssetManager()->get(this->texB);
    Texture * c = (Texture*)r->getAssetManager()->get(this->texC);
    Texture * d = (Texture*)r->getAssetManager()->get(this->texD);
    
    // Start using this FwdTile's shader.
    glUseProgram(program->getID());
    
    // Set the texture unit uniforms.
    program->setTextureUniform("texA", a->getID(), 0);
    program->setTextureUniform("texB", b->getID(), 1);
    program->setTextureUniform("texC", c->getID(), 2);
    program->setTextureUniform("texD", d->getID(), 3);
    
    // Send in the resolution.
    FwdTile::resolution[0] = (GLfloat)(r->getDefPass()->getWidth());
    FwdTile::resolution[1] = (GLfloat)(r->getDefPass()->getHeight());
    program->setUniform("resolution", &(FwdTile::resolution));
    
    // Send in the camera position.
    FwdTile::camPosition[0] = (GLfloat)(r->getCamera()->getX());
    FwdTile::camPosition[1] = (GLfloat)(r->getCamera()->getY());
    program->setUniform("camera", &(FwdTile::camPosition));
    
    // Since FwdTiles do the parallax effect entirely in the vertex shader,
    // we can send them a virgin matrix.
    float * lm = this->getCompoundMat()->getLinear();
    program->setUniform("transform", &lm);
    
    // Let's not forget the time.
    float time = glfwGetTime();
    program->setUniform("time", &time);
    
    // Get the parallax factor and send it in.
    float Fp = this->getParallaxFactor(this->getPlane());
    program->setUniform("pFactor", &Fp);
    
    // Give the shader the ignoreScroll value.
    float is = (float) this->ignoresScroll();
    program->setUniform("ignoreScroll", &is);
    
    // Send in the depth of this Tile as well.
    float depth = Tile::getTileDepth(this->getPlane());
    program->setUniform("depth", &depth);
    
    // Let's not forget about texture flip!
    GLuint hFlip = (GLuint)(this->getTextureFlip() & Tile::FLIP_HORIZ);
    GLuint vFlip = (GLuint)(this->getTextureFlip() & Tile::FLIP_VERT);
    program->setUniform("hFlip", &hFlip);
    program->setUniform("vFlip", &vFlip);
    
    // The usual call to glDrawArrays()
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void FwdTile::report()
{
    std::cout << "FwdTile:\t" << this->getID()  
              << " trans: " << this->hasTrans() 
              << " plane: " << this->getPlane()
              << " texA: " << this->texA 
              << " texB: " << this->texB
              << " texC: " << this->texC 
              << " texD: " << this->texD << std::endl;
}

// IT'S GONE IN THE NAME OF not malloc'ing in the innermost function call.
GLfloat * FwdTile::resolution = (GLfloat*) malloc(sizeof(GLfloat)*2);
GLfloat * FwdTile::camPosition = (GLfloat*) malloc(sizeof(GLfloat)*2);
GLfloat * FwdTile::pOffset = (GLfloat*) malloc(sizeof(GLfloat)*2);
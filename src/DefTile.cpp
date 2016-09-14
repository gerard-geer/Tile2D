#include "DefTile.h"

DefTile::DefTile()
{
}

DefTile::~DefTile()
{
}

void DefTile::init(GLfloat x, GLfloat y, tile_plane plane, GLfloat width, 
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

void DefTile::setShader(char* shader)
{
    this->shader = shader;
}
void DefTile::setTexA(char* texA)
{
    this->texA = texA;
}
void DefTile::setTexB(char* texB)
{
    this->texB = texB;
}
void DefTile::setTexC(char* texC)
{
    this->texC = texC;
}
void DefTile::setTexD(char* texD)
{
    this->texD = texD;
}
char * DefTile::getShader()
{
    return shader;
}
char * DefTile::getTexA()
{
    return texA;
}

char * DefTile::getTexB()
{
    return texB;
}
char * DefTile::getTexC()
{
    return texC;
}

char * DefTile::getTexD()
{
    return texD;
}

void DefTile::render(Renderer * r)
{
	// Get all the stuff we need out of the AssetManager.
    Shader * program = (Shader*)r->getAssetManager()->get(this->shader);
    Texture * a = (Texture*)r->getAssetManager()->get(this->texA);
    Texture * b = (Texture*)r->getAssetManager()->get(this->texB);
    Texture * c = (Texture*)r->getAssetManager()->get(this->texC);
    Texture * d = (Texture*)r->getAssetManager()->get(this->texD);
    
    // Start using this DefTile's shader.
    glUseProgram(program->getID());
    
    // Set the texture unit uniforms.
    program->setTextureUniform("texA", a->getID(), 0);
    program->setTextureUniform("texB", b->getID(), 1);
    program->setTextureUniform("texC", c->getID(), 2);
    program->setTextureUniform("texD", d->getID(), 3);
    
    // Be sure to supply the fwd pass.
    program->setTextureUniform("fwdColor", r->getFwdPass()->getRenderTexture(), 4);
    program->setTextureUniform("fwdDepth", r->getFwdPass()->getDepthTexture(), 5);
    
    // Send in the resolution.
    GLfloat * resolution = (GLfloat*) malloc(sizeof(GLfloat)*2);
    resolution[0] = (GLfloat)(r->getDefPass()->getWidth());
    resolution[1] = (GLfloat)(r->getDefPass()->getHeight());
    program->setUniform("resolution", &resolution);
    free(resolution);
    // Send in the camera position.
    GLfloat * camPosition = (GLfloat*) malloc(sizeof(GLfloat)*2);
    camPosition[0] = (GLfloat)(r->getCamera()->getX());
    camPosition[1] = (GLfloat)(r->getCamera()->getY());
    program->setUniform("camera", &camPosition);
    free(camPosition);
    
    // Since DefTiles do the parallax effect entirely in the vertex shader,
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

void DefTile::report()
{
    std::cout << "DefTile:\t" << this->getID()  
              << " trans: " << this->hasTrans() 
              << " plane: " << this->getPlane()
              << " texA: " << this->texA 
              << " texB: " << this->texB
              << " texC: " << this->texC 
              << " texD: " << this->texD << std::endl;
}
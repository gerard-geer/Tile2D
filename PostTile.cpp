#include "PostTile.h"

PostTile::PostTile()
{
}

PostTile::~PostTile()
{
}

void PostTile::init(GLfloat x, GLfloat y, tile_plane plane, GLfloat width, 
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

Shader * PostTile::createPostTileShader(char * vertPath, char * fragPath)
{
    Shader * s = new Shader();
    shader_error e = s->load(vertPath, fragPath);
    s->addUniform((char*)"transform", UNI_MAT3);
    s->addUniform((char*)"depth", UNI_FLOAT);
    s->addUniform((char*)"camera", UNI_VEC2);
    s->addUniform((char*)"time", UNI_FLOAT);
    s->addUniform((char*)"resolution", UNI_VEC2);
    s->addUniform((char*)"fwdColor", UNI_TEX);
    s->addUniform((char*)"fwdDepth", UNI_TEX);
    s->addUniform((char*)"texA", UNI_TEX);
    s->addUniform((char*)"texB", UNI_TEX);
    s->addUniform((char*)"texC", UNI_TEX);
    s->addUniform((char*)"texD", UNI_TEX);
    return s;
}

void PostTile::setShader(char* shader)
{
    this->shader = shader;
}
void PostTile::setTexA(char* texA)
{
    this->texA = texA;
}
void PostTile::setTexB(char* texB)
{
    this->texB = texB;
}
void PostTile::setTexC(char* texC)
{
    this->texC = texC;
}
void PostTile::setTexD(char* texD)
{
    this->texD = texD;
}
char * PostTile::getShader()
{
    return shader;
}
char * PostTile::getTexA()
{
    return texA;
}
char * PostTile::getTexB()
{
    return texB;
}
char * PostTile::getTexC()
{
    return texC;
}
char * PostTile::getTexD()
{
    return texD;
}

void PostTile::render(Renderer * r)
{
    Shader * program = (Shader*)r->getAssetManager()->get(this->shader);
    Texture * a = (Texture*)r->getAssetManager()->get(this->texA);
    Texture * b = (Texture*)r->getAssetManager()->get(this->texB);
    Texture * c = (Texture*)r->getAssetManager()->get(this->texC);
    Texture * d = (Texture*)r->getAssetManager()->get(this->texD);
    
    // Start using this PostTile's shader.
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
    
    // Since PostTiles do the parallax effect entirely in the vertex shader,
    // we can send them a virgin matrix.
    float * lm = this->getMatrix()->getLinear();
    program->setUniform("transform", &lm);
    
    // Let's not forget the time.
    float time = glfwGetTime();
    program->setUniform("time", &time);
    
    // Get the parallax factor and send it in as "depth."
    float Fp = this->getParallaxFactor(this->getPlane());
    program->setUniform((char*)"depth", &Fp);
    
    // The usual call to glDrawArrays()
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
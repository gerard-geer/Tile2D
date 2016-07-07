#include "Renderer.h"
#include <algorithm>

Renderer::Renderer()
{
    this->tileVertVBO = 0;
    this->tileUvVBO = 0;
    this->defFB = NULL;
    this->fwdFB = NULL;
}

Renderer::~Renderer()
{
}

void Renderer::initPlaceholderTexture()
{
    Texture * placeholder = new Texture();
    placeholder->createEmpty();
    this->assets->addNewTexture((char*)"EMPTY_TEX", NULL);
}

void Renderer::initStockShaders()
{
    this->assets->addNewShader("bg_tile_shader", 
                               "../Assets/Rendering Assets/Shaders/bg_tile_shader.vert",
                               "../Assets/Rendering Assets/Shaders/bg_tile_shader.frag");
    this->assets->addNewShader("scene_tile_shader",
                               "../Assets/Rendering Assets/Shaders/scene_tile_shader.vert",
                               "../Assets/Rendering Assets/Shaders/scene_tile_shader.frag");
    this->assets->addNewShader("anim_tile_shader", 
                               "../Assets/Rendering Assets/Shaders/anim_tile_shader.vert",
                               "../Assets/Rendering Assets/Shaders/anim_tile_shader.frag");
    this->assets->addNewShader("final_pass_shader",
                               "../Assets/Rendering Assets/Shaders/final_pass_shader.vert",
                               "../Assets/Rendering Assets/Shaders/final_pass_shader.frag");    
}

void Renderer::initTileVAO()
{
    // Now we get to create a VBO!
    // First we need some values.
    GLfloat tileVerts[18] = {
        -0.5f, -0.5f, 1.0f,    0.5f,  0.5f, 1.0f,
         0.5f, -0.5f, 1.0f,   -0.5f, -0.5f, 1.0f,
        -0.5f,  0.5f, 1.0f,    0.5f,  0.5f, 1.0f
    };

    GLfloat tileUVs[12] = {
        0.0f,  1.0f,    1.0f,  0.0f,
        1.0f,  1.0f,    0.0f,  1.0f,
        0.0f,  0.0f,    1.0f,  0.0f
    };
        
    // Now we generate the buffers.
    glGenBuffers( 1, &(this->tileVertVBO) );
    glGenBuffers( 1, &(this->tileUvVBO) );
    
    // Bind to the vertex positions buffer.
    glBindBuffer(GL_ARRAY_BUFFER, this->tileVertVBO);
    // Feed it the vertex positions data.
    glBufferData(GL_ARRAY_BUFFER, 18*sizeof(float), tileVerts, GL_STATIC_DRAW);
    
    // Do the same for the UV buffer.
    glBindBuffer(GL_ARRAY_BUFFER, this->tileUvVBO);
    glBufferData(GL_ARRAY_BUFFER, 12*sizeof(float), tileUVs, GL_STATIC_DRAW);
    
    // Now that we've created the VBOs, we can create the VAO and tie them together.
    glGenVertexArrays(1, &(this->tileVAO));
    glBindVertexArray(this->tileVAO);
    
    // Attach the vertex VBO to the VAO.
    glBindBuffer(GL_ARRAY_BUFFER, this->tileVertVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    
    // Attach the texture coordinate VBO as well.
    glBindBuffer(GL_ARRAY_BUFFER, this->tileUvVBO);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    
    // Tell the VAO which indices we're actually using.
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
}

bool Renderer::init(GLuint width, GLuint height)
{
    // First initialize the Asset Manager.
    this->assets = new AssetManager();
    
    // Next initialize the Camera.
    this->camera = new Camera(0.0, 0.0);
        
    // Then initialize the Tiles' VAO.
    initTileVAO();
    
    // Oh go ahead and initialize the final pass Tile.
    this->finalPass = new SceneTile();
    this->finalPass->init(0,0,PLANE_PLAYFIELD_B,2,2,false,(char*)"using our own");
    
    // Now that that's done, we load the stock shaders for the tiles
    // that use them.
    initStockShaders();
    
    // The placeholder texture is mighty important.
    initPlaceholderTexture();
    
    // Oh and also we need to init the framebuffers.
    bool success;
    this->fwdFB = new Framebuffer();
    success = this->fwdFB->init(width, height);
    if(!success) this->destroy();
    this->defFB = new Framebuffer();
    success = this->defFB->init(width, height);
    if(!success) this->destroy();
    return success;
    
}

bool Renderer::resize(GLuint width, GLuint height)
{
    this->defFB->resize(width, height);
    this->fwdFB->resize(width, height);
    return true;
}

AssetManager * Renderer::getAssetManager()
{
    return this->assets;
}

Camera * Renderer::getCamera()
{
    return this->camera;
}

GLuint Renderer::getTileVAO()
{
    return this->tileVAO;
}

Framebuffer * Renderer::getFwdPass()
{
    return this->fwdFB;
}

Framebuffer * Renderer::getDefPass()
{
    return this->defFB;
}

unsigned int Renderer::getWidth()
{
    return this->fwdFB->getWidth();
}

unsigned int Renderer::getHeight()
{
    return this->fwdFB->getHeight();
}

void Renderer::memoize()
{
    unsigned int i = 0;
    for(std::vector<TileWithType>::iterator it = renderQueue.begin(); it != renderQueue.end(); ++it)
    {
        /* it->first: TileType
         * it->second: Tile* */
        this->rqMemo.insert(IdAndIndex(it->second->getID(), i));
        ++i;
    }
}

/**
 * @brief This is used as the predicate when sorting Tiles. It places opaque
 *        from front to back, then transparent Tiles from back to front.
 * @param lhs The left-hand-side Tile in the comparision.
 * @param rhs The right-hand-size Tile in the comparison.
 * @return Whether or not the first one is less than the second.
 */
bool tileSortingPredicate(TileWithType lhs, TileWithType rhs)
{
    Tile * a = lhs.second;
    Tile * b = rhs.second;
    // We need to render transparent objects last.
    if(  a->hasTrans() && !b->hasTrans() ) return false;
    if( !a->hasTrans() &&  b->hasTrans() ) return true;
    
    // At this point both operands will have the same transparency status.
    
    // Now we're comparing rendering planes. We want to
    // draw the closest Tiles first, so we can take
    // advantage of depth testing and thusly minimize redraw.
    if( a->getPlane() <= b->getPlane() ) return true;
    if( a->getPlane() >  b->getPlane() ) return false;
    return true; // Just to get rid of the warnings.
}

void Renderer::addToRenderQueue(tile_type type, Tile * tile)
{
    // Just tack the Tile on the end since it'll be sorted.
    this->renderQueue.push_back(TileWithType(type,tile));
    
    // Sort the Tiles to cut down on overdraw.
    std::sort(this->renderQueue.begin(), this->renderQueue.end(), tileSortingPredicate);
    
    // Now that it's sorted, we need to re-memoize.
    this->memoize();
}

bool Renderer::removeFromRenderQueue(Tile* tile)
{
    // Check to make sure the Tile is in the memoization.
    if ( this->rqMemo.find(tile->getID()) != this->rqMemo.end() )
    {
        // If it is we get the index of the item from the memoization.
        unsigned int index = this->rqMemo[tile->getID()];
        
        // Use that index to erase.
        renderQueue.erase(renderQueue.begin()+index);
        
        // Also erase the memoization entry.
        rqMemo.erase(tile->getID());
        
        this->memoize();
        
        // Oh hey we did it! Return true as a reward.
        return true;
    }
    
    // Didn't exist in the queue! Return false to say so.
    return false;
    
}

void Renderer::flushRenderQueue()
{
    this->renderQueue.clear();
}

bool Renderer::onScreenTest(Tile * t)
{
    // First we check if the distance between the center of the screen and the
    // center of the Tile is greater than 1 + the width of the Tile. If so the
    // Tile is not on screen.
    if( std::abs( this->camera->getX() - t->getX() ) > 1.0 + t->getWidth()*.5 ) return false;
    // We do the same in the vertical axis.
    if( std::abs( this->camera->getY() - t->getY() ) > 1.0 + t->getHeight()*.5 ) return false;
    return true;
}

void Renderer::renderFinalPass()
{
    // Get the shader that we need for the final pass' screen quad.
    Shader * program = (Shader*) this->assets->get("final_pass_shader");
    
    // Tell OpenGL to use that program.
    program->use();
    
    // Assign the forward and deferred passes to a texture unit, then pass
    // that unit to the shader program.
    program->setTextureUniform("fwdFB", this->fwdFB->getRenderTexture(), 0);
    program->setTextureUniform("defFB", this->defFB->getRenderTexture(), 1);
    
    // Now we just draw the triangles.
    glDrawArrays(GL_TRIANGLES, 0, 6); 
}

void Renderer::render(Window * window)
{
    
    // Bind to the VAO.
    glBindVertexArray( this->tileVAO );
    
    // Enable these just in case.
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    
    // Start drawing to the forward framebuffer.
    this->fwdFB->setAsRenderTarget();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear out what was in the framebuffer.
    
    // Now that we've sorted things, we go through and render the
    // non-post-process Tiles.
    for(std::vector<TileWithType>::iterator it = renderQueue.begin(); it != renderQueue.end(); ++it)
    {
        // it->first is the tile_type;
        // it->second is the Tile*.
        
        // If this is a PostTile, then we go ahead and save it for later.
        if( it->first == POST_TILE ) continue;
        
        // Also if it's offscreen we don't need to render it.
        if( !this->onScreenTest(it->second) ) continue;
        
        // Otherwise we render the tile.
        it->second->render(this);
    }
    
    // Now for the PostTiles we flip to the second framebuffer.
    this->defFB->setAsRenderTarget();
    //glClearColor(1.0f, 0.0f, 0.0f, 0.0f); // Set the clear color to have an alpha of zero
                                          // so that any pixel not rendered to has an easily
                                          // identifiable attribute. This makes mixing the
                                          // two framebuffers much easier later on.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Flush again.
    // Now that the primary-pass Tiles have been rendered...
    for(std::vector<TileWithType>::iterator it = renderQueue.begin(); it != renderQueue.end(); ++it)
    {
        // Now we only render PostTiles.
        if( it->first != POST_TILE ) continue;
        
        // Again, if it's offscreen we don't need to render it.
        if( !this->onScreenTest(it->second) ) continue;
        
        // Render the PostTile.
        it->second->render(this);
    }
    
    // Now we go back to renderering to the window.
    window->setAsRenderTarget();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Jiggle the handle.
    this->renderFinalPass(); // Draws a full screen quad with the two FBOs mixed.
}

BGTile * Renderer::makeBGTile(GLfloat x, GLfloat y, GLfloat width, GLfloat height, 
                              bool normalize, char* texture)
{
    BGTile * t = new BGTile();
    if(normalize)
    {
        width /= this->getWidth()*.5;
        height /= this->getHeight()*.5;
        x = (x/(this->getWidth()*.5))-1.0+width*.5;
        y = (y/(this->getHeight()*.5))-1.0+height*.5;
    }
    t->init(x, y, width, height, texture);
    return t;
}

BGTile * Renderer::makeBGTile(GLfloat x, GLfloat y, GLfloat width, GLfloat height, 
                              bool normalize, const char* texture)
{
	return this->makeBGTile(x,y,width,height,normalize,(char*)texture);
}

SceneTile * Renderer::makeSceneTile(GLfloat x, GLfloat y, tile_plane plane, GLfloat width, 
                                  GLfloat height, bool normalize, char* texture)
{
    SceneTile * t = new SceneTile();
    if(normalize)
    {
        width /= this->getWidth()*.5;
        height /= this->getHeight()*.5;
        x = (x/(this->getWidth()*.5))-1.0+width*.5;
        y = (y/(this->getHeight()*.5))-1.0+height*.5;
    }
    t->init(x, y, plane, width, height, 
            // Oh hey look since we're part of Renderer, we can just go see if
            // the texture has an alpha channel.
            ((Texture*)(this->getAssetManager()->get(texture)))->hasAlpha(),
            texture);
    return t;
}

SceneTile * Renderer::makeSceneTile(GLfloat x, GLfloat y, tile_plane plane, GLfloat width, 
                                  GLfloat height, bool normalize, const char* texture)
{
	return this->makeSceneTile(x,y,plane,width,height,normalize,(char*)texture);
}

AnimTile * Renderer::makeAnimTile(GLfloat x, GLfloat y, tile_plane plane, GLfloat width,
                                GLfloat height, bool normalize, char* texture, 
                                unsigned int numFrames, unsigned int frameWidth, 
                                unsigned int frameHeight, float frameTime)
{
    AnimTile * t = new AnimTile();
    if(normalize)
    {
        width /= this->getWidth()*.5;
        height /= this->getHeight()*.5;
        x = (x/(this->getWidth()*.5))-1.0+width*.5;
        y = (y/(this->getHeight()*.5))-1.0+height*.5;
    }
    t->init(x, y, plane, width, height, 
            ((Texture*)(this->getAssetManager()->get(texture)))->hasAlpha(),
            texture, numFrames, frameWidth, frameHeight, frameTime);
    return t;
}

AnimTile * Renderer::makeAnimTile(GLfloat x, GLfloat y, tile_plane plane, GLfloat width,
                                GLfloat height, bool normalize, const char* texture, 
                                unsigned int numFrames, unsigned int frameWidth, 
                                unsigned int frameHeight, float frameTime)
{
	return this->makeAnimTile(x,y,plane,width,height,normalize,(char*)texture,numFrames,
							  frameWidth,frameHeight,frameTime);
}

PostTile * Renderer::makePostTile(GLfloat x, GLfloat y, tile_plane plane, GLfloat width,
                                GLfloat height, bool normalize, char* texA, char* texB,
                                char* texC, char* texD, char* shader)
{
    PostTile * t = new PostTile();
    if(normalize)
    {
        width /= this->getWidth()*.5;
        height /= this->getHeight()*.5;
        x = (x/(this->getWidth()*.5))-1.0+width*.5;
        y = (y/(this->getHeight()*.5))-1.0+height*.5;
    }
    t->init(x, y, plane, width, height, texA, texB, texC, texD, shader);
    return t;
}

PostTile * Renderer::makePostTile(GLfloat x, GLfloat y, tile_plane plane, GLfloat width,
								  GLfloat height, bool normalize, const char * texA, 
								  const char * texB, const char * texC, const char * texD,
								  const char * shader)
{
	return this->makePostTile(x,y,plane,width,height,normalize,(char*)texA,(char*)texB,
							  (char*)texC, (char*)texD, (char*)shader);
}

GLfloat Renderer::getTilePxX(Tile * tile)
{
    return ( tile->getX()+1.0 ) * (this->getWidth() * .5);
}

GLfloat Renderer::getTilePxY(Tile * tile)
{
    return ( tile->getY()+1.0 ) * (this->getHeight() * .5);
}

void Renderer::setTilePxPos(Tile * tile, GLfloat x, GLfloat y)
{
    x = (x/(this->getWidth()*.5))-1.0+tile->getWidth()*.5;
    y = (y/(this->getHeight()*.5))-1.0+tile->getHeight()*.5;
    tile->setX(x);
    tile->setY(y);
}

GLfloat Renderer::getTilePxW(Tile * tile)
{
    return ( tile->getWidth() ) * (this->getWidth() * .5);
}

GLfloat Renderer::getTilePxH(Tile * tile)
{
    return ( tile->getHeight() ) * (this->getHeight() * .5);
}

void Renderer::setTilePxDim(Tile * tile, GLfloat w, GLfloat h)
{
    w /= this->getWidth()*.5;
    h /= this->getHeight()*.5;
    tile->setWidth(w);
    tile->setHeight(h);
}
void Renderer::destroyTileVAO()
{
    glDeleteBuffers(1, &this->tileVertVBO);
    glDeleteBuffers(1, &this->tileUvVBO);
    glDeleteVertexArrays(1, &this->tileVAO);
}

void Renderer::destroyFBOs()
{
    this->fwdFB->destroy();
    this->defFB->destroy();
    delete this->fwdFB;
    delete this->defFB;
}

void Renderer::destroyAssetManager()
{
    this->assets->clear();
    delete this->assets;
}

void Renderer::destroy()
{
    this->destroyAssetManager();
    this->destroyTileVAO();
    this->destroyFBOs();
}

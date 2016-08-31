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
    this->vitalAssets->addNewShaderStrings("bg_tile_shader", 
                               bg_tile_shader_vert,
                               bg_tile_shader_frag);
    this->vitalAssets->addNewShaderStrings("scene_tile_shader",
                               scene_tile_shader_vert,
                               scene_tile_shader_frag);
    this->vitalAssets->addNewShaderStrings("anim_tile_shader", 
                               anim_tile_shader_vert,
                               anim_tile_shader_frag);
    this->vitalAssets->addNewShaderStrings("final_pass_shader",
                               final_pass_shader_vert,
                               final_pass_shader_frag);    
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
    // First initialize the AssetManagers.
    this->assets = new AssetManager();
    this->vitalAssets = new AssetManager();
    
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

void Renderer::setCustomShader(char * customCompositor)
{
	if( customCompositor == NULL )
	{
		this->customCompositor = customCompositor;
	}
	else if( this->assets->contains(customCompositor) )
	{
		this->customCompositor = customCompositor;
	}
	else this->customCompositor = NULL;	
}
void Renderer::setCustomShader(const char * customCompositor)
{
	this->setCustomShader((char*)customCompositor);
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
    
    // When Tiles don't have transparency, we can draw them from front to
    // back and take advantage of z-culling.
    if( !a->hasTrans() )
    {
        // Now we're comparing rendering planes. We want to
        // draw the closest Tiles first, so we can take
        // advantage of depth testing and thusly minimize redraw.
        if( a->getPlane() <= b->getPlane() ) return true;
        if( a->getPlane() >  b->getPlane() ) return false;
    }
    
    // Otherwise we need to draw from back to front in order for transparency
    // to work right.
    else
    {
        if( a->getPlane() <= b->getPlane() ) return false;
        if( a->getPlane() >  b->getPlane() ) return true;
    }
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
	// Get the Tile's projected screen coordinates.
    float tx, ty;
    float Fp = t->getParallaxFactor(t->getPlane());
    if( t->ignoresScroll() )
    {
    	tx = t->getX();
    	ty = t->getY();
    }
    else
    {
    	tx = ( t->getX() - this->getCamera()->getX() )*Fp;
    	ty = ( t->getY() - this->getCamera()->getY() )*Fp;
    }
    	
    // First we check if the distance between the center of the screen and the
    // center of the Tile is greater than 1 + the width of the Tile. If so the
    // Tile is not on screen.
    if( fabs(tx) > 1.0 + t->getWidth()*.5 ) return false;
    // We do the same in the vertical axis.
    if( fabs(ty) > 1.0 + t->getHeight()*.5 ) return false;
    
    return true;
}

void Renderer::renderFinalPass(Window * window)
{
    // Get the shader that we need for the final pass' screen quad. If the customCompositor key
	// is not NULL, and actually represents a value in the AssetManager then it is used for
	// composition. Otherwise the stock shader is used.
	Shader * program;
	if( this->customCompositor == NULL )
	{
		program = (Shader*) this->vitalAssets->get("final_pass_shader");
	}
	else if( ! this->assets->contains(customCompositor) )
	{
		program = (Shader*) this->vitalAssets->get("final_pass_shader");
		customCompositor = NULL;
	}
	else
	{
		program = (Shader*) this->assets->get(this->customCompositor);
	}
    
    // Tell OpenGL to use that program.
    program->use();
    
    // Assign the forward and deferred passes to a texture unit, then pass
    // that unit to the shader program.
    program->setTextureUniform("fwdFB", this->fwdFB->getRenderTexture(), 0);
    program->setTextureUniform("defFB", this->defFB->getRenderTexture(), 1);
	
	// If we're using a custom shader, we pass in extra stuff that's useful.
	if( this->customCompositor != NULL )
	{
		GLfloat * resolution = (GLfloat*) malloc(sizeof(GLfloat)*2);
		resolution[0] = (GLfloat)(this->getWidth());
		resolution[1] = (GLfloat)(this->getHeight());
		program->setUniform("resolution", &resolution);
		free(resolution);
		float time = glfwGetTime();
		program->setUniform("time", &time);
	}
    
    // Now we just draw the triangles.
    glDrawArrays(GL_TRIANGLES, 0, 6); 
}

void Renderer::render(Window * window)
{
    // Counters for how many Tiles were drawn and how many were culled, and
    // timer values for how long the entire frame, fwd pass, and deferred pass takes.
    #ifdef T2D_PER_FRAME_STATS
    int drawn = 0, culled = 0;
    float total, fwd, def;
    #endif
    
    // Print a header to delineate each frame.
    #ifdef T2D_PER_TILE_STATS
    std::cout << "START OF FRAME" << std::endl;
    #endif
    
    // Start timing the frame.
    #ifdef T2D_PER_FRAME_STATS
    total = glfwGetTime();
    #endif
    
    // Bind to the VAO.
    glBindVertexArray( this->tileVAO );    
    
    // Enable these just in case.
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    
    // Start timing the forward pass.
    #ifdef T2D_PER_FRAME_STATS
    fwd = glfwGetTime();
    #endif
    
    // Start drawing to the forward framebuffer.
    this->fwdFB->setAsRenderTarget();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear out what was in the framebuffer.
    
    // Now that we've sorted things, we go through and render the
    // non-post-process Tiles.
    for(std::vector<TileWithType>::iterator it = renderQueue.begin(); it != renderQueue.end(); ++it)
    {
        // it->first is the tile_type;
        // it->second is the Tile*.
        
        // If this is a DefTile, then we go ahead and save it for later.
        if( it->first == DEF_TILE ) continue;
        
        // Also if it's offscreen we don't need to render it.
        if( !this->onScreenTest(it->second) ) 
        {
    
            #ifdef T2D_PER_FRAME_STATS
            ++ culled;
            #endif
            continue;
        }
        
        // Otherwise we render the tile.
        it->second->render(this);
    
        // Print out the current tile if necessary.
        #ifdef T2D_PER_TILE_STATS
        it->second->report();
        #endif
        
        #ifdef T2D_PER_FRAME_STATS
        ++ drawn;
        #endif
    }
    
    // Clock the forward pass and
    // start timing the deferred pass.
    #ifdef T2D_PER_FRAME_STATS
    fwd = glfwGetTime()-fwd;
    def = glfwGetTime();
    #endif
    
    // Now for the DefTiles we flip to the second framebuffer.
    this->defFB->setAsRenderTarget();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Flush again.
    // Now that the primary-pass Tiles have been rendered...
    for(std::vector<TileWithType>::iterator it = renderQueue.begin(); it != renderQueue.end(); ++it)
    {
        // Now we only render DefTiles.
        if( it->first != DEF_TILE ) continue;
        
        // Again, if it's offscreen we don't need to render it.
        if( !this->onScreenTest(it->second) )
        {
    
            #ifdef T2D_PER_FRAME_STATS
            ++ culled;
            #endif
            continue;
        }
        
        // Render the DefTile.
        it->second->render(this);
    
        #ifdef T2D_PER_TILE_STATS
        it->second->report();
        #endif
        
        #ifdef T2D_PER_FRAME_STATS
        ++ drawn;
        #endif
    }
    
    // Clock the final pass.
    #ifdef T2D_PER_FRAME_STATS
    def = glfwGetTime()-def;
    #endif
    
    // Now we go back to renderering to the window.
    window->setAsRenderTarget();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Jiggle the handle.
    this->renderFinalPass(window); // Draws a full screen quad with the two FBOs mixed.
    
    // Clock the entire frame and actually print the stats to the screen.
    #ifdef T2D_PER_FRAME_STATS
    total = glfwGetTime()-total;
    std::cout << "Tiles drawn: " << drawn << "\tculled: " << culled << "\ttotal: " << drawn+culled << std::endl;
    std::cout << "Frame time: " << total << " (fwd: " << fwd << ") (def: " << def << ")" << std::endl;
    #endif
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

DefTile * Renderer::makeDefTile(GLfloat x, GLfloat y, tile_plane plane, GLfloat width,
                                GLfloat height, bool normalize, char* texA, char* texB,
                                char* texC, char* texD, char* shader)
{
    DefTile * t = new DefTile();
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

DefTile * Renderer::makeDefTile(GLfloat x, GLfloat y, tile_plane plane, GLfloat width,
								  GLfloat height, bool normalize, const char * texA, 
								  const char * texB, const char * texC, const char * texD,
								  const char * shader)
{
	return this->makeDefTile(x,y,plane,width,height,normalize,(char*)texA,(char*)texB,
							  (char*)texC, (char*)texD, (char*)shader);
}

FwdTile * Renderer::makeFwdTile(GLfloat x, GLfloat y, tile_plane plane, GLfloat width,
                                GLfloat height, bool normalize, char* texA, char* texB,
                                char* texC, char* texD, char* shader)
{
    FwdTile * t = new FwdTile();
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

FwdTile * Renderer::makeFwdTile(GLfloat x, GLfloat y, tile_plane plane, GLfloat width,
								  GLfloat height, bool normalize, const char * texA, 
								  const char * texB, const char * texC, const char * texD,
								  const char * shader)
{
	return this->makeFwdTile(x,y,plane,width,height,normalize,(char*)texA,(char*)texB,
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

void Renderer::destroyAssetManagers()
{
    this->assets->clear();
    this->vitalAssets->clear();
    delete this->assets;
    delete this->vitalAssets;
}

void Renderer::destroy()
{
    this->destroyAssetManagers();
    this->destroyTileVAO();
    this->destroyFBOs();
}


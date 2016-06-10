#ifndef RENDERER_H
#define RENDERER_H

#include <iostream>
#include <vector>
#include <valarray>
#include "AssetManager.h"
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include "Tile.h"
#include "SceneTile.h"
#include "Framebuffer.h"
#include "Window.h"

class Window;

typedef std::pair<tile_type, Tile*> TileWithType;
class Renderer
{
friend class Window;

private:

    /*
     * This Asset Manager is used for keeping the things Tiles
     * need to be drawn. This Renderer is passed to the Tiles'
     * render() function, and from there they will have access
     * to everything they need. Including the shared and only
     * transformed for each Tile geometry.
     */
    AssetManager * assets;
    
    /*
     * The handle to the vertex buffer object that stores the
     * vertex positions of the tile data.
     */
    GLuint tileVertVBO;
    
    /*
     * The handle to the vertex buffer object that stores the
     * vertex texture coordinates.
     */
    GLuint tileUvVBO;
    
    /*
     * The handle to the vertex array object that links the 
     * vertex buffer objects together.
     */
    GLuint tileVAO;
    
    /*
     * The Camera used for rendering.
     */
    Camera * camera;
    
    /*
     * The render queue. The way framebuilding works here is that
     * one adds Tiles to the renderQueue, and then calls render()
     * to render them all.
     */
    std::vector< std::pair<tile_type,Tile*> > renderQueue;
    
    /*
     * After the Tiles are drawn into the framebuffer, we need a
     * way to draw that framebuffer to the screen. How about atop
     * a SceneTile that has a different shader program thrown at it?
     */
    SceneTile * finalPass;
    
    /*
     * The forward pass Framebuffer. This is where all the good Tiles 
     * are drawn to so that they can be judged by PostTiles in the deferred pass.
     */
    Framebuffer * fwdFB;
    
    /*
     * The deferred pass Framebuffer. Alrightly, so it's not quite a "deferred"
     * rendering pass, but it does receive all the PostTiles.
     */
    Framebuffer * defFB;
    
    /**
     * @brief Adds a single, empty placeholder texture to the AssetManager for 
     *        use when PostTiles don't specify one or more of their textures.
     */
    void initPlaceholderTexture();
    /**
     * @brief Loads the stock shaders of the BGTile, SceneTile and AnimTile
     * and puts them in the AssetManager.
     */
    void initStockShaders();
    
    /**
     * @brief Initializes the Tiles' VAO. (They all share the same geometry.
     */
    void initTileVAO();  

    /**
     * @brief Draws the finished framebuffer onto a full screen Tile and
     *        renders it to the screen.
     */
    void renderFinalPass();
    
    /**
     * @brief Destroys the Tile VAO. This is useful for when context switching.
     */
    void destroyTileVAO();
    
    /**
     * @brief Destroys the forward and deferred framebuffers.
     */
    void destroyFBOs();
    
    
public:

    /**
     * @brief Constructs the renderer. Doesn't really do much. Most
     *        initialization is done in init().
     */
    Renderer();
    /**
     * @brief Doesn't do much. Use destroy() before deleting this
     *        object to prevent dangling pointers to the geometry
     *        and AssetManager.
     */
    ~Renderer();
    
    /**
     * @brief Initializes the renderer by creating the AssetManager
     *        and tile geometry, as well as the forward pass Framebuffer
     *        and the Tile the final pass is drawn onto.
     * @return Whether or not the Renderer was created successfully.
     *         If it wasn't, it was because the creation of the
     *         forward pass framebuffer failed. 
     */
    bool init(GLuint width, GLuint height);
    
    /**
     * @brief Resizes the forward pass Framebuffer.
     * @param width The new forward pass Framebuffer width.
     * @param height The new forward pass Framebuffer height.
     * @return Whether or not the the operation succeeded. The framebuffer
     *         has to be entirely recreated.
     */
    bool resize(GLuint width, GLuint height);
    
    /**
     * @brief Returns a handle to the Renderer's AsssetManager.
     * @return A handle to the Renderer's AssetManager.
     */
    AssetManager * getAssetManager();
    
    /**
     * @brief Returns a handle to the Renderer's Camera.
     * @return A handle to the Renderer's Camera.
     */
    Camera * getCamera();
    
    /**
     * @brief Returns the generalized Tile VAO.
     * @return The generalized Tile VAO.
     */
    GLuint getTileVAO();
    
    /**
     * @brief Returns a pointer to the Framebuffer that's used for the forward pass.
     * @return A pointer to the Framebuffer that's used for the forward pass.
     */
    Framebuffer * getFwdPass();
    
    /**
     * @brief Returns a pointer to the Framebuffer that's used for the deferred pass.
     * @return A pointer to the Framebuffer that's used for the deferred pass.
     */
    Framebuffer * getDefPass();
    
    /**
     * @brief Adds a Tile to the render queue. Whatever is in the render
     *        queue when Renderer::render() is called will be rendered.
     * @param tile The Tile to add to the render queue.
     */
    void addToRenderQueue(tile_type type, Tile * tile);
    
    /**
     * @brief Clears the rendering queue. Note that this doesn't destroy
     *        the Tiles within. It just simply clears out the line of Tiles
     *        waiting to get to be drawn.
     */
    void flushRenderQueue();
    
    /**
     * @brief Renders everything in the rendering queue.
     * @param window The Window instance being rendered to. This is needed
     *        to set the render target back to the window after framebuffered
     *        rendering, and to have glViewport() reset accordingly.
     */
    void render(Window * window);
    
    /**
     * @brief Destroys the Renderer, clearing its AssetManager and freeing
     *        the memory that stores the Tile geometry.
     */
    void destroy();
    
};

#endif // RENDERER_H

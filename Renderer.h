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
#include "BGTile.h"
#include "SceneTile.h"
#include "AnimTile.h"
#include "PostTile.h"
#include "Framebuffer.h"
#include "Window.h"

// All of these classes include Renderer.h, and so in that
// include, these classes aren't yet defined. Therefore we
// make a forward declaration here.
class Window;
class BGTile;
class SceneTile;
class AnimTile;
class PostTile;

/*
 * A type definition that links a Tile with the subclass it was cast from.
 */
typedef std::pair<tile_type, Tile*> TileWithType;

/**
 * @class Renderer
 * @author Gerard Geer
 * @date 06/13/16
 * @file Renderer.h
 * @brief This is the core Renderer. It renders any Tiles added to its render
 *        queue with respect to its encapsulated Camera instance. Any "assets",
 *        such as Textures or Shaders, that Tiles use must be added to the
 *        encapsulated AssetManager prior to drawing. Note that the Shaders for
 *        the BGTile, SceneTile, and AnimTile are loaded automatically by the
 *        renderer at startup. They must be in a directory 
 *        "../Assets/Rendering Assets/Shaders/" relative to the executable.
 *        The resolution of the underlying framebuffers and window are
 *        independent.
 * 
 *        How the pipeline works:
 *        -To begin, the renderer uses two framebuffer objects, so support of
 *         FBOs is essential.
 *        -First of all to draw a Tile you must add it to the render queue.
 *        -Second, the render queue is sorted with all opaque Tiles sorted from
 *         front to back, then all transparent Tiles are sorted from back to
 *         to front.
 *        -Next the BG-, Scene-, and AnimTiles are rendered into a first,
 *         forward framebuffer.
 *        -Then all of the PostTiles are rendered into a second, deferred
 *         framebuffer, with the depth and color buffers of the first pass made
 *         available to them. (As well as all of their custom textures, and
 *         using their custom shaders.)
 *        -PostTiles must do their own parallax calculations in their vertex
 *         Shader to allow for disabling them.
 *        -PostTiles must also do their own depth-test against the depth buffer
 *         of the first pass, also to make for being able to disable it.
 *        -If a PostTile fragment fails a depthtest, it needs to output an alpha
 *         of zero.
 *        -Finally the two buffers are stitched together and drawn onto a full
 *         screen quad. They're stitched together by mixing a texel from the 
 *         forward pass with the deferred pass based on the deferred pass texel's
 *         alpha.
 *        
 *        How Tile rendering works:
 *        -Each Tile subclass overrides a pure virtual method from Tile: 
 *         render(). This method is passed a pointer to the calling Renderer
 *         instance.
 *        -The Tile sets all the uniforms and constructs the parallax 
 *         transformation matrix of itself, then calls glDrawArrays() on the
 *         common VAO for every Tile stored within the Renderer.
 */
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
     * @brief Tests to see if a Tile is on screen for proactive culling.
     * @param t A pointer to the Tile to test.
     * @return Whether or not the Tile is on screen.
     */
    bool onScreenTest(Tile * t);
    
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
     * @brief Returns the horizontal resolution of the underlying FBOs.
     * @return The horizontal resolution of the underlying FBOs.
     */
    unsigned int getWidth();
    
    /**
     * @brief Returns the vertical resolution of the underlying FBOs.
     * @return The vertical resolution of the underlying FBOs.
     */
    unsigned int getHeight();
    
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
     * @brief A factory method that creates and initializes a BGTile. Make sure that
     *        this Tile's texture asset is loaded and stored in the AssetManager first.
     * @param x The X position of this BGTile.
     * @param y The Y position of this BGTile.
     * @param width The width of this BGTile.
     * @param height The height of this BGTile.
     * @param normalize Normally X, Y, width and height are in the range [-1,1]. This
     *        parameter specifies whether or not to divide these by the framebuffer
     *        resolution in order to have a 1:1 pixel ratio. This remaps X and Y
     *        to be in the range [0, FBO resolution]. Note though, calls to Tile::set()
     *        will still evaluate in the [-1,1] range.
     * @param texture The AssetManager key to the texture this BGTile uses.
     * @return A pointer to a freshly created BGTile.
     */
    BGTile * makeBGTile(GLfloat x, GLfloat y, GLfloat width, GLfloat height, bool normalize, char * texture);
    
    /**
     * @brief A factory method to be used to create and initialize a SceneTile. Make sure
     *        that this Tile's texture asset is loaded and stored in the AssetManager first.
     * @param x The X position of this SceneTile.
     * @param y The Y position of this SceneTile.
     * @param plane The scrolling plane that this SceneTile will be rendered on.
     * @param width The width of this SceneTile.
     * @param height The height of this SceneTile.
     * @param normalize Normally X, Y, width and height are in the range [-1,1]. This
     *        parameter specifies whether or not to divide these by the framebuffer
     *        resolution in order to have a 1:1 pixel ratio. This remaps X and Y
     *        to be in the range [0, FBO resolution]. Note though, calls to Tile::set()
     *        will still evaluate in the [-1,1] range.
     * @param texture The key to this SceneTile's texture asset.
     * @return A pointer to a freshly created BGTile.
     */
    SceneTile * makeSceneTile(GLfloat x, GLfloat y, tile_plane plane, GLfloat width, 
                              GLfloat height, bool normalize, char * texture);
    /**
     * @brief A factory method used to create an AnimTile. Make sure that this Tile's
     *        texture asset is loaded first.
     * @param x The X position of this AnimTile.
     * @param y The Y position of this AnimTile.
     * @param plane The scrolling plane to render this AnimTile on.
     * @param width The width of this AnimTile.
     * @param height The height of this AnimTile.
     * @param normalize Normally X, Y, width and height are in the range [-1,1]. This
     *        parameter specifies whether or not to divide these by the framebuffer
     *        resolution in order to have a 1:1 pixel ratio. This remaps X and Y
     *        to be in the range [0, FBO resolution]. Note though, calls to Tile::set()
     *        will still evaluate in the [-1,1] range.
     * @param texture The key to the this AnimTile's texture in the Renderer's AssetManager.
     * @param numFrames The number of frames held in the texture.
     * @param framewidth The width of each individual frame.
     * @param frameHeight The height of each individual frame.
     * @param frameTime The duration of each frame.
     * @return 
     */
    AnimTile * makeAnimTile(GLfloat x, GLfloat y, tile_plane plane, GLfloat width,
                            GLfloat height, bool normalize, char * texture,
                            unsigned int numFrames, unsigned int framewidth, unsigned int frameHeight,
                            float frameTime);
    
    /**
     * @brief A factory method used to create a PostTile. Make sure that every
     *        texture specified is already loaded.
     *        To not use a texture slot, just pass in NULL. Note though, that 
     *        the corresponding uniform must still be present in the shader. 
     *        The only difference is that an empty texture will be passed to 
     *        it. 
     *        Also realize that since PostTiles use bespoke shaders X, Y, plane, 
     *        width and height may be ignored by the shader. 
     *        Also because of the custom shaders, it's assumed that these will
     *        have transparency. 
     *        Finally, remember that the shader and textures must already be 
     *        added to the Renderer's Asset Manager.
     * 
     * @param x The X position of this PostTile.
     * @param y The Y position of this PostTile.
     * @param plane The parallax plane to draw this PostTile to.
     * @param width The width of this PostTile.
     * @param height The height of this PostTile.
     * @param normalize Normally X, Y, width and height are in the range [-1,1]. This
     *        parameter specifies whether or not to divide these by the framebuffer
     *        resolution in order to have a 1:1 pixel ratio. This remaps X and Y
     *        to be in the range [0, FBO resolution]. Note though, calls to Tile::set()
     *        will still evaluate in the [-1,1] range.
     * @param texA The first custom texture that this PostTile uses.
     * @param texB The second custom texture that this PostTile uses.
     * @param texC The third custom texture that this PostTile uses.
     * @param texD The fourth custom texture that this PostTile uses.
     * @param shader This PostTile's shader.
     * @return A pointer to a freshly constructed PostTile.
     */
    PostTile * makePostTile(GLfloat x, GLfloat y, tile_plane plane, GLfloat width, 
                            GLfloat height, bool normalize, char * texA, char * texB,
                            char * texC, char * texD, char * shader);
    
    /**
     * @brief Helper function.
     *        Returns the Tile's X position in the range [0, FBO horizontal resolution].
     * @param tile The Tile instance to check.
     * @return The Tile's X position in the range[0, FBO horizontal resolution].
     */
    GLfloat getTileSSX(Tile * tile);
    
    /**
     * @brief Helper function.
     *        Returns the Tile's Y position in the range [0, FBO vertical resolution].
     * @param tile The Tile instance to check.
     * @return The Tile's Y position in the range[0, FBO vertical resolution].
     */
    GLfloat getTileSSY(Tile * tile);
    
    /**
     * @brief Helper function.
     *        Sets the Tile's position with an expected range of [0, FBO resolution]
     * @param tile The Tile instance to update.
     * @param x The Tile's new X position in the range[0, FBO horizontal resolution].
     * @param y The Tile's new Y position in the range[0, FBO vertical resolution].
     */
    void setTileSSPos(Tile * tile, GLfloat x, GLfloat y);
    
    /**
     * @brief Helper function.
     *        Returns the Tile's width in the range [0, FBO horizontal resolution].
     * @param tile The Tile instance to check.
     * @return The Tile's width in the range [0, FBO horizontal resolution].
     */
    GLfloat getTileSSW(Tile * tile);
    
    /**
     * @brief Helper function.
     *        Returns the Tile's height in the range [0, FBO vertical resolution].
     * @param tile The Tile instance to check.
     * @return The Tile's width in the range [0, FBO horizontal resolution].
     */
    GLfloat getTileSSH(Tile * tile);
    
    /**
     * @brief Helper function.
     *        Sets the Tile's dimensions, with an expected range of [0, FBO resolution]
     * @param tile The Tile instance to update.
     * @param w The Tile's new width in the range [0, FBO horizontal resolution].
     * @param h The Tile's new height in the range [0, FBO vertical resolution].
     */
    void setTileSSDim(Tile * tile, GLfloat w, GLfloat h);
    
    /**
     * @brief Destroys the Renderer, clearing its AssetManager and freeing
     *        the memory that stores the Tile geometry.
     */
    void destroy();
    
};

#endif // RENDERER_H

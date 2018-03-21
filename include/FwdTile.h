#ifndef FWDTILE_H
#define FWDTILE_H

#include "Shader.h"
#include "Tile.h"
#include "Renderer.h"
#include "Shader.h"
#include "BasicMatrix.h"

/**
 * @class DefTile
 * @author Gerard Geer
 * @date 06/13/16
 * @file DefTile.h
 * @brief DefTiles are drawn in the deferred stage of rendering, giving them access
 *        to the contents of the forward pass framebuffer. But what if you didn't 
 *        care for that? What if you just wanted a simple programmable Tile that's
 *        part of the forward pass? FwdTiles are for you.
 *        Like the DefTile, you need to write a GLSL 1.2 shader. You have access to
 *        the following uniforms to make that easier.
 *        resolution: Buffer resolution (vec2)
 *        time: Time since the window was created (float)
 *        camera: The current position of the camera (vec2)
 *        transform: The Tile's transformation matrix. Note though that unlike
 *         other Tiles, this one must have the Camera position taken into its 
 *         account by the vertex shader. (mat3)
 *        texA: The first custom texture. (sampler2D)
 *        texB: The second custom texture. (sampler2D)
 *        texC: The third custom texture. (sampler2D)
 *        texD: The fourth custom texture. (sampler2D)

 */
class FwdTile : public Tile
{
private:

    /*
     * The keys to the four custom textures.
     */
    char * texA;
    char * texB;
    char * texC;
    char * texD;
    
    /*
     * The key to the DefTile's custom shader.
     */
    char * shader;

    /*
     * Static members so we don't have to malloc in the render function.
     */
    static GLfloat * resolution;
    static GLfloat * camPosition;
    static GLfloat * pOffset;

public:
    
    /**
     * @brief Constructs a new DefTile. Don't forget to call init()!
     */
    FwdTile();
    /**
     * @brief Destructs this FwdTile. Don't forget to call destroy() first!
     */
    ~FwdTile();
    
    /**
     * @brief Initializes this FwdTile. This is a hefty one. It sets up the
     *        internal BasicMatrix pointer, as well as defines the shader and
     *        all the textures to be used. 
     *        To not use a texture slot, just pass in NULL. Note though, that 
     *        the corresponding uniform must still be present in the shader. 
     *        The only difference is that an empty texture will be passed to 
     *        it. 
     *        Also realize that since DefTiles use bespoke shaders X, Y, plane, 
     *        width and height may be ignored by the shader. 
     *        Also because of the custom shaders, it's assumed that these will
     *        have transparency. 
     *        Finally, remember that the shader and textures must already be 
     *        added to the Renderer's Asset Manager.
     * 
     * @param x The X position of this FwdTile.
     * @param y The Y position of this FwdTile.
     * @param plane The parallax plane to draw this FwdTile to.
     * @param width The width of this FwdTile.
     * @param height The height of this FwdTile.
     * @param texA The first custom texture that this FwdTile uses.
     * @param texB The second custom texture that this FwdTile uses.
     * @param texC The third custom texture that this FwdTile uses.
     * @param texD The fourth custom texture that this FwdTile uses.
     * @param shader this FwdTile's shader.
     */
    void init(GLfloat x, GLfloat y, tile_plane plane, GLfloat width, 
              GLfloat height, char * texA, char * texB,
              char * texC, char * texD, char * shader);

    /**
     * @brief Called by the Renderer to draw this Tile.
     * @param r A pointer to the calling Renderer instance.
     */
    void render(Renderer * r);
    
    /**
     * @brief Changes the key used to acces this FwdTile's shader.
     * @param shader The new key.
     */
    void setShader(char* shader);
    
    /**
     * @brief Changes the key used to access this FwdTile's first texture.
     * @param texA The new key.
     */
    void setTexA(char* texA);
    
    /**
     * @brief Changes the key used to access this FwdTile's second texture.
     * @param texA The new key.
     */
    void setTexB(char* texB);
    
    /**
     * @brief Changes the key used to access this FwdTile's third texture.
     * @param texA The new key.
     */
    void setTexC(char* texC);
    
    /**
     * @brief Changes the key used to access this FwdTile's fourth texture.
     * @param texA The new key.
     */
    void setTexD(char* texD);
    
    /**
     * @brief Returns the key used to access this FwdTile's shader.
     * @return The key used to access this FwdTile's Shader.
     */
    char * getShader();
    
    /**
     * @brief Returns the key used to access this FwdTile's first custom texture.
     * @return The key used to access this FwdTile's first custom texture.
     */
    char * getTexA();
    
    /**
     * @brief Returns the key used to access this FwdTile's second custom texture.
     * @return The key used to access this FwdTile's second custom texture.
     */
    char * getTexB();
    
    /**
     * @brief Returns the key used to access this FwdTile's second custom texture.
     * @return The key used to access this FwdTile's second custom texture.
     */
    char * getTexC();
    
    /**
     * @brief Returns the key used to access this FwdTile's second custom texture.
     * @return The key used to access this FwdTile's second custom texture.
     */
    char * getTexD();
    
    /**
	 * @brief Prints out info to stdout about this Tile.
	 */
    void report();
};

#endif // FWDTILE_H

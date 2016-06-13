#ifndef POSTTILE_H
#define POSTTILE_H

#include "Shader.h"
#include "Tile.h"
#include "Renderer.h"
#include "Shader.h"
#include "BasicMatrix.h"

/**
 * @class PostTile
 * @author Gerard Geer
 * @date 06/13/16
 * @file PostTile.h
 * @brief A PostTile, short for Post-processing Tile. This variety of Tile
 *        allows you to specify up to four custom Textures and have a custom
 *        shader (GLSL 1.2) as well. The shader can only access certain uniforms
 *        though:
 *        resolution: Buffer resolution (vec2)
 *        time: Time since the window was created (float)
 *        camera: The current position of the camera (vec2)
 *        transform: The Tile's transformation matrix. Note though that unlike
 *         other Tiles, this one must have the Camera position taken into its 
 *         account by the vertex shader. (mat3)
 *        fwdColor: The color buffer of the first pass. (sampler2D)
 *        fwdDepth: The depth buffer of the first pass. (sampler2D)
 *        texA: The first custom texture. (sampler2D)
 *        texB: The second custom texture. (sampler2D)
 *        texC: The third custom texture. (sampler2D)
 *        texD: The fourth custom texture. (sampler2D)
 */
class PostTile : public Tile
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
     * The key to the PostTile's custom shader.
     */
    char * shader;
public:
    
    /**
     * @brief Constructs a new PostTile. Don't forget to call init()!
     */
    PostTile();
    /**
     * @brief Destructs this PostTile. Don't forget to call destroy() first!
     */
    ~PostTile();
    
    /**
     * @brief Initializes this PostTile. This is a hefty one. It sets up the
     *        internal BasicMatrix pointer, as well as defines the shader and
     *        all the textures to be used. 
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
     * @param texA The first custom texture that this PostTile uses.
     * @param texB The second custom texture that this PostTile uses.
     * @param texC The third custom texture that this PostTile uses.
     * @param texD The fourth custom texture that this PostTile uses.
     * @param shader This PostTile's shader.
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
     * @brief Changes the key used to acces this PostTile's shader.
     * @param shader The new key.
     */
    void setShader(char* shader);
    
    /**
     * @brief Changes the key used to access this PostTile's first texture.
     * @param texA The new key.
     */
    void setTexA(char* texA);
    
    /**
     * @brief Changes the key used to access this PostTile's second texture.
     * @param texA The new key.
     */
    void setTexB(char* texB);
    
    /**
     * @brief Changes the key used to access this PostTile's third texture.
     * @param texA The new key.
     */
    void setTexC(char* texC);
    
    /**
     * @brief Changes the key used to access this PostTile's fourth texture.
     * @param texA The new key.
     */
    void setTexD(char* texD);
    
    /**
     * @brief Returns the key used to access this PostTile's shader.
     * @return The key used to access this PostTile's Shader.
     */
    char * getShader();
    
    /**
     * @brief Returns the key used to access this PostTile's first custom texture.
     * @return The key used to access this PostTile's first custom texture.
     */
    char * getTexA();
    
    /**
     * @brief Returns the key used to access this PostTile's second custom texture.
     * @return The key used to access this PostTile's second custom texture.
     */
    char * getTexB();
    
    /**
     * @brief Returns the key used to access this PostTile's second custom texture.
     * @return The key used to access this PostTile's second custom texture.
     */
    char * getTexC();
    
    /**
     * @brief Returns the key used to access this PostTile's second custom texture.
     * @return The key used to access this PostTile's second custom texture.
     */
    char * getTexD();
};

#endif // POSTTILE_H

#ifndef POSTTILE_H
#define POSTTILE_H

#include "Shader.h"
#include "Tile.h"
#include "Renderer.h"
#include "Shader.h"
#include "BasicMatrix.h"

class PostTile : public Tile
{
private:
    char * texA;
    char * texB;
    char * texC;
    char * texD;
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
     * @brief Creates a Shader instance, initializes it from the source
     *        code given, and even preconfigures all the expected
     *        PostTile shader uniforms before giving it back.
     * @param vertPath The path to the vertex shader source.
     * @param fragPath The path to the fragment shader source.
     * @param s A shader pointer that we can allocate and initialize.
     * @return Any shader_errors returned in the process.
     */
    static Shader * createPostTileShader(char * vertPath, char * fragPath);

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
     * @return 
     */
    char * getShader();
    char * getTexA();
    char * getTexB();
    char * getTexC();
    char * getTexD();
};

#endif // POSTTILE_H

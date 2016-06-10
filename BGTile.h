#ifndef BGTILE_H
#define BGTILE_H

#include "Shader.h"
#include "Tile.h"
#include "Renderer.h"
#include "BasicMatrix.h"

class BGTile : public Tile
{
private:
    /*
     * The key to this BGTile's texture asset.
     */
    char * texture;
public:

    /**
     * @brief Constructs a new, uninitialized BGTile.
     */
    BGTile();
    
    /**
     * @brief Destructs this BGTile. Doesn't really do much. Call destroy() first.
     */
    ~BGTile();
    
    /**
     * @brief Initializes this BGTile, setting up the underlying BasicMatrix.
     * @param x The X position of this BGTile.
     * @param y The Y position of this BGTile.
     * @param width The width of this BGTile.
     * @param height The height of this BGTile.
     * @param texture The AssetManager key to the texture this BGTile uses.
     */
    void init(GLfloat x, GLfloat y, GLfloat width, GLfloat height, char * texture);
    
    /**
     * @brief We're overloading this so one can't screw with
     *        it. The plane of a BG tile is locked to PLANE_BG.
     * @param plane
     */
    void setPlane(tile_plane plane);
    
    /**
     * @brief Renders the Tile.
     */
    void render(Renderer * r);
    
    
    /**
     * @brief Changes the key used to access this SceneTile's texture.
     * @param texture The new key.
     */
    void setTexture(char * texture);
    
    /**
     * @brief Returns the key used to access this SceneTile's texture.
     * @return This SceneTile's texture key.
     */
    char * getTexture();
    

};

#endif // BGTILE_H

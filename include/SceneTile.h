#ifndef SCENETILE_H
#define SCENETILE_H

class SceneTile;
#include "Shader.h"
#include "Tile.h"
#include "Renderer.h"
#include "BasicMatrix.h"

/**
 * @class SceneTile
 * @author Gerard Geer
 * @date 06/03/16
 * @file SceneTile.h
 * @brief SceneTiles are Tiles that can be drawn on any plane, can have
 *        have transparency, but are not animated as AnimTiles are.
 */
class SceneTile : public Tile
{
private:
    /*
     * The key to this SceneTile's texture asset.
     */
    char * texture;
public:
    
    /**
     * @brief Constructs a new, uninitialized SceneTile.
     */
    SceneTile();
    
    /**
     * @brief Destructs this SceneTile. Doesn't really do muc. Call destroy() first.
     */
    ~SceneTile();
    
    /**
     * @brief Initializes this SceneTile.
     * @param x The X position of this SceneTile.
     * @param y The Y position of this SceneTile.
     * @param plane The scrolling plane that this SceneTile will be rendered on.
     * @param width The width of this SceneTile.
     * @param height The height of this SceneTile.
     * @param trans Whether or not this SceneTile has transparency.
     * @param texture The key to this SceneTile's texture asset.
     */
    void init(GLfloat x, GLfloat y, tile_plane plane, GLfloat width, GLfloat height, bool trans, char * texture);
    
    /**
     * @brief Called by the Renderer to render this SceneTile.
     * @param r A copy of the Renderer to give this SceneTile access to its
     *        Camera and AssetManager.
     */
    void render(Renderer * r);
    
    /**
     * @brief Returns the key used to access this SceneTile's texture.
     * @return This SceneTile's texture key.
     */
    char * getTexture();
    
    /**
	 * @brief Prints out info to stdout about this Tile.
	 */
    void report();

};

#endif // SCENETILE_H

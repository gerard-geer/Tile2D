#ifndef RENDERQUEUE_H
#define RENDERQUEUE_H

#include <vector>
#include <valarray>
#include <map>
#include "AssetManager.h"
#include "Tile.h"

/*
 * A type definition that links a Tile's ID with its index in the rendering queue.
 */
typedef std::pair<unsigned long, unsigned int> IdAndIndex;

class RenderQueue
{
private:

    /*
	 * The vector that we consider to be the queue.
	 */
    std::vector< TileWithType > queue;
    
    /*
	 * A map that maps Tile IDs to indices.
	 */
    std::map< unsigned long, unsigned int > memo;
    
    /**
     * @brief Memoizes or re-memoizes the rendering queue.
     */
    void memoize();
    
    /**
     * @brief This is used as the predicate when sorting Tiles. It places opaque
     *        from front to back, then transparent Tiles from back to front.
     * @param lhs The left-hand-side Tile in the comparision.
     * @param rhs The right-hand-size Tile in the comparison.
     * @return Whether or not the first one is less than the second.
     */
    static bool tileSortingPredicate(TileWithType lhs, TileWithType rhs);
    
public:
    
    /**
     * @brief Adds a Tile to the render queue. Whatever is in the render
     *        queue when Renderer::render() is called will be rendered.
     * @param tile The Tile to add to the render queue.
     */
    void addToRenderQueue(tile_type type, Tile * tile);
    
    /**
     * @brief Removes a single Tile from the sorted rendering queue. This does
     *        not delete the Tile instance the pointer points to.
     * @param tile The Tile instance to remove.
     * @return Whether or not the Tile could be removed. If false, the Tile
     *         was not in the RenderQueue.
     */
    bool removeFromRenderQueue(Tile* tile);
    
    /**
     * @brief Clears the rendering queue. Note that this doesn't destroy
     *        the Tiles within. It just simply clears out the line of Tiles
     *        waiting to get to be drawn.
     */
    void flush();
    
};

#endif // RENDERQUEUE_H
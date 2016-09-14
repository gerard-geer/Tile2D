#include "RenderQueue.h"

void RenderQueue::memoize()
{
    // Clear the old memoization first.
    this->memo.clear();
    
    // Iterate over each item in the queue, and hash its index.
    unsigned int i = 0;
    for(std::vector<TileWithType>::iterator it = this->queue.begin(); it != this->queue.end(); ++it)
    {
        /* it->first: TileType
         * it->second: Tile* */
        this->memo.insert(IdAndIndex(it->second->getID(), i));
        ++i;
    }
}

bool RenderQueue::tileSortingPredicate(TileWithType lhs, TileWithType rhs)
{
    Tile * a = lhs.second;
    Tile * b = rhs.second;
    
    // Let's first split things up into fwd and def tiles.
    if( lhs.first == DEF_TILE && rhs.first != DEF_TILE ) return false;
    if( lhs.first != DEF_TILE && rhs.first == DEF_TILE ) return true;
    
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

void RenderQueue::addToRenderQueue(tile_type type, Tile * tile)
{
    // Just tack the Tile on the end since it'll be sorted.
    this->queue.push_back(TileWithType(type,tile));
    
    // Sort the Tiles to cut down on overdraw.
    std::sort(this->queue.begin(), this->queue.end(), tileSortingPredicate);
    
    // Now that it's sorted, we need to re-memoize.
    this->memoize();
}

bool RenderQueue::removeFromRenderQueue(Tile* tile)
{
    // Check to make sure the Tile is in the memoization.
    if ( this->memo.find(tile->getID()) != this->memo.end() )
    {
        // If it is we get the index of the item from the memoization.
        unsigned int index = this->memo[tile->getID()];
        
        // Use that index to erase.
        this->queue.erase(this->queue.begin()+index);
        
        // Re-memoize the list.
        this->memoize();
        
        // Oh hey we did it! Return true as a reward.
        return true;
    }
    
    // Didn't exist in the queue! Return false to say so.
    return false;
}

void RenderQueue::flush()
{
    this->queue.clear();
    this->memo.clear();
}

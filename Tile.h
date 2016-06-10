#ifndef TILE_H
#define TILE_H

#include <GLFW/glfw3.h>
#include "BasicMatrix.h"

// Forward definitions of Tile and Renderer since these two
// classes are interdependent.
class Tile;
class Renderer;

/**
 * @class Tile
 * @author Gerard Geer
 * @date 05/18/16
 * @file Tile.h
 * @brief This is an abstract class representing a Tile, the base
 *        renderable element in the engine. Everything is a Tile.
 *        The background, sprites, level blocks, etc. are all Tiles.
 */
 
enum tile_plane
{
    PLANE_BG = 9,
    PLANE_NEG_4 = 8,
    PLANE_NEG_3 = 7,
    PLANE_NEG_2 = 6,
    PLANE_NEG_1 = 5,
    PLANE_PLAYFIELD_A = 4,
    PLANE_PLAYFIELD_B = 3,
    PLANE_PLAYFIELD_C = 2,
    PLANE_POS_1 = 1,
    PLANE_POS_2 = 0
};

enum tile_type
{
    BG_TILE,
    SCENE_TILE,
    ANIM_TILE,
    POST_TILE
};

class Tile
{
private:
    
    /*
     * The plane of this Tile.
     */
    tile_plane plane;
    
    /*
     * A BasicMatrix that stores this Tile's position and dimensions
     * for easy and efficient shader uniform assignment.
     */
    BasicMatrix * m;
    
    
    /*
     * Whether or not this Tile has any transparent regions.
     */
    bool trans;
    
    /*
     * How to flip the texture, if at all.
     */
    GLuint texFlip;
    
public:

    static const GLuint FLIP_VERT = 1;
    static const GLuint FLIP_HORIZ = 2;
    
    /**
     * @brief Converts a Tile rendering plane to an appropriate Z depth.
     * @param plane The rendering plane.
     * @return A depth value.
     */
    static GLfloat getParallaxFactor(tile_plane plane);
    
    /**
     * @brief Constructs a new Tile instance.
     */
    Tile();
    
    /**
     * @brief That BasicMatrix pointer is kind of heavy. This makes sure that it is deleted.
     */
    ~Tile();
    
    /**
     * @brief Initializes this Tile's underlying BasicMatrix.
     * @param x The X position of this new Tile.
     * @param y The Y position of this new Tile.
     * @param plane The plane that this Tile is to be rendered on.
     *              For parallax and z-ordering.
     * @param width The width of this new Tile.
     * @param height The height of this new Tile.
     */
    void init(GLfloat x, GLfloat y, tile_plane plane, GLfloat width, GLfloat height, bool trans);
    
    /**
     * @brief An overloading of the < operator. This is primarily
     *        for the Renderer to be able to sort Tiles into a more
     *        effective drawing order.
     * @param rhs The Tile on the Right Hand Side of the equation.
     * @return Whether or not this Tile is "less" than the other.
     */
    bool operator<(const Tile &rhs) const
    {
        // We need to render transparent objects last.
        if(trans && !rhs.hasTrans()) return false;
        if(!trans && rhs.hasTrans()) return true;
        
        // At this point both operands will have the same transparency status.
        
        // Now we're comparing rendering planes. We want to
        // draw the closest Tiles first, so we can take
        // advantage of depth testing and thusly minimize redraw.
        if(plane <= rhs.getPlane()) return true;
        if(plane > rhs.getPlane()) return false;
        return true; // Just to get rid of the warnings.
    }
        
    /**
     * @brief Retrieves the X position of this Tile.
     * @return The X position of this Tile.
     */
    GLfloat getX() const;
    
    /**
     * @brief Returns the Y position of this Tile.
     * @return The Y position of this Tile.
     */
    GLfloat getY() const;
    
    /**
     * @brief Returns the Tile's rendering plane.
     * @return The Tile's rendering plane.
     */
    tile_plane getPlane() const;
    
    /**
     * @brief Returns the width of this Tile.
     * @return The width of this Tile.
     */
    GLfloat getWidth() const;
    
    /**
     * @brief Returns the height of this Tile.
     * @return The height of this Tile.
     */
    GLfloat getHeight() const;
    
    /**
     * @brief Returns whether or not this Tile has transparent regions.
     * @return whether or not this Tile has transparent regions.
     */
    bool hasTrans() const;
    
    /**
     * @brief Returns the current texture flip mode. This is a bitwise
     *        member however, so you might get FLIP_VERT | FLIP_HORIZ.
     * @return The current texture flip mode.
     */
    GLuint getTextureFlip() const;
    
    /**
     * @brief Returns a reference to this Tile's underlying BasicMatrix.
     * @return A reference to this Tile's underlying BasicMatrix.
     */
    BasicMatrix * getMatrix() const;
    
    /**
     * @brief Changes the X position of this Tile. Useful for animation.
     * @param x The new X position.
     */
    void setX(GLfloat x);
    
    /**
     * @brief Changes the Y position of this Tile. Useful for animation.
     * @param y The new Y position.
     */
    void setY(GLfloat y);
    
    /**
     * @brief Changes the rendering plane of this Tile.
     * @param plane The new plane.
     */
    void setPlane(tile_plane plane);
    
    /**
     * @brief Changes the width of this Tile.
     * @param w The new width.
     */
    void setWidth(GLfloat width);
    
    /**
     * @brief Changes the height of this Tile.
     * @param h The new height.
     */
    void setHeight(GLfloat height);
    
    /**
     * @brief Sets the "has-transparency" flag of this Tile.
     * @param Sets whether or not this Tile has transparency.
     */
    void setTransparency(bool trans);
    
    /**
     * @brief Sets the texture flip mode. This is a bitwise member, so
     *        (FLIP_VERT | FLIP_HORIZ) can be used to flip across both
     *        axes.
     * @param flip The new texture flip mode.
     */
    void setTextureFlip(GLuint flip);
    
    /**
     * @brief A virtual function that provides a rendering interface for
     *        any Tile subclasses.
     */
    virtual void render(Renderer* r) = 0;
    
    /**
     * @brief Destroys this Tile's underlying BasicMatrix. Call this before
     * deletion and destruction.
     */
    void destroy();
};

#endif
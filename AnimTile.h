#ifndef ANIMTILE_H
#define ANIMTILE_H

#include "Shader.h"
#include "Tile.h"
#include "Renderer.h"
#include "BasicMatrix.h"

/**
 * @class AnimTile
 * @author Gerard Geer
 * @date 06/13/16
 * @file AnimTile.h
 * @brief An animated Tile. What do I mean by animated? Well this can have a
 *        a texture divided horizontally into different frames, and the shader
 *        steps through them at a specified step.
 */
class AnimTile : public Tile
{
private:

    /*
     * The index of the current frame being displayed.
     */
    unsigned int curFrame;

    /*
     * The total number of frames in the image.
     */
    unsigned int numFrames;

    /*
     * The width of each individual frame.
     */
    unsigned int frameWidth;

    /*
     * The height of each frame.
     */
    unsigned int frameHeight;

    /*
     * The amount of time to pass before moving
     * to the next sprite frame.
     */
    float frameTime;
    
    /*
     * The time of the last frame change.
     */
    float lastChange;

    /*
     * The key to this AnimTile's texture.
     */
    char * texture;

public:

    /**
     * @brief Constructs a new AnimTile. Don't forget to call init()!
     */
    AnimTile();

    /**
     * @brief Destructs this AnimTile. Don't forget to call destroy()!
     */
    ~AnimTile();

    /**
     * @brief Initializes this AnimTile.
     * @param x The X position of this AnimTile.
     * @param y The Y position of this AnimTile.
     * @param plane The scrolling plane to render this AnimTile on.
     * @param width The width of this AnimTile.
     * @param height The height of this AnimTile.
     * @param trans Whether or not this AnimTile has transparent regions.
     * @param texture The key to the this AnimTile's texture in the Renderer's AssetManager.
     * @param numFrames The number of frames held in the texture.
     * @param framewidth The width of each individual frame.
     * @param frameHeight The height of each individual frame.
     * @param frameTime The duration of each frame.
     */
    void init(GLfloat x, GLfloat y, tile_plane plane, GLfloat width,
              GLfloat height, bool trans, char * texture,
              unsigned int numFrames, unsigned int framewidth, unsigned int frameHeight,
              float frameTime);

    /**
     * @brief Called by the Renderer to draw this AnimTile to the screen.
     * @param r The renderer to draw.
     */
    void render(Renderer * r);
    
    /**
     * @brief Returns the amount of time to wait between changing frames.
     * @return  The amount of time to wait before iterating the current frame.
     */
    float getFrameTime() const;
    
    /**
     * @brief Returns the current frame of animation.
     * @return The current frame of animation.
     */
    unsigned int getCurFrame() const;

    /**
     * @brief Returns the height of each individual frame.
     * @return The height of each individual frame.
     */
    unsigned int getFrameHeight() const;
    
    /**
     * @brief Returns the width of each individual frame.
     * @return The width of each individual frame.
     */
    unsigned int getFrameWidth() const;
    
    /**
     * @brief Returns the number of frames in the animation.
     * @return The number of frames in the animation.
     */
    unsigned int getNumFrames() const;
    
    /**
     * @brief Returns the key used to retrieve the texture.
     * @return The key used to retrieve the texture.
     */
    char* getTexture();
    
    /**
     * @brief Sets the inter-frame duration.
     * @param animWait The new amount of time to wait between frames.
     */
    void setFrameTime(float animWait);
    
    /**
     * @brief Sets the current frame. Don't exceed numFrames! That'll
     *        be some seriously undefined stuff.
     * @param curFrame The new current frame.
     */
    void setCurFrame(unsigned int curFrame);
    
    /**
     * @brief Sets the number of frames. Dishonest values will
     *        mess things up in ways that aren't fun.
     * @param numFrames The new number of frames stored in the texture.
     */
    void setNumFrames(unsigned int numFrames);

};

#endif // ANIMTILE_H

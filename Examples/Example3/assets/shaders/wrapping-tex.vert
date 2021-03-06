#version 120
/**
 * File: ex_post_tile_shader.vert
 * Author: Gerard Geer (github.com/gerard-geer)
 * License: GPL v3.0
 * 
 * This is an example vertex shader for FwdTiles, which require
 * shaders to be written for them. There are several things to
 * note about this shader:
 * -First, the transformation matrix of PostTiles isn't set up to
 *  automatically perform parallax scrolling, in order to make it
 *  possible to disable it in the vertex stage. If you want the
 *  scolling, you'll have to do it yourself. (This example shows
 *  you how.)
 * -You also need to do texture flipping yourself if you want it.
 *  (There's an example of this down below as well.)
 * -Second, your shaders must declare the two attribute variables.
 *  GLSL doesn't care if you implement the uniforms, but if you
 *  want to use them, you'll have to declare them yourself.
 * -You can't specify your own uniforms, but you can mix the
 *  uniforms of the two stages, and even use the same ones in both.
 * -The engine itself targets OpenGL 2.1 (With framebuffer extension).
 *  Therefore, use GLSL version 1.2 or earlier.
 */

// The vertex position being wrought from the VAO.
attribute vec3 vertPos;

// The vertex texture coordinate, also from the VAO.
attribute vec2 vertUV;

// The transformation matrix, containing only position and scale.
uniform mat3 transform;

// The position of the camera.
uniform vec2 camera;

// The factor by which scrolling is modified to facilitate the parallax effect.
uniform float pFactor;

// The center-of-parallax offset.
uniform vec2 pOffset;

// Whether or not to ignore scrolling.
uniform float ignoreScroll;

// The depth of this PostTile.
uniform float depth;

// Texture flip requests.
uniform int hFlip;
uniform int vFlip;

// We want to give the fragment shader a texture coordinate, right?
varying vec2 fragUV;

// The position of the Tile, since we only move the texture and not the geometry.
varying vec2 pos;

/**
 * This modifies the transformation matrix by making it into a modelview 
 * projection matrix that performs parallax scrolling.
 */
mat3 parallaxSetup()
{
    // One can't edit uniforms, so we need to make a copy.
    mat3 m = transform;
    
    // Now we do the actual math to enable the parallax.
    // This boils down to essentially:
    // TilePosition = (TilePosition - CameraPosition) / ParallaxFactor
    m[2][0] = (m[2][0] - camera.x)*pFactor - pOffset.x*(1.0-pFactor);
    m[2][1] = (m[2][1] - camera.y)*pFactor - pOffset.y*(1.0-pFactor);
    
    return m;
}

/**
 * The entry point into the shader.
 */
void main(void)
{
    // Get the parallax matrix.
    mat3 m = parallaxSetup();
    
    // Set the output position to just that of full screen. We tell
    // the fragment shader where the Tile is, so that rather than moving the
    // the Tile, we just scroll the texture. 
    gl_Position.xyz = (vertPos*2.0);
    gl_Position.y = gl_Position.y;
    
    // Now we just need to set the depth and W term. Since the negative
    // Z axis goes into the screen, a Z coordinate <depth> deep would be
    // at -depth.
    gl_Position.zw = vec2(depth,1);
    
    pos = m[2].xy;
    
    // Oh, let's not forget to send over a texture coordinate.
    fragUV = vertUV;
    
    // If we've got horizontal flip, we need to flip.
    if( hFlip > 0 ) fragUV.x = 1.0-fragUV.x;
    
    // Do the same for horizontal.
    if( vFlip > 0 ) fragUV.y = 1.0-fragUV.y;
}

#version 120
/**
 * File: ex_def_tile_shader.vert
 * Author: Gerard Geer (github.com/gerard-geer)
 * License: GPL v3.0
 * 
 * This is an example vertex shader for DefTiles, which require
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

// Whether or not to ignore scrolling.
uniform float ignoreScroll;

// The depth of this PostTile.
uniform float depth;

// Texture flip requests.
uniform int hFlip;
uniform int vFlip;

/**
 * The entry point into the shader.
 */
void main(void)
{
    // Multiply the vertex position by the parallax projection matrix.
    gl_Position.xyz = (vertPos*2.0);
    
    // Now we just need to set the depth and W term. Since the negative
    // Z axis goes into the screen, a Z coordinate <depth> deep would be
    // at -depth.
    gl_Position.zw = vec2(depth,1);
}

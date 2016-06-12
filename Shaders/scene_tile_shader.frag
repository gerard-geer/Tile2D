#version 120
/**
 * File: scene_tile_shader.frag
 * Author: Gerard Geer
 * License: GPL v3.0
 *
 * This is the fragment shader for SceneTiles. It only needs
 * to sample the texture and color the current fragment.
 */

// A sampler bound to the texture unit that we sent this
// SceneTile's texture to.
uniform sampler2D texture;

// The interpolated texture coordinate we get from the 
// vertex shader.
varying vec2 fragUV;

/**
 * The fragment shader entry point.
 */
void main(void)
{
    // Do some samplin'
    gl_FragColor = texture2D(texture, fragUV);
}
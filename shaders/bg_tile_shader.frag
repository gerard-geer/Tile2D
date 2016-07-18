#version 120
/**
 * File: bg_tile_shader.frag
 * Author: Gerard Geer
 * License: GPL v3.0
 *
 * This is the fragment shader for BGTiles. It only needs
 * to sample the texture and color the current fragment.
 */

// A sampler bound to the texture unit that we sent this
// BGTile's texture to.
uniform sampler2D texture;

// The interpolated texture coordinate we get from the 
// vertex shader via the rasterizer step.
varying vec2 fragUV;

/**
 * The fragment shader entry point.
 */
void main(void)
{
    gl_FragColor = texture2D(texture, fragUV);
}
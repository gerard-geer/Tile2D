#version 120
/**
 * File: anim_tile_shader.frag
 * Author: Gerard Geer
 * License: GPL v3.0
 * 
 * This is the AnimTile fragment shader. All the hard work
 * was done in the vertex shader, so we don't have much left
 * to do here but sample the texture.
 */
 
// The texture of this AnimTile.
uniform sampler2D texture;

// The texture coordinates that have been interpolated for us.
varying vec2 fragUV;

/**
 * The shader's entry point.
 */
void main(void)
{
    gl_FragColor = texture2D(texture, fragUV);
}
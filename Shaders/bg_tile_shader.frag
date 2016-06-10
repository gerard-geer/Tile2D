#version 120

/**
 * This is the fragment shader for BGTiles. It only needs
 * to sample the texture and color the current fragment.
 */

// A sampler bound to the texture unit that we sent this
// BGTile's texture to.
uniform sampler2D texture;

// The interpolated texture coordinate we get from the 
// vertex shader via the rasterizer step.
uniform vec2 fragUV;

/**
 * The fragment shader entry point.
 */
void main(void)
{
    gl_FragColor = texture2D(texture, fragUV);
}
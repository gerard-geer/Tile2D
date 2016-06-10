#version 120

/**
 * This is the stock vertex shader for a BGTile. It transforms the
 * incoming X and Y coordinates by a homogeneous transformation
 * matrix, then sets the depth to the requested level.
 */

// The vertex position taken from the VAO.
attribute vec3 vertPos;

// The vertex texture coorinate, also taken from the VAO.
attribute vec2 vertUV;

// The transformation matrix. This scales the tile to the
// appropriate size and moves it to the correct position. This
// matrix also does parallax scrolling passively.
uniform mat3 transform;

// The depth of this BGTile. Since it's a BGTile, this value will
// always be the same.
uniform float depth;

// The texture coordinate that we'll send off to get interpolated
// and passed to the fragment stage.
varying vec2 fragUV;

/**
 * The entry point to this shader.
 */
void main(void)
{
    // Get the texture coordinate squared away.
    fragUV = vertUV;
    
    // Transform the vertex position, casting it to the correct type.
    gl_Position = vec4( transform*vertPos, 1.0 );
    
    // Now once we set the depth, we're done.
    gl_Position.z = depth;
}

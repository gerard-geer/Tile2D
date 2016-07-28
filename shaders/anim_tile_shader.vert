#version 120
/**
 * File: anim_tile_shader.vert
 * Author: Gerard Geer
 * License: GPL v3.0
 *
 * This is the vertex shader for AnimTiles. AnimTiles store multiple
 * frames of animation linearly in their texture, and this vertex
 * shader manipulates the incoming texture coordinates to be centered
 * on the current frame. We do all the work here so we don't have to
 * do exponentially more in the fragment shader.
 */
 
// The VAO's idea of a vertex position. It's a pretty solid idea.
attribute vec3 vertPos;

// The VAO's interpretation of a texture coordinate. Also solid.
attribute vec2 vertUV;

// The dimensions of a single frame, expressed as a fraction of the
// whole texture.
uniform vec2 fractFrameDim;

// The index of the current frame.
uniform int curFrame;

// The transformation matrix common to all the Tiles.
uniform mat3 transform;

// The depth of this AnimTile.
uniform float depth;

// Texture flip requests.
uniform int hFlip;
uniform int vFlip;

// The texture coordinates we're going to send to the fragment stage.
varying vec2 fragUV;

/**
 * The entrypoint into the shader.
 */
void main(void)
{
    // First we do the usual setup of the vertex.
    gl_Position = vec4( (transform*vertPos).xy, depth, 1.0 );
    
    // Now we do the texture coordinates.
    fragUV = vertUV * fractFrameDim;
    fragUV.x += fractFrameDim.x * float(curFrame);
    
    // If we've got horizontal flip, we need to flip.
    if( hFlip > 0 ) fragUV.x = 1.0-fragUV.x;
    
    // Do the same for horizontal.
    if( vFlip > 0 ) fragUV.y = 1.0-fragUV.y;
}
    


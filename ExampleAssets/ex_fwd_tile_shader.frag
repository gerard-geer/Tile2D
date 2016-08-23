#version 120

/**
 * File: ex_fwd_tile_shader.frag
 * Author: Gerard Geer
 * License: GPL v3.0
 * 
 * This is an example fragment shader for PostTiles. PostTile fragment
 * shaders have requirements just like the vertex shaders.
 * -Depth-testing against the previous path isn't done by OpenGL. Rather,
 *  you must implement it yourself in the fragment stage. (This example
 *  shader includes a suggestion as to how to do this.
 * -Remember, you can share with and take uniforms from the vertex shader.
 * -If the depth test fails, write out an alpha of 0. This is how the
 *  compositor knows to display the first pass rather than this one.
 * -Also remember to target GLSL version 1.2.
 * -Want to have transparent fragments? Just output a transparent fragment.
 * -The clear color used by the renderer has an alpha of zero. Therefore if
 *  you wish to output blank regions of the forward pass' color buffer, you
 *  must set the alpha to 1.0 manually.
 */

// The time since the window was created.
uniform float time;

// The resolution of the framebuffer being rendered into.
uniform vec2 resolution;

// The four custom textures.
uniform sampler2D texA;
uniform sampler2D texB;
uniform sampler2D texC;
uniform sampler2D texD;

// The texture coordinate we get from the vertex stage.
varying vec2 fragUV;

/**
 * Performs the depth test.
 */
bool depthTest(float existingDepth)
{
    return (gl_FragCoord.z < existingDepth);
}

/**
 * Gives a cool color value.
 */
vec4 someCoolStuff(vec2 uv)
{
    uv *= 10.0;
    float coolValue = step(.5, sin(uv.x)*cos(uv.y))
    
    return vec4(coolValue);
}

/**
 * The entry point of this shader.
 */
void main(void)
{
    
    // First we need a screen-relative normalized coordinate.
    vec2 uv = gl_FragCoord.xy/resolution;
    
    // Get the existing depth.
    float depth = texture2D(fwdDepth, uv).r;
    
    // Do the depth test, and if we fail set the fragment to transparent.
    if( !depthTest(depth) )
    {
        // Transparent green.
        gl_FragColor = vec4(0, 1, 0, 0);
        return;
    }
    
    // If we don't fail we set it to some cool colors.
    gl_FragColor = someCoolStuff(uv);
}
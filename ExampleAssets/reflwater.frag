#version 120

/**
 * File: ex_def_tile_shader.frag
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

// The color buffer of the first pass.
uniform sampler2D fwdColor;

// The depth buffer of the first pass.
uniform sampler2D fwdDepth;

// The transformation matrix, containing only position and scale.
uniform mat3 transform;

// The four custom textures.
uniform sampler2D texA;
uniform sampler2D texB;
uniform sampler2D texC;
uniform sampler2D texD;

// The texture coordinate we get from the vertex stage.
varying vec2 fragUV;

// The depth of this PostTile.
uniform float depth;

/**
 * Performs the depth test.
 */
bool depthTest(float existingDepth)
{
    return (gl_FragCoord.z < existingDepth);
}

/**
 * Silly 2d ray trace.
 */
vec3 silly2Drt(in vec2 start)
{
    // The current marching position.
    vec2 cur = start;

    // The current depth sample.
    float d = 0;

    // Resolution independent step size.
    float s = 1.0/resolution.y;

    // Get the max distance possible to travel.
    float maxd = fragUV.y * transform[1][1]*.5;///resolution.y;
    //return vec3(maxd);

    // March along the depth buffer until we hit something that's in front of us.
    for(int i = 0; i < 128; ++i)
    {
        d = texture2D(fwdDepth, cur).r;
        if(gl_FragCoord.z > d || cur.y-start.y > maxd)
            break;
        else cur.y += s;
    }

    vec2 uv = cur+(cur-start);
    uv.x += sin(500.0*(cur.y-start.y)-time*10.0)*(cur.y-start.y)*.1;
    return texture2D(fwdColor, uv).rgb;
}

/**
 * The entry point of this shader.
 */
void main(void)
{
    
    // First we need a screen-relative normalized coordinate.
    vec2 uv = gl_FragCoord.xy/resolution;
    //gl_FragColor = vec4(vec3(fragUV.y),1.0);return;
    
    // Get the existing depth.
    float depth = texture2D(fwdDepth, uv).r;
    
    // Do the depth test, and if we fail set the fragment to transparent.
    if( !depthTest(depth) )
    {
        // Transparent green.
        gl_FragColor = vec4(0, 1, 0, 0);
        return;
    }
    
    // Get the reflection.
    vec3 refl = silly2Drt(uv);
    
    // Mix it with sea color.
    vec3 color = mix(refl, vec3(.22,.61,.91), .667);
    
    // Mix it with some 
    // If we don't fail we set it to some cool colors.
    gl_FragColor = vec4(color,1.0);
}
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

// The four custom textures.
uniform sampler2D texA;
uniform sampler2D texB;
uniform sampler2D texC;
uniform sampler2D texD;

// The texture coordinate we get from the vertex stage.
varying vec2 fragUV;

float isItBG(in vec2 uv)
{
    float depth = texture2D(fwdDepth, uv).r;
    return step(.99,1.0-abs(depth-.999));
}

vec4 gs(in vec2 uv, in float coeff) // Gauss Sample.
{
    return texture2D(fwdColor, uv)*coeff*2.*isItBG(uv);
}

/**
 * The entry point of this shader.
 */
void main(void)
{
    
    // First we need a screen-relative normalized coordinate.
    vec2 uv = gl_FragCoord.xy/resolution;
    
    // Pixel size.
    vec2 px = 1.0/resolution;
    
    // OH GOD SINGLE PASS GAUSSIAN 25 samples per pixel jazzus.
    #define V vec2
    #define S(x,y,c) gs(uv-px*V(x,y),c)
    gl_FragColor  = S(-2,-2,.003765) + S(-1,-2,.015019) + S( 0,-2,.023792) + S( 1,-2,.015019) + S( 2,-2,.003765);
    gl_FragColor += S(-2,-1,.015019) + S(-1,-1,.059912) + S( 0,-1,.094907) + S( 1,-1,.059912) + S( 2,-1,.015019);
    gl_FragColor += S(-2, 0,.023792) + S(-1, 0,.094907) + S( 0, 0,.150342) + S( 1, 0,.094907) + S( 2, 0,.023792);
    gl_FragColor += S(-2, 1,.015019) + S(-1, 1,.059912) + S( 0, 1,.094907) + S( 1, 1,.059912) + S( 2, 1,.015019);
    gl_FragColor += S(-2, 2,.003765) + S(-1, 2,.015019) + S( 0, 2,.023792) + S( 1, 2,.015019) + S( 2, 2,.003765);
    #undef V
    #undef S
    vec4 orig = texture2D(fwdColor, uv);
    orig += orig*.125*isItBG(uv);
    gl_FragColor = mix(orig, gl_FragColor, 1.0-isItBG(uv));
    
}
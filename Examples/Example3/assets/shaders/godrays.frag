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
#define RBLUR_SAMPLES 128

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

/**
 * Uses the mask texture and depth to determine the current
 * radial blur mask.
 */
float mask( in vec2 uv )
{
    // Get the depth.
    float depth = texture2D(fwdDepth,uv).r;

    // Get a texel of the ray origin.
    float origin = texture2D(texB,vec2(uv.x,1.0-uv.y)).r;

    // Return the origin and whether or not the background is visible.
    return step(.78,depth); // That's just what ends up being the bg depth.
}

/**
 * Pretty quick radial blur. How does it work? Samples the luminance
 * buffer at increasingly large scales centered around <center> and
 * accumulates the results.
 * It's actually just a tweak on @jcant0n's version so I could learn
 * this blur works.
 *
 * https://www.shadertoy.com/view/XsfSDs
 * 
 * Parameters:
 * uv: Screen coordinates, normalized however you see fit.
 * center: The center point of the blur, in the same system as <uv>
 * falloffExp: An exponent to attenuate how gosh darn bright it is.
 */
float rblur( in vec2 uv, in vec2 center, float falloffExp )
{
    // Translate our floating point space to the center of our blur.
    uv -= center;
    
    // Go ahead and precompute the inverse of the number of samples.
    // so we don't have any inner divisions.
    float invSamples = 1.0 / float(RBLUR_SAMPLES);
    
    // Place to accumulate the result.
    float result = 0.0;
    
    // Independent locations to store the results of each inner tap.
    // Why? So each tap doesn't need to write back before the next one
    // can start executing, preventing stalls. Works on x86 and piped
    // MIPS and I think it helps this out too (at least on my old thinkpad).
    float r0=0.0,r1=0.0,r2=0.0,r3=0.0;
    
    // We need to do each tap at a different index/position, so by storing
    // them in a vector we can make incrementation a single op instead of 4.
    vec4 indices = vec4(0,1,2,3);
    
	// Same thing with the scale.
    vec4 scale = vec4(0);
    
    // Go through and and sample the texture.
    for( int i = 0; i < RBLUR_SAMPLES; i+=4 )
    {
        scale = indices*invSamples;
        r0 = mask(uv*scale.x + center);
        r1 = mask(uv*scale.y + center);
        r2 = mask(uv*scale.z + center);
        r3 = mask(uv*scale.w + center);
        indices += 4.0;
        result += r0+r1+r2+r3;
    }
    return pow(result * invSamples,falloffExp);
}

/**
 * The entry point of this shader.
 */
void main(void)
{
    
    // First we need a screen-relative normalized coordinate.
    vec2 uv = gl_FragCoord.xy/resolution;

    // Get a blur value.
    float b = rblur(uv, vec2(.1,.375),2.5);

    // Get the original BG.
    vec4 fwd = texture2D(fwdColor,uv);

    // Get the depth.
    float depth = texture2D(fwdDepth,uv).r;


    
    gl_FragColor = vec4(vec3(1,.9,.7)+fwd.rgb,b)*step(depth,.78)*.75;

}
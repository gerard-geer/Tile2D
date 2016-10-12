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

// The position of the Tile itself.
varying vec2 pos;

/**
 * Performs the depth test.
 */
bool depthTest(float existingDepth)
{
    return (gl_FragCoord.z < existingDepth);
}

// OH WAIT GLSL 1.2 ISN'T GUARANTEED TO HAVE SMOOTHSTEP.
float ss( in float edge0, in float edge1, in float x )
{
    float t = clamp((x - edge0) / (edge1 - edge0), 0.0, 1.0);
    return t * t * (3.0 - 2.0 * t);
}

// Since textures are sampled nearest neighbor style, we have
// to overcome that.
vec4 linearSample(vec2 uv, vec2 texDim, sampler2D tex)
{
    vec3 px = vec3(1.0/texDim,0);
    uv *= texDim;
    vec2 local = fract(uv);
    uv = floor(uv)*px.xy;
    
    vec4 s00 = texture2D(tex, uv+px.zz);
    vec4 s01 = texture2D(tex, uv+px.zy);
    vec4 s11 = texture2D(tex, uv+px.xy);
    vec4 s10 = texture2D(tex, uv+px.xz);
    
    return mix( mix(s00, s10, local.x), mix(s01, s11, local.x), local.y );
}

/**
 * Creates crepuscular rays.
 */
float crepuscular(vec2 uv)
{
    uv.x += .05*uv.y;
    uv *= .05;
    vec2 dim = vec2(256);
    float n = linearSample(uv.xx, dim, texA).r;
    n += linearSample(uv.xx*2.0, dim, texA).r*.5;
    n /= 1.5;
    return ss(.4, 1.0, n);
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
    float c = crepuscular((uv+pos*.5));
    gl_FragColor = vec4(c,c,c,.75*c);
}
#version 120

/**
 * File: ex_def_tile_shader.frag
 * Author: Gerard Geer
 * License: GPL v3.0
 *
 * This is an example custom compositor fragment shader. It composites the two
 * rendering passes, but also creates a scanline effect and does some vignetting.
 *
 * -Custom composition shaders get four uniforms:
 *    -fwdFB (sampler2D) The framebuffer of the forward pass.
 *    -defFB (sampler2D) The framebuffer of the deferred pass.
 *    -winResolution (vec2) The resolution of the window.
 *    -winResolution (vec2) The resolution of the framebuffers.
 *    -time (float) The time since the OpenGL context was created.
 *  The samplers can only be accessed in the fragment shader, but the others can be
 *  used in either stage.
 *
 * -As always, target GLSL 1.2.
 * 
 * **How to composite the two passes**
 *   The deferred pass should have done depth-testing itself, and if that test failed
 *   it should have output vec4(0). Since that leaves failed tests and unused FBO
 *   regions both with and alpha of 0, we can composite using the alpha channel of the
 *   deferred pass. (Yes, the clear color of the deferred pass has an alpha of 0.)
 * 
 * -In short, to composite:
 *   vec4 composited = vec4(mix(fwd.rgb, def.rgb, def.a), 1.0);
 *
 * -Note, this must be done for the final frame to look anywhere near as intended.
 *  Whatever else you do is up to you.
 */ 

// Interpolated texture coordinates.
varying vec2 fragUV;

// The first pass' framebuffer.
uniform sampler2D fwdFB;

// The second pass' framebuffer.
uniform sampler2D defFB;

// The resolution of the window.
uniform vec2 winResolution;

// The resolution of the internal framebuffers.
uniform vec2 fbResolution;

// The time since the OpenGL context was created.
uniform float time;

float vignette()
{
    // Get some normalized screen coordinates ( [-1, 1] )
    vec2 uv = fragUV*2.0 - 1.0;
    
    // Set aspect ratio to that of the screen.
	uv.x *= winResolution.x/winResolution.y;
    
    // Do some vignetting.
    return 1.0-dot(uv,uv)*.1;
}

float scanlines()
{
    float y = fragUV.y;
    y *= fbResolution.y*3.14159;
    return abs(sin(y));
}

void main(void)
{

    // Sample the textures and mix based on alpha. That's it!
    vec4 fwd = texture2D(fwdFB, fragUV);
    vec4 def = texture2D(defFB, fragUV);
    vec4 raw = vec4(mix(fwd.rgb, def.rgb, def.a), 1.0);
    
    raw.rgb *= vignette();
    raw.rgb *= .75 + .25*scanlines();
    
    gl_FragColor = raw;
}
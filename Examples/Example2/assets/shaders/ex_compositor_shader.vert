#version 120
/**
 * File: ex_compositor_shader.vert
 * Author: Gerard Geer
 * License: GPL v3.0
 *
 * This is an example custom compositor vertex shader.
 *
 * Things to note:
 *
 * -The goal of this vertex shader is to arrange the incoming vertices into
 *  a full screen quad. Note that the screen goes from (-1, 1), and the default
 *  values of the Tile vertex data is from (-.5, .5). Just multiply them by 2.0.
 *  (You can do wild stuff, just be congnisant of the consequences of such actions.)
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
 */
 
// The incoming vertex position.
attribute vec3 vertPos;

// The incoming vertex texture coordinate.
attribute vec2 vertUV;

// The texture coordinate to be interpolated and sent to the
// fragment shader.
varying vec2 fragUV;

void main(void)
{
    // Translate the X and Y coordinates to the edge of the screen,
    // the Z to "really close", and the W to ... something.
    gl_Position = vec4( vertPos.xy*2.0, 0.0, 1.0 );
    
    // We've also got to flip the Y. Since the coordinates range from
    // -1 to +1, we can just negate.
    gl_Position.y = -gl_Position.y;
    
    // Shove the texture coordinate face first into the interpolator.
    fragUV = vertUV;
}
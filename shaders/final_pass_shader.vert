#version 120
/**
 * This is the vertex shader used in compositing the first
 * and second passes onto a full screen quad.
 * Since we know the dimensions of the screen, we directly
 * manipulate vertex coordinates to save the extra
 * computation of using a matrix.
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
    // -.5 to +.5, we can just negate.
    gl_Position.y = -gl_Position.y;
    
    // Shove the texture coordinate face first into the interpolator.
    fragUV = vertUV;
}
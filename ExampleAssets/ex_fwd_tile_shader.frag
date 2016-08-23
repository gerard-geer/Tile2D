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
 *
 *  NOTE: This example is a silly ray-marcher because I wanted to have fun
 *        in the sun.
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

uniform mat3 transform;

// The texture coordinate we get from the vertex stage.
varying vec2 fragUV;

// The maximum acceptable distance to consider a point found.
#define EPSILON .1

// The max distance we'll allow rays to travel.
#define MAX_DEPTH 5.0

// How many steps we want to allow.
#define NUM_STEPS 40

// Base UP vector.
const vec3 UP = vec3(0,1,0);

/*
 * Standard pinhole camera, but defined in terms of custom axes to allow for
 * camera movement.
 */
void camera(in vec2 uv, in vec3 cp, in vec3 cd, in float f, out vec3 ro, out vec3 rd)
{
	ro = cp;
    vec3 left = (cross(cd,UP));
    vec3 up = (cross(left, cd));
	rd = normalize((cp + cd*f + cross(cd, up)*uv.x + up*uv.y)-ro);
}

/*
 * IQ's cube distance function, with a rotation transformation thrown in there to.
 */
float cube( vec3 p )
{
    float c = cos(time), s = sin(time);
    mat2 rot = mat2(c,s,-s,c);
    p.xz *= rot;
  	return length(max(abs(p)-vec3(1),0.0));
}

/*
 * Returns the surface normal at a found point.
 */
vec3 norm(vec3 p)
{
	return normalize(vec3(cube(vec3(p.x+EPSILON,p.y,p.z)),
						  cube(vec3(p.x,p.y+EPSILON,p.z)),
						  cube(vec3(p.x,p.y,p.z+EPSILON)))-cube(p));
}

/*
 * Marches a position along a ray until that position is within EPSILON
 * distance of a surface.
 */
void march( inout vec3 p, in vec3 d )
{
    float c = cube(p);
    for( int i = 0; i < NUM_STEPS; ++i )
    {
        if( c < EPSILON ) return;
        else
        {
            p += d*c*.5;
            c = cube(p);
        }
    }
}

/*
 * Shades a point found by ray-marching.
 */
vec3 shade( in vec3 p, in vec3 d, in vec3 e )
{
    if( length(p-e) > MAX_DEPTH ) return vec3(0);
    
    vec3 n = norm(p);
  
    return vec3(.8,.2,.2) * max(0.4, dot(-d,n));
}

/*
 * The main function of the shader.
 */
void main( void )
{
    // The texture coordinate is a good way to get a local frame of reference
    // coordinate, which we need to set up the ray-marcher's camera.
	vec2 uv = fragUV;
    
    // Transform the domain of the space to [-1, 1]
    uv = (uv*2.0)-1.0;
    
    // Flip Y.
    uv.y *= -1.0;
    
    // Now we fix the aspect ratio by multiplying it by the
    // dimensions of the Tile, extracted from the transform matrix.
	uv.x *= transform[0][0]/transform[1][1];
    
    // Create variables for the ray  position, ray direction, and eye coordinate.
    vec3 p, d, e;
    
    // Set up the ray position and direction.
    camera(uv, vec3(0, 2, -3), normalize(vec3(0, -2, 3)), 1.0, p, d);
    
    // Store the initial position as the eye coordinate.
    e = p;
    
    // Do the actual march.
    march(p, d);
           
    // Set the color of the fragment.
	gl_FragColor = vec4(shade(p,d,e),1.0);
}
#version 120

// Interpolated texture coordinates.
varying vec2 fragUV;

// The first pass' framebuffer.
uniform sampler2D fwdFB;

// The second pass' framebuffer.
uniform sampler2D defFB;

void main(void)
{
    // Sample the textures and mix based on alpha. That's it!
    vec4 fwd = texture2D(fwdFB, fragUV);
    vec4 def = texture2D(defFB, fragUV);
    gl_FragColor = vec4(fwd.xyz, 1);
}
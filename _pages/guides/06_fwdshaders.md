---
layout: featuredoc
title: Writing FwdTile Shaders
featuretitle: Writing FwdTile Shaders
permalink: /guides/fwdshaders/
isShaderGuide: true
---

{::options parse_block_html="true" /}

Writing FwdTile Shaders
-----------------------
{: .floating-header}
FwdTiles require a custom shader to function. This guide describes how to write one.

Expected Attribute Variables
----------------------------
In order for geometry to get passed into the vertex shader, you need to define the
attributes the rendering code is expecting in your shader.

| ```attribute vec3``` | ```vertPos``` | The incoming vertex position. |
| ```attribute vec2``` | ```vertUV``` | The incoming vertex texture coordinate. |

Provided Uniform Variables
--------------------------
The FwdTile rendering code will try to populate the following variables, so it's best that
your shader at least exposes them.

| ```uniform mat3``` | ```transform``` | The transformation matrix. Since we wish to allow these vertex shaders to optionally redefine how scrolling works, the transformation only contains position and scale. |
| ```uniform vec2``` | ```camera``` | The X and Y coordinates of the camera. |
| ```uniform float``` | ```pFactor``` | The factor by which scrolling is throttled to create parallax comes from a LUT and is passed in via this uniform. |
| ```uniform float ``` | ```ignoreScroll``` | Whether or not it has been specified CPU-side that this Tile shall ignore camera scrolling. |
| ```uniform float``` | ```depth``` | A depth value that corresponds with the current rendering plane to allow for z-ordering and cullling. |
| ```uniform int``` | ```hFlip``` | 1 when horizontal texture flipping is desired CPU-side, 0 otherwise. |
| ```uniform int``` | ```vFlip``` | 1 when vertical texture flipping is desired CPU-side, 0 otherwise. |
| ```uniform float``` | ```time``` | The time since GLFW was initialized. |
| ```uniform vec2``` | ```resolution``` | The resolution of the framebuffer being rendered into. |

Additionally the fragment shader has access to the four custom textures.

| ```uniform sampler2D``` | ```texA``` | The first custom texture. |
| ```uniform sampler2D``` | ```texB``` | The second custom texture. |
| ```uniform sampler2D``` | ```texC``` | The third custom texture. |
| ```uniform sampler2D``` | ```texD``` | The fourth custom texture. |

The Vertex Shader
-----------------
Since this is a custom shader, it goes without saying that you need to implement all the
GPU-side features expected by the Tile class.

- <p class='li-text'>Texture flipping</p>
- <p class='li-text'>Parallax scrolling</p>
- <p class='li-text'>Scroll ignore</p>

Here are some examples describing how to do so.

<h3>Texture Flipping</h3>
A quick but not very fast function that illustrates the basic behavior of texture
flipping could be:

```glsl
void setupTextureFlip(inout uv)
{
    if( hFlip > 0 ) fragUV.x = 1.0-fragUV.x;
    if( vFlip > 0 ) fragUV.y = 1.0-fragUV.y;
}
```

Where the texture coordinate is passed in, and the original variable is modified
to suit the whims of texture flipping. However this code uses some branching, and
as GPUs don't enjoy branching code, we should try to get rid of it.

```glsl
void setupTextureFlip(inout uv)
{
    fragUV.x = mix( fragUV.x, 1.0-fragUV.x, float(hFlip) );
    fragUV.y = mix( fragUV.y, 1.0-fragUV.y, float(vFlip) );
}
```

Now we've replaced the branching, but we can go a step further and do this as
a single vector operation rather than two scalar ones.

```glsl
void setupTextureFlip(inout uv)
{
    fragUV = mix( fragUV, 1.0-fragUV, vec2(hFlip,vFlip) );
}
```

<h3>Parallax Scrolling</h3>

To describe how parallax scrolling can be implemented it's best to look directly at the math, or rather
the transformation matrix that performs such scrolling.

![The parallax scrolling transformation matrix]({{ site.baseurl }}/assets/transmat.svg){:class="inline-image"}
*Where **Fp** is the plane-dependent parallax scrolling factor.*{:class="image-text"}

In this ***Fp*** is the *parallax factor* stored in ```pFactor```. Notice how without ***Fp*** this is
simply a matrix that scales the Tile to its size, then translates it according to its position relative
to the camera. However, ***Fp*** throttles this translation, giving the effect of parallax.

An example of how this is implemented in GLSL can be:

```glsl
mat3 parallaxSetup()
{
    // One can't edit uniforms, so we need to make a copy.
    mat3 m = transform;
    
    // Now we do the actual math to enable the parallax.
    // This boils down to essentially:
    // TilePosition = (TilePosition - CameraPosition) / ParallaxFactor
    m[2][0] = ( m[2][0] - camera.x ) * pFactor;
    m[2][1] = ( m[2][1] - camera.y ) * pFactor;
    
    return m;
}
```

This is nearly a direct translation of the matrix above. The main takeaways of this example are:

- <p class='li-text'>The camera position is stored in ```camera```</p>
- <p class='li-text'>You can't modify uniforms, so you have to make a copy.</p>
- <p class='li-text'>**Matrix access in GLSL is *column major***. This means that the
first index is the column vector, and the second is the element in that column. This contrasts with the more commonly used
row-major access, where the row is indexed prior to the column.</p>

But wait, we can vectorize it:
```glsl
mat3 parallaxSetup()
{
    // One can't edit uniforms, so we need to make a copy.
    mat3 m = transform;
    
    // Now we do the actual math to enable the parallax.
    // This boils down to essentially:
    // TilePosition = (TilePosition - CameraPosition) * ParallaxFactor
    m[2].xy = ( m[2].xy - camera ) * pFactor;
    
    return m;
}
```

<h3>Scroll Ignore</h3>

The reason for ignoring scroll is to have elements be static relative to the screen, such as with HUD elements.
Sure, you can simply ignore parallax in your FwdTile shader, but that would disconnect the CPU-side functionality
of Tile::setIgnoreScroll(), leaving you unable to enable/disable the effect if necessary. Therefore it is wise
to implement this.

To illustrate what needs to be done, I've simply wrapped the parallax setup code in a conditional branch.

```glsl
mat3 parallaxSetup()
{
    // One can't edit uniforms, so we need to make a copy.
    mat3 m = transform;
    
    // Check to see if we need to ignore scrolling.
    if(ignoreScroll  < .5)
    {
        // Now we do the actual math to enable the parallax.
        // This boils down to essentially:
        // TilePosition = (TilePosition - CameraPosition) * ParallaxFactor
        m[2].xy = ( m[2].xy - camera ) * pFactor;
    }
    
    return m;
}
```

However since we've gotten it down to a single command, it's easy to remove the branching.

```glsl
mat3 parallaxSetup()
{
    // One can't edit uniforms, so we need to make a copy.
    mat3 m = transform;
    
    // Now we do the actual math to enable the parallax.
    // This boils down to essentially:
    // TilePosition = (TilePosition - CameraPosition) * ParallaxFactor
    m[2].xy = mix( ( m[2].xy - camera ) * pFactor, m[2].xy, ignoreScroll );
    
    return m;
}
```

<h3>Passing Variables to The Fragment Shader</h3>

Don't forget to send stuff to the fragment shader! This is done by using the ```varying``` keyword. 
Simply declare the same ```varying``` variable in both stages, and the value of the variable at the end
of the vertex shader will be interpolated and given to the fragment shader's variable of the same name.

Here's an example. First, the vertex shader.

```glsl
varying vec4 foo;
void main(void)
{
    . . .
    foo = vec4(1, 0, 1, 1);
    . . .
}
```

And now the fragment shader.

```glsl
varying vec4 foo;
void main(void)
{
    . . .
    gl_FragColor = foo;
    // gl_FragColor now is vec4(1, 0, 1, 1);
}
```

In short, don't forget to give the fragment shader things like texture coordinates.

```glsl
. . .
attribute vec2 vertUV;
. . .
varying vec2 fragUV;
void main(void)
{
    // Incoming vertex position will be interpolated across
    // the primitive and given to the fragment's fragUV.
    fragUV = vertUV;
}
```

The Fragment Shader
-------------------

Unlike the fragment shaders of DefTiles, there's very few requirements of a FwdTile shader. Just don't forget to
assign a value to ```gl_FragColor```.

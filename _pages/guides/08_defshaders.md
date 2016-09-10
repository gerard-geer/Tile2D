---
layout: featuredoc
title: Writing DefTile Shaders
featuretitle: Writing DefTile Shaders
permalink: /guides/defshaders/
isShaderGuide: true
---

{::options parse_block_html="true" /}

Writing DefTile Shaders
-----------------------
{: .floating-header}
DefTiles also require a custom shader to function. This guide describes how to write it.

*If you haven't read the [FwdTile shader documentation](/Tile2D/guides/fwdshaders/), it is recommended
that you do so, as it goes into greater depth in regards to some optimizations and derivations.

Expected Attribute Variables
----------------------------
In order for geometry to get passed into the vertex shader, you need to define the
attributes the rendering code is expecting in your shader.

| ```attribute vec3``` | ```vertPos``` | The incoming vertex position. |
| ```attribute vec2``` | ```vertUV``` | The incoming vertex texture coordinate. |

Provided Uniform Variables
--------------------------
The DefTile rendering code will try to write to the following uniform variables, so it's best that
your shader at least defines them. (Though unused uniforms are optimized away anyhow.)

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

In addition to the above uniforms shared with FwdTiles, DefTiles get two other uniforms.

| ```uniform sampler2D``` | ```fwdColor``` | The contents of the color target of the forward rendering pass' framebuffer. |
| ```uniform sampler2D``` | ```fwdDepth``` | The contents of the depth target of the forward rendering pass' framebuffer. |

The Vertex Shader
-----------------
Since this is a custom shader, it goes without saying that you need to implement all the
GPU-side features expected by the Tile class.

- <p class='li-text'>Texture flipping</p>
- <p class='li-text'>Parallax scrolling</p>
- <p class='li-text'>Scroll ignore</p>

Here are some examples describing how to do so.

<h3>Texture Flipping</h3>

Here's the optimized solution discussed in the [FwdTile shader guide](/Tile2D/guides/fwdshaders/),

```glsl
void setupTextureFlip(inout uv)
{
    fragUV = mix( fragUV, 1.0-fragUV, vec2(hFlip,vFlip) );
}
```

<h3>Parallax Scrolling</h3>

Once again let us look at the parallax transformation matrix:

![The parallax scrolling transformation matrix]({{ site.baseurl }}/assets/transmat.svg){:class="inline-image"}
*Where **Fp** is the plane-dependent parallax scrolling factor.*{:class="image-text"}

***Fp*** throttles Tile translation relative to both its location and the camera, giving the 
effect of parallax.

Here is the optimized example from the [FwdTile shader guide](/Tile2D/guides/fwdshaders/):

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
Sure, you can simply ignore parallax in your DefTile shader, but that would disconnect the CPU-side functionality
of Tile::setIgnoreScroll(), leaving you unable to enable/disable the effect if necessary. Therefore it is wise
to implement this.

Here's the optimized version from [FwdTile shader guide](/Tile2D/guides/fwdshaders/), Notice that we're merely
modifying ```parallaxSetup()``` from the prior topic. This allows us to bundle uniform functionality away in an
initialization function.

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

As always, on't forget to send stuff to the fragment shader! This is done by using the ```varying``` keyword. 
Simply declare the same ```varying``` variable in both stages, and the value of the variable at the end
of the vertex shader will be interpolated and given to the fragment shader's variable of the same name.

Essentially, don't forget to give the fragment shader things like texture coordinates. (You can send other
stuff too.)

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

DefTile fragment shaders have duty upon them.

- <p class='li-text'>Depth Testing</p>

<h3>Depth Testing in The Fragment Shader</h3>

First, let me answer the question you may be asking: Why are we doing depth testing ourselves?
The forward and deferred rendering passes have independent depth buffers so that Z culling can
be altered or skipped by Tile2D users. However, this means that OpenGL at large is unaware of
the depth values of the forward pass when drawing DefTiles in the deferred pass. 

This means you have to manually compare each fragment with its analog in the forward pass should
you want your DefTiles to be depth tested against the Tiles of the forward pass. Don't worry though:
testing is still done as usual between DefTiles.

Another important note is that the compositor depends on how your depth-test goes, and how failures
are represented. The compositor mixes between the forward and deferred pass based on the alpha of
the deferred pass' color buffer. **Therefore in order to specify that you want the fragment of the
forward buffer to be displayed on the screen, you must output an alpha value of zero.** 

Additionally The clear color for the deferred framebuffer also has an alpha of zero, so unwritten
portions of the buffer are ignored just as well.

Here is an example of how to do such depth testing:

```glsl
bool depthTest()
{
    vec2 uv = gl_FragCoord.xy / resolution.xy;
    float depth = texture2D(fwdDepth, uv).r;
    return gl_FragCoord.z < depth;
}
```

But as we've established, branching in GLSL is not a good idea--and this function forces us to either
use it or typecast. Let's change the signature and remove the up-front comparison to rectify the situation.

```glsl
float depthTest()
{
    vec2 uv = gl_FragCoord.xy / resolution.xy;
    float depth = texture2D(fwdDepth, uv).r;
    return step(gl_FragCoord.z, depth);
}
```

Now we no longer force the rest of the program to use boolean values, and also removed the top-level
comparison from the function by using ```mix()```.  However, if you're doing anything in screen space,
you'll likely already have computed ```uv``` somewhere. So why do the same work twice? I've also inlined
the call to texture2D since the returned value is only used once.

```glsl
float depthTest(vec2 uv)
{
    return step( gl_FragCoord.z, texture2D(fwdDepth,uv).r );
}
```

Also, if you're using the depth value *anywhere else*, it's best to have one texture call and pass the
value around. In general, consider variable reuse when writing shaders.
---
layout: page
title: Design
navtitle: Design
permalink: /design/
---

Design
------
{: .floating-header}
Tile2D is a deferred rendering engine that draws elements called *Tiles* to a GLFW window
managed by the engine. The GLFW context used by Tile2D targets OpenGL 2.1 with GL_FRAMEBUFFER_EXT
in order to be compatible with the maximimum number of devices that have a programmable
pipeline and framebuffer support. 

Resource Sharing
----------------
In the spirit of how older hardware (such as the NES or Genesis) allowed for reuse and tiling
of tiles without consequence, texture objects and shaders are stored in an *AssetManager* to be
shared amongst all Tile2D Tiles. Not only that, only one copy of the vertex data used to draw
each Tile is kept in memory and is simply reused. This keeps the memory complexity of drawing
several Tiles that share resources well below _O(n)_. 
![Tiles share assets]({{ site.baseurl }}/assets/tiles_share_assets.svg){:class="inline-image"}
*Every Tile gets its GPU assets from a shared resource, and can share those GPU assets with
one another.*{:class="image-text"}

Parallax Scrolling
------------------
Tiles can exist on one of 10 different parallax planes. There are five planes extending into
the screen, three that share nearly the same position in the middleground, and two foreground
planes. Scrolling is done in the fragment shader by taking the camera and tile positions' contribution
to the 2D transformation matrix and throttling it by a constant based upon the Tile's rendering
plane. This moves parallax plane scrolling--often a non-negligibly complex CPU-side task--to the
vertex shader, which is often under-utilized in 2D games.
![The parallax scrolling transformation matrix]({{ site.baseurl }}/assets/transmat.svg){:class="inline-image"}
*Where Fp is the plane-dependent parallax scrolling factor. This curtails the movements made by
either the camera or the Tile itself.*{:class="image-text"}

Tile Types
----------
There are five types of Tiles: **BGTiles**, **SceneTiles**, **AnimTiles**, **FwdTiles**, 
and **DefTiles**. The first three are basic work-out-of-the-box types that you can simply create
and draw. They exist to minimize extra work done for a given purpose.

**BGTiles** are guaranteed to be opague, and are locked to the background plane. That allows us to
take shortcuts in their shader code as well as when sorting Tiles for drawing. Since they're opague,
we can take advantage of z-buffer testing to avoid drawing a majority of each BGTile when other
Tiles lie in front.

**SceneTiles** add a little bit of complexity. They can be transparent, and are also free to exist on any
plane. This adds a little bit of complexity to the shader code, and also requires a bit more
testing when z-ordering. However they are still less complex to draw than Anim-, Fwd- or DefTiles.

**AnimTiles** animate their textures, stepping through several frames contained within it at a constant
but configurable interval. As you might imagine, there is more to do on the GPU for AnimTiles.
FwdTiles are fully programmable Tiles. They feature custom shaders that have access to global time,
framecount, Tile position and size, and four specifiable textures. These can become very complex
Tiles, and because of uniform variable and texture overhead, even the simplest FwdTile is more consuming
to draw than any Tile type discussed yet. Additionally, for maximum programmability another responsibility
is accrued: The camera and parallax scrolling must be done by the shader programmer in the vertex stage.
(If desired.)

**DefTiles** are the golden child of the Tiles. They're the same as FwdTiles with one key difference: They're
drawn to their own framebuffer after every other Tile has been drawn, and have access to the color and
depth buffer of that first rendering pass. This allows them to do post-processing effects on the rest of
the pass. However, since it is likely that a post-tile might want to insert itself into the scene
alongside the Tiles of the first pass, the depth-test must be done in the fragment shader.

Rendering
---------
Rendering a Tile2D frame is a multi-step process. First, only Tiles added to the **Renderer**'s rendering
queue are processed. When a Tile is added to the rendering queue, the entire queue is sorted to minimize
overdraw. This means all opaque Tiles are sorted from front to back, then the transparent Tiles are ordered
from back to front. At this point the entire queue is memoized for easy removal of single Tiles from
the queue.

As mentioned elsewhere, all Tiles have access to the same GPU assets. This is allowable because since
texture units might need to be swapped out each Tile, multiple Tiles can't be drawn on the same draw call.
Therefore the same geometry and potentially the same textures and shaders are shoved through the pipeline
for each Tile. (The vertex data is transformed to the position and dimensions of each Tile in the vertex
shader.)

Rendering is done in two passes. First, all Tiles which aren't DefTiles are drawn to a forward-rendered
framebuffer. The parallax plane of each Tile is aptly stored in the depth buffer, allowing for z-buffer
testing and culling. 

At this point the active framebuffer is swapped to a second buffer specifically for DefTiles, and then
all DefTiles are drawn to it. The fragment shader of a DefTile is passed the color and depth targets
of the forward framebuffer, hence allowing it to do deferred rendering work (such as reflections, lighting,
transparency effects, etc.). However, since it is not required that DefTiles be part of the original
scene, depth-testing is not performed automatically. It must be done in the fragment shader by comparing
```gl_FragCoord.z``` with the texel of the framebuffer at the same coordinate as the current fragment. If
the test fails, the fragment shader should write out an alpha of zero. There are two reasons this test
is done here rather than in the compositor:
- FwdTile fragment shaders can be very intensive. Shortcutting out before hand with such a test can save
  a lot of cycles on most modern GPUs that can handle branching code.
- As mentioned before, it is not required that DefTiles respect z-ordering (Perhaps a GUI element,
  for example.) Should depth testing be done in the compositor, it would force DefTiles to obey z-ordering.

The clear color of the framebuffer also has an alpha of zero. This makes it easy to composite the two
framebuffers together. If the alpha of the texel from the deferred framebuffer is zero, we output the
forward result, otherwise we output the deferred buffer. Simple transparency between the two can also be 
done by simply calling
```glsl
gl_FragColor = vec4( mix(fwdFB.rgb, defFB.rgb, defFB.a), 1.0 );
```

The Compositor
--------------
The way the compositor works is simple. It draws a full screen quad using the same geometry as the
tiles, and with it takes both color buffers, does the above comparison, and outputs the final result.
However, a user can specify a custom shader in the AssetManager to override this functionality with their
own. In this case, the shader receives extra inputs: the resolutions of the framebuffers and the screen,
and the time since startup. This facilitates several effects with ease, such as CRT-blur or the Gunstar Heroes
teleportation effect, among other things.

![The general data-path of the renderer.]({{ site.baseurl }}/assets/renderer_architecture.svg){:class="inline-image"}
*An overview of the inner workings of the rendering engine.*{:class="image-text"}
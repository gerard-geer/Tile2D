---
layout: featuredoc
title: Renderer Usage
featuretitle: Renderer Usage
permalink: /guides/renderer/
---

Advanced Renderer Usage
-----------------------
{: .floating-header}

The Renderer can do much more than the Quickstart lets on. To refresh, here's a diagram describing its operation.
![The general data-path of the renderer.]({{ site.baseurl }}/assets/renderer_architecture.svg){:class="inline-image"}

Using The Renderer
------------------
Each Tile2D Window instance you create has associated with it an instance of Renderer. This Renderer instance is
initialized when the Window is initialized, so there is no setup involved. To use it simply call:

```cpp
Renderer* Window::getRenderer()
```

and store that reference somewhere for easy access. The Window should be initialized prior to calling this function.

To resize the renderer:

```cpp
bool Renderer::resize(GLuint width, GLuint height);
```

| width | The new forward pass Framebuffer width. |
| height | The new forward pass Framebuffer height. |
| Returns | Whether or not the the operation succeeded, as the framebuffers have to be entirely recreated. |

To tell the Renderer to draw a frame, call Window's update() function. The reason for this is that
Renderer's render() function takes a pointer to the governing Window instance so that it can access
and use various properties of the Window.

```cpp
void Window::update();
```

The Render Queue
----------------
The rendering queue is sorted to take full advantage of overdraw culling while still allowing
for transparency.
To do this the sorted render queue looks similar to this:

![How the sorted render queue looks sans DefTiles.]({{ site.baseurl }}/assets/sorted-forward-pass.svg){:class="inline-image"}

This diagram does not include DefTiles, however. DefTiles are stored in-situ in this list, but the
forward pass acts only upon non-DefTiles. The deferred pass does the opposite, skipping all but DefTiles.

What this means is that if your Tile has no transparent regions, it is a good idea to set the transparency
flag to false via ```Tile::setTransparency(false)``` so that the renderer can perhaps cull the image.
The renderer does offscreen culling as well.

Additionally, sorting only needs to take place when a new Tile is added to the Queue. Therefore, do not
repeatedly add and remove Tiles from the queue between frames. There's a lot of GPU memory to be had, and
culling is done to keep draw calls low. It is better to leave unseen Tiles in the queue than to trigger
a sorting and memoization.

To add a Tile to the RenderQueue, one specify the Tile type as well as a pointer to the Tile. Remember
that this triggers a sort.

```cpp
void Renderer::addToRenderQueue(tile_type type, Tile * tile);
```

| type | A tile_type describing the type of the Tile. |
| tile | The Tile to add to the render queue. |

To remove a Tile from the RenderQueue, simply call the following. True will be returned if the Tile was
found and removed. This does not delete the Tile the reference points to. That's your job. In addition,
Tile removal does not trigger a sort.

```cpp
bool Renderer::removeFromRenderQueue(Tile* tile);
```

| tile | A pointer to the Tile to be removed. |
| Returns | Whether or not the Tile was found and removed. |

To completely clear the rendering queue, call 

```cpp
void Renderer::flushRenderQueue();
```

Writing a Custom Compositor Shader
----------------------------------
The compositor is the functionality that stitches together both the forward and deferred passes into a
coherent image. The clear color of the render buffers has an alpha of zero, and when the fragment shader
of a DefTile fails its depth-test, it outputs zero as well. This greatly simplifies the logic regarding
the decision of which buffer to display for a given fragment. It's simply:

```glsl
gl_FragColor.rgb = vec4( mix(fwdFB.rgb, defFB.rgb, defFB.a), 1.0 );
```

The default shader does this quite well, but sadly, that's all it does. It's very reasonable to want
to add some sort of effects to the final image, and to allow for this, custom shaders are allowed.
When using a custom shader, these uniform variables are defined and populated:

| ```uniform sampler2D fwdFB``` | The color buffer of the forward pass'' framebuffer. |
| ```uniform sampler2D defFB``` | The color buffer of the deferred pass' framebuffer. |
| ```uniform vec2 winResolution``` | The resolution of the window. |
| ```uniform vec2 fbResolution``` | The resolution of the renderer's framebuffers. |
| ```uniform float time``` | The time since the OpenGL context was created. |

To write the shader, be sure to perform the operation listed above. Beyond that, go wild!

In order to use your custom shader it must be added to the AssetManager, then you must call:

```cpp
void Renderer::setCustomShader(char * customCompositor);
```

| customCompositor | The key of the Shader to use. |

If the shader does not exist or is removed then the Renderer will default back to the stock shader.
If it has been removed, one must call ```setCustomShader()``` again to resume use.
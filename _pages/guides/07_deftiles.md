---
layout: featuredoc
title: Using DefTiles
featuretitle: DefTiles
permalink: /guides/deftiles/
isTileGuide: true
---

Using DefTiles
-------------
{: .floating-header}

DefTiles are Tile2D's second type of fully programmable Tile: They use custom textures
with custom shaders. Not only that, they have access to the product of the forward pass,
so they can do untold amounts of stuff. Things like:

- <p class='li-text'>Reflections</p>
- <p class='li-text'>Lighting and shadows</p>
- <p class='li-text'>Ambient occlusion (though this is 2D)</p>
- <p class='li-text'>Sprite masking</p>
- <p class='li-text'>Post-processed filtering and color-grading</p>
- <p class='li-text'>Water effects</p>

... and the list goes on.


Creating a DefTile
------------------

<h3>Creating a DefTile Shader</h3>

The first step in creating a DefTile is to have a shader program for it to use. If you don't
already have one that you want it to use, write it using the 
[DefTile shader guide](/Tile2D/guides/defshaders/) for reference.

After you've written your shader, save it in a location relative to the location of your application's
executable, then create the shader and add it to the AssetManager.

```cpp
Shader * s = assetManager->addNewShader("shader-key", "path/to/vertex_shader_file", 
                                        "path/to/frag_shader_file");
```

<h3>Creating the DefTile</h3>

To create a DefTile use the static factory method in the Renderer class. Remember to use the key
to the Shader you're using with your new DefTile. Additionally, if you aren't using a texture slot,
pass in ```NULL```.

```cpp
DefTile * makeDefTile(GLfloat x, GLfloat y, tile_plane plane, GLfloat width, 
                      GLfloat height, bool normalize, char * texA, char * texB,
                      char * texC, char * texD, char * shader);
```

| ```x``` | The X position of this DefTile. |
| ```y``` | The Y position of this DefTile. |
| ```plane``` | The parallax plane to draw this DefTile to. |
| ```width``` | The width of this DefTile. |
| ```height``` | The height of this DefTile. |
| ```normalize``` | Normally X, Y, width and height are in the range [-1,1]. This parameter specifies whether or not to divide these by the framebuffer resolution in order to have a 1:1 pixel ratio. This remaps X and Y to be in the range [0, FBO resolution]. Note though, calls to Tile::set() will still evaluate in the [-1,1] range. |
| ```texA``` | The first custom texture that this DefTile uses. |
| ```texB``` | The second custom texture that this DefTile uses. |
| ```texC``` | The third custom texture that this DefTile uses. |
| ```texD``` | The fourth custom texture that this DefTile uses. |
| ```shader``` | The key to this DefTile's shader. |
| Returns | A pointer to a freshly constructed DefTile. |

Usage Notes
-----------
Just like with FwdTiles, features such as ```Tile::setIgnoreScroll()``` ```Tile::setTextureFlip()``` and the passive parallax scrolling
must be implemented GPU-side in the shader program in order to work.
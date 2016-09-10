---
layout: featuredoc
title: Using FwdTiles
featuretitle: FwdTiles
permalink: /guides/fwdtiles/
isTileGuide: true
---

Using FwdTiles
-------------
{: .floating-header}

FwdTiles are Tile2D's first type of fully programmable Tile: They use custom textures
with custom shaders. This means though that they require a bit more setup.

Creating a FwdTile
------------------

<h3>Creating a FwdTile Shader</h3>

The first step in creating a FwdTile is to have a shader program for it to use. If you don't
already have one that you want it to use, write it using the 
[FwdTile shader guide](/Tile2D/guides/fwdshaders/) for reference.

After you've written your shader, save it in a location relative to the location of your application's
executable, then create the shader and add it to the AssetManager.

```cpp
Shader * s = assetManager->addNewShader("shader-key", "path/to/vertex_shader_file", 
                                        "path/to/frag_shader_file");
```

<h3>Creating the FwdTile</h3>

To create a FwdTile use the static factory method in the Renderer class. Remember to use the key
to the Shader you're using with this FwdTile. Additionally, if you aren't using a texture slot,
pass in ```NULL```.

```cpp
FwdTile * makeFwdTile(GLfloat x, GLfloat y, tile_plane plane, GLfloat width, 
                      GLfloat height, bool normalize, char * texA, char * texB,
                      char * texC, char * texD, char * shader);
```

| ```x``` | The X position of this FwdTile. |
| ```y``` | The Y position of this FwdTile. |
| ```plane``` | The parallax plane to draw this FwdTile to. |
| ```width``` | The width of this FwdTile. |
| ```height``` | The height of this FwdTile. |
| ```normalize``` | Normally X, Y, width and height are in the range [-1,1]. This parameter specifies whether or not to divide these by the framebuffer resolution in order to have a 1:1 pixel ratio. This remaps X and Y to be in the range [0, FBO resolution]. Note though, calls to Tile::set() will still evaluate in the [-1,1] range. |
| ```texA``` | The first custom texture that this FwdTile uses. Pass in NULL to provide the shader with an empty texture if this slot is to be unused. |
| ```texB``` | The second custom texture that this FwdTile uses. Pass in NULL to provide the shader with an empty texture if this slot is to be unused. |
| ```texC``` | The third custom texture that this FwdTile uses. Pass in NULL to provide the shader with an empty texture if this slot is to be unused. |
| ```texD``` | The fourth custom texture that this FwdTile uses. Pass in NULL to provide the shader with an empty texture if this slot is to be unused. |
| ```shader``` | This FwdTile's shader. |
| Returns | A pointer to a freshly constructed FwdTile. |

Usage Notes
-----------
Features such as ```Tile::setIgnoreScroll()``` ```Tile::setTextureFlip()``` and the passive parallax scrolling
must be implemented GPU-side in the shader program in order to work.
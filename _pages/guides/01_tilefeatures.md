---
layout: featuredoc
title: Advanced Tile Features
featuretitle: Advanced Tile Features
permalink: /guides/tilefeatures/
isTileGuide: true
---

Advanced Tile Features
----------------------
{: .floating-header}
Tiles have many more features than what's described in the Quickstart.

Getting and Setting
-------------------

All Tiles' properties can be accessed and set via getters and setters. They're listed here.

```cpp
GLfloat getX() const;
```

| Returns | The X position of the Tile, in the range of [-1, 1].  |

```cpp
GLfloat getY() const;
```

| Returns | The Y position of the Tile, in the range of [-1, 1].  |

```cpp
tile_plane getPlane() const;
```

| Returns | An enum type that represents which rendering plane this Tile is assigned to. For BGTiles this will always return BG_PLANE.|

```cpp
GLfloat getWidth() const;
```

| Returns | The width of the Tile, in the range of [-1, 1].  |

```cpp
GLfloat getHeight() const;
```

| Returns | The height of the Tile, in the range of [-1, 1]. |

```cpp
bool hasTrans() const;
```

| Returns | Whether or not the Tile has transparency.  For BGTiles this will always return false. |

```cpp
bool ignoresScroll() const;
```

| Returns | Whether or not this Tile ignores camera scrolling. |

```cpp
GLuint getTextureFlip() const;
```

| Returns | The Tile's texture flipping. This int stores both horizontal and vertial flipping. Bitwise-AND it with FLIP_HORIZ or FLIP_VERT to check it. |

```cpp
GLfloat getRotation() const;
```

| Returns | How far the current Tile has been rotated, in radians. |

```cpp
BasicMatrix* getCompoundMat();
```

| Returns | A reference to a BasicMatrix that represents the product of this Tile's position and rotation matrices. |

```cpp
BasicMatrix* getPositionMat();
```

| Returns | A reference to this Tile's position matrix. |

```cpp
BasicMatrix* getRorationMatrix();
```

| Returns | A reference to this Tile's rotation matrix. |

```cpp
unsigned long getID() const;
```

| Returns | This Tile's unique ID. Used for memoization of the rendering queue. |


```cpp
void setX(GLfloat x);
```

| x | The new X position of this Tile, in the range of [-1, 1]. |

```cpp
void setY(GLfloat y);
```

| y | The new Y position of this Tile, in the range of [-1, 1]. |

```cpp
void setPlane(tile_plane plane);
```

| plane | The new rendering plane of this Tile. |

```cpp
void setWidth(GLfloat width);
```

| width | The new width of this Tile, in the range of [-1, 1].  |

```cpp
void setHeight(GLfloat height);
```

| height | The new height of this Tile, in the range of [-1, 1].  |

```cpp
void setTransparency(bool trans);
```

| trans | Whether or not this Tile has transparency. Setting this to true will tell the Renderer to take advantage of overdraw culling when drawing this Tile. |

```cpp
void setTextureFlip(GLuint flip);
```

| flip | The bitwise-coded texture flip mode. Bitwise OR zero with FLIP_HORIZ or FLIP_VERT to properly set. |

```cpp
void setRotation(GLfloat rotation);
```

| rotation | The Tile's new rotation, in radians. |

```cpp
void setIgnoreScroll(bool ignoreScroll);
```

| ignoreScroll | Whether or not this Tile ignores the scrolling of the camera. |

Setting Texture Flip
--------------------

Texture flip is set in a bitwise fashion. In order to have horizontal flip:

```cpp
tile->setTextureFlip( Tile::HORIZ_FLIP );
```

To have a vertical flip:

```cpp
tile->setTextureFlip( Tile::VERT_FLIP );
```

To do both:

```cpp
tile->setTextureFlip( Tile::HORIZ_FLIP | Tile::VERT_FLIP );
```

Using Pixel Space rather than Normalized Screen Space
-----------------------------------------------------

The default behavior of Tile2D is to use a [-1,1] positional space, where -1 is the left/bottom of the screen,
and +1 is the right/top of the screen. However, this is not always the best way to go. Sometimes being able
to specify the usage of pixel coordinates directly is advantageous. Such functionality is provided via helper-functions
in the Renderer class. 

**Note that the screen resolution considered here is that of the internal buffers of the Renderer, not that of the window.**

```cpp
GLfloat getTilePxX(Tile * tile);
```

| tile | A reference to the Tile to be modified. |
| Returns | The Tile's horizontal position in terms of pixels. |

```cpp
GLfloat getTilePxY(Tile * tile);
```

| tile | A reference to the Tile to be modified. |
| Returns | The Tile's vertical position in terms of pixels. |

```cpp
void setTilePxPos(Tile * tile, GLfloat x, GLfloat y);
```

| tile | A reference to the Tile to be modified. |
| x    | The new X position, in terms of pixels. |
| y    | The new Y position, in terms of pixels. |

```cpp
GLfloat getTilePxW(Tile * tile);
```

| tile    | A reference to the Tile to be modified. |
| Returns | The Tile's width, in terms of pixels.   |

```cpp
GLfloat getTilePxH(Tile * tile);
```

| tile    | A reference to the Tile to be modified. |
| Returns | The Tile's height, in terms of pixels.  |

```cpp
void setTilePxDim(Tile * tile, GLfloat w, GLfloat h);
```

| tile    | A reference to the Tile to be modified. |
| width   | The new width of the Tile.              |
| height  | The new height of the Tile.             |

Additionally, to work in terms of ```px``` when creating Tiles, set normalize to True when using the Renderer's Tile Factory methods.

```cpp
BGTile * Renderer::makeBGTile(GLfloat x, GLfloat y, GLfloat width, GLfloat height,
                              bool normalize, char* texture)
```

| x         | The X position of this BGTile. |
| y         | The Y position of this BGTile. |
| width     | The width of this BGTile.      |
| height    | The height of this BGTile.     |
| normalize | Normally X, Y, width and height are in the range [-1,1]. This parameter specifies whether or not to divide these by the framebuffer resolution in order to have a 1:1 pixel ratio. This remaps X and Y to be in the range [0, FBO resolution]. Note though, calls to ```Tile::set()``` will still evaluate in the [-1,1] range. |
| texture | The AssetManager key to the texture this BGTile uses. |
| Returns | A pointer to a freshly created BGTile. |

```cpp
SceneTile * makeSceneTile(GLfloat x, GLfloat y, tile_plane plane, GLfloat width,
                          GLfloat height, bool normalize, char * texture);
```

| x         | The X position of this SceneTile. |
| y         | The Y position of this SceneTile. |
| width     | The width of this SceneTile.      |
| height    | The height of this SceneTile.     |
| plane     | The rendering plane of the new SceneTile. |
| normalize | Normally X, Y, width and height are in the range [-1,1]. This parameter specifies whether or not to divide these by the framebuffer resolution in order to have a 1:1 pixel ratio. This remaps X and Y to be in the range [0, FBO resolution]. Note though, calls to ```Tile::set()``` will still evaluate in the [-1,1] range. |
| texture | The AssetManager key to the texture this SceneTile uses. |
| Returns | A pointer to a freshly created SceneTile. |

```cpp
AnimTile * makeAnimTile(GLfloat x, GLfloat y, tile_plane plane, GLfloat width,
                        GLfloat height, bool normalize, char * texture,
                        unsigned int numFrames, unsigned int framewidth,
                        unsigned int frameHeight, float frameTime);
```

| x         | The X position of this SceneTile. |
| y         | The Y position of this SceneTile. |
| width     | The width of this SceneTile.      |
| height    | The height of this SceneTile.     |
| plane     | The rendering plane of the new SceneTile. |
| normalize | Normally X, Y, width and height are in the range [-1,1]. This parameter specifies whether or not to divide these by the framebuffer resolution in order to have a 1:1 pixel ratio. This remaps X and Y to be in the range [0, FBO resolution]. Note though, calls to ```Tile::set()``` will still evaluate in the [-1,1] range. |
| texture | The AssetManager key to the texture this SceneTile uses. |
| Returns | A pointer to a freshly created SceneTile. |
| numFrames   | The number of frames held in the texture. |
| framewidth  | The width of each individual frame. |
| frameHeight | The height of each individual frame. |
| frameTime   | The duration of each frame. |
| Returns | A pointer to a freshly created AnimTile. |

```cpp
DefTile * makeDefTile(GLfloat x, GLfloat y, tile_plane plane, GLfloat width, 
                      GLfloat height, bool normalize, char * texA, char * texB,
                      char * texC, char * texD, char * shader);
```

| x | The X position of this DefTile. |
| y | The Y position of this DefTile. |
| plane | The parallax plane to draw this DefTile to. |
| width | The width of this DefTile. |
| height | The height of this DefTile. |
| normalize | Normally X, Y, width and height are in the range [-1,1]. This parameter specifies whether or not to divide these by the framebuffer resolution in order to have a 1:1 pixel ratio. This remaps X and Y to be in the range [0, FBO resolution]. Note though, calls to Tile::set() will still evaluate in the [-1,1] range. |
| texA | The first custom texture that this DefTile uses. |
| texB | The second custom texture that this DefTile uses. |
| texC | The third custom texture that this DefTile uses. |
| texD | The fourth custom texture that this DefTile uses. |
| shader | The key to this DefTile's shader. |
| Returns | A pointer to a freshly constructed DefTile. |

```cpp
FwdTile * makeFwdTile(GLfloat x, GLfloat y, tile_plane plane, GLfloat width, 
                      GLfloat height, bool normalize, char * texA, char * texB,
                      char * texC, char * texD, char * shader);
```

| x | The X position of this FwdTile. |
| y | The Y position of this FwdTile. |
| plane | The parallax plane to draw this FwdTile to. |
| width | The width of this FwdTile. |
| height | The height of this FwdTile. |
| normalize | Normally X, Y, width and height are in the range [-1,1]. This parameter specifies whether or not to divide these by the framebuffer resolution in order to have a 1:1 pixel ratio. This remaps X and Y to be in the range [0, FBO resolution]. Note though, calls to Tile::set() will still evaluate in the [-1,1] range. |
| texA | The first custom texture that this FwdTile uses. |
| texB | The second custom texture that this FwdTile uses. |
| texC | The third custom texture that this FwdTile uses. |
| texD | The fourth custom texture that this FwdTile uses. |
| shader | This FwdTile's shader. |
| Returns | A pointer to a freshly constructed FwdTile. |
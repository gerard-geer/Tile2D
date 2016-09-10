---
layout: featuredoc
title: Using AnimTiles
featuretitle: AnimTiles
permalink: /guides/animtiles/
isTileGuide: true
---

Using AnimTiles
---------------
{: .floating-header}
AnimTiles--or Animated-Tiles--are just that: Tiles that are animated. Give them a texture that contains
multiple frames of animation, set up your animation preferences, and away it goes!

Creating a AnimTile
-----------------
To create an AnimTile use the static factory method in the Renderer class.

```cpp
AnimTile* Renderer::makeAnimTile(GLfloat x, GLfloat y, tile_plane plane, GLfloat width,
                                 GLfloat height, bool normalize, char * texture,
                                 unsigned int numFrames, unsigned int framewidth,
                                 unsigned int frameHeight, float frameTime);
```

| ```x``` | The X position of the new AnimTile. |
| ```y``` | The Y position of the new AnimTile. |
| ```plane``` | The rendering plane to draw this AnimTile on. |
| ```width``` | The width of the new AnimTile. |
| ```height``` | The height of the new AnimTile. |
| ```normalize``` | When true, uses a pixel-based coordinate system for x, y, width, and height. |
| ```texture``` | The key to this AnimTile's texture in the AssetManager. |
| ```numFrames``` | How many frames of animation the texture contains. |
| ```frameWidth``` | The width of each frame, in pixels. |
| ```frameHeight``` | The height of each frame, in pixels. |
| ```frameTime``` | How long to display each frame, in seconds. |
| Returns | a pointer to a new ```AnimTile```. |

AnimTile Texture Organization
-----------------------------
![AnimTile texture organization]({{ site.baseurl }}/assets/animtile_tex_org.svg){:class="inline-image"}
*The layout of frames within an AnimTile's texture.*{:class="image-text"}

AnimTiles need their frames organized in a specific way to work right. Illustrated above is the expected
way for frames to be organized.

{::options parse_block_html="true" /}

- <p class='li-text'>Frames are stored from left to right on a single row.</p>
- <p class='li-text'>```frameHeight``` is the height of each frame.</p>
- <p class='li-text'>```frameWidth``` is the width of each frame.</p>
- <p class='li-text'>```numFrames``` is the number of frames stored in the texture.</p>

Yes, this does result in some very wide images, but restrictions--and in general, care--regarding texture
dimensions has evaporated.

Additional AnimTile Features
----------------------------
As AnimTiles are the first real extension of the base Tile class, they define a few more functions than
their parent.

To retrieve the frame duration:

```cpp
float getFrameTime() const;
```

| Returns | The amount of time to wait before iterating the current frame. |

To get the current frame index:

```cpp
unsigned int getCurFrame() const;
```

| Returns | The current frame of animation. |

To get the height of each frame:

```cpp
unsigned int getFrameHeight() const;
```

| Returns | The height of each individual frame. |

To get the width of each frame:

```cpp
unsigned int getFrameWidth() const;
```

| Returns | The width of each individual frame. |

To get the total number of frames:

```cpp
unsigned int getNumFrames() const;
```

| Returns | The number of frames in the animation. |

```cpp
To get the texture key:
char* getTexture();
```

| Returns | The key used to retrieve the texture. |

To set the duration of each frame:

```cpp
void setFrameTime(float animWait);
```

|  animWait | The new amount of time to wait between frames. |

To override the underlying animation mechanics and set the frame yourself:

```cpp
void setCurFrame(unsigned int curFrame);
```

|  curFrame | The new current frame. |

To change the total number of frames:

```cpp
void setNumFrames(unsigned int numFrames);
```

|  numFrames | The new number of frames stored in the texture. |

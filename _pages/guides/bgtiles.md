---
layout: featuredoc
title: Using BGTiles
featuretitle: Using BGTiles
permalink: /guides/bgtiles/
---

Using BGTiles
=============
{: .floating-header}
BGTiles are the most basic kind of Tile, meant to be the absolute fastest-drawn Tile, since
it's likely that they'll take up a large percentage of the screen, and are most likely to
be used.
Since they're called BGTiles for _BackGround_ Tile, these optimizations come in the form of
several restrictions and optimizations on those restrictions:
- Confined to the ```PLANE_BG``` rendering plane.
- Forced to be opague.
- Static, ```gl_FragColor = texel``` drawing.

Creating a BGTile
-----------------
To create a BGTile use the static factory method in the Renderer class.

```cpp
BGTile *tile Renderer::makeBGTile(GLfloat x, GLfloat y, GLfloat width, GLfloat height,
bool normalize, char * texture);
```

| ```x``` | The X position of the new BGTile. |
| ```y``` | The Y position of the new BGTile. |
| ```width``` | The width of the new BGTile. |
| ```height``` | The height of the new BGTile. |
| ```normalize``` | When true, uses a pixel-based coordinate system for x, y, width, and height. |
| Returns | a pointer to a new ```BGTile```. |
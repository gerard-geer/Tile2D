---
layout: featuredoc
title: Using SceneTiles
featuretitle: SceneTiles
permalink: /guides/scenetiles/
isTileGuide: true
---

Using SceneTiles
=============
{: .floating-header}
SceneTiles get their name from the idea of a "scene element." They're much like BGTiles, except they have
some restrictions lifted:
- Can be wholly or partially transparent.
- Can exist on any plane.

Creating a SceneTile
-----------------
To create a SceneTile use the static factory method in the Renderer class.

```cpp
SceneTile * Renderer::makeSceneTile(GLfloat x, GLfloat y, tile_plane plane, GLfloat width, 
                                    GLfloat height, bool normalize, const char * texture);
```

| ```x``` | The X position of the new SceneTile. |
| ```y``` | The Y position of the new SceneTile. |
| ```plane``` | The rendering plane to draw this SceneTile on. |
| ```width``` | The width of the new SceneTile. |
| ```height``` | The height of the new SceneTile. |
| ```normalize``` | When true, uses a pixel-based coordinate system for x, y, width, and height. |
| ```texture``` | The key to this SceneTile's texture in the AssetManager. |
| Returns | a pointer to a new ```SceneTile```. |
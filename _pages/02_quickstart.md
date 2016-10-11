---
layout: page
title: Quickstart
navtitle: Quickstart
permalink: /quickstart/
---

Quickstart
----------
{: .floating-header}
In just a few steps you can draw sprites to the screen. This walkthrough assumes you've downloaded the
repository and makes use of the example assets. **Note that all file paths described below must be
relative to your executable (or be  definite paths.)**

**Step 1:** Build and test. Download the source and run ```make``` to view the avaiable build options, then
build with your preferred method. Make sure you have all the [dependencies](/Tile2D/dependencies/)!

**Step 2:** Setup. Include the common header ```tile2d.h``` in your project, as well as the results of
the build (the object or library files.)

**Step 3:** Create a window! This creates a Tile2D Window, which is simply just a managed GLFW window.
Also, it's also a good idea to store local pointers to the ```Renderer``` and ```AssetManager```.

```cpp
Window w = new Window();
w->create(1000, 700, 256, 224, (char*)"Tile2D Window");
Renderer * r = w->getRenderer();
AssetManager * a = r->getAssetManager();
```

**Step 4:** Load some assets. Most Tiles need textures, and some even use custom shaders. In Tile2D,
shaders and textures are considered to be Assets. Assets are stored in a key-value structure called
an AssetManager, contained within the renderer itself. It provides factory functions for both textures
and shaders.

```cpp
a->addNewTexture("puppy", "../ExampleAssets/puppy.png");
```

**Step 5:** Create some Tiles. The Renderer contains several factory methods to easily create Tiles.
Note though that there are more types of Tiles, and more options than apparent here.

```cpp
SceneTile * st = r->makeSceneTile(.25, 0, PLANE_NEG_2, .5, .5, false, "puppy");
```

**Step 6:** Add the Tiles to the rendering queue. The rendering queue is an opaque list that
automatically sorts the Tiles when added to minimize overdraw and maximize efficiency.

```cpp
r->addToRenderQueue(BG_TILE, bg);
```

**Step 7:** Render! (In the style of GLFW)

```cpp
glfwPollEvents(); // Required for keyboard input to be seen.
r->render(&window);
glfwSwapBuffers(window.getWindow());
```
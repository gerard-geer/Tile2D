# Tile2D
A Tile-based 2D rendering engine.
---------------------------------
Ever just want a decent way to just make a window, load some sprites and blit them?
This is for you. Ever want a 2D engine that's super light, but also highly capable?
This is also for you.

Features
------------
- **PNG support.** It's a pretty good image format. Tile2D also autodetects alpha channels and optimizes on the presence
or absence of an alpha channel.
- **Targets OpenGL 2.1 (With FBO support).** Uses OpenGL 2.1 (GLSL 1.2) and its programmable pipeline for maximimum
compatibility.
- **Passive parallax scrolling.** Parallax scrolling is deferred to the vertex shader. Just set a plane and that's it.
- **Rotation.** Not too many sprite-based rendering libraries give you clear and simple rotation. (Also deferred to the vertex shader.)
- **Extensive programability.** Custom shader code can be written for FwdTiles, DefTiles and the screen compositor (what combines both passes). Even further, you can extend the base Tile class to create whatever kind of Tile you want. Further yet, GLFW, GLEW, and OpenGL are all visible. Tile2D's encapsulated GLFW and OpenGL objects can all be accessed directly. Want to set a crazy GLFW window option, or change the OpenGL state to do some wild stuff? Nothing is stopping you.
- **Deferred rendering and post processing.** With DefTiles you can write your own shader code, with access to the depth and color buffer of the forward rendering pass. Post-processed and deferred effects await!
- **Proactive downsampling** The Renderer and Window have independent resolutions. Therefore you can render at lower
resolutions such as ```256x240```(NES) or ```320x224```(Genesis) and have that great pixelated appearance.
Better yet Tiles are renderered at this resolution--not rendered then downsampled--so overdraw is kept to a minimum.
- **Screenspace and normalized coordinate systems.** You can work in terms of pixels _or_ [-1,1].
- **Automatic optimization.** Add Tiles in whatever order you want. They're sorted each frame to minimize overdraw and
maximize culling.
- **Simplified window management.** Fullscreen rendering and resolution changing and OpenGL state management are all handled
behind the scenes. Want to go fullscreen or change resolution (of the window or framebuffers)? They're single function
calls.
- **Profiling and debugging.** Tile2D can be built with several debug options that send extra output to ```stdout```. Want to see what Tiles are drawn each frame? What are your render times? The properties of the textures you've loaded? What the current screen and buffer resolutions are? This and more can be specified to be displayed.

Quickstart
----------
In just a few steps you can draw sprites to the screen.

**Step 0:** Setup. If you're on Linux they should be available through your package manager. If you'd prefer to build things yourself, just follow their directions and use your linking instincts. If you're on Windows, download and run their installers.

**Step 1:** Build. Download the source and run ```make``` to view the avaiable build options, then
build with your preferred method. Make sure you have all the dependencies! (Mentioned above, listed below.)

**Step 2:** Include. Include the common header ```tile2d.h``` in your project, as well as the results of
the build.

**Step 3:** Create a window! This creates a Tile2D Window, which is simply just a managed GLFW window. Also, it's also a good idea to store local pointers to the ```Renderer``` and ```AssetManager```.
```c++
Window w = new Window();
w->create(1000, 700, 256, 224, (char*)"Tile2D Window");
Renderer * r = w->getRenderer();
AssetManager * a = r->getAssetManager();
```

**Step 4:** Load some assets. Most Tiles need textures, and some even use custom shaders. In Tile2D, shaders and textures are
considered to be Assets. Assets are stored in a key-value structure called an AssetManager, contained within the renderer itself.
```c++
a->addNewTexture("puppy", "../ExampleAssets/puppy.png");
```
**Step 5:** Create some Tiles. The Renderer contains several factory methods to easily create Tiles. Note though that there are more types of Tiles, and more options than apparent here.
```c++
SceneTile * st = r->makeSceneTile(.25, 0, PLANE_NEG_2, .5, .5, false, "puppy");
```
**Step 6:** Add the Tiles to the rendering queue. The rendering queue is an opaque list that automatically sorts the Tiles
when added to minimize overdraw and maximize efficiency.
```c++
r->addToRenderQueue(BG_TILE, bg);
```
**Step 7:** Render! (In the style of GLFW)
```c++
glfwPollEvents();
glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
r->render(&window);
glfwSwapBuffers(window.getWindow());
++framecount;
```

Dependencies
--------------
- **LibPNG** for image reading.
- **GLFW** for OpenGL context creation.
- **GLEW** for OpenGL extension wrangling.
- **OpenGL 2.1 with ```GL_ext_framebuffer```** so we can have a programmable pipeline and framebuffer objects.
- **Python** (2 or 3) for the script that consolidates shader code into a header file.



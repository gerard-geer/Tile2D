# Tile2D
A Tile-based 2D rendering engine.
---------------------------------
Ever just want a decent way to just make a window, load some sprites and blit them?
This is for you. Ever want a 2D engine that's super light, but also highly capable?
This is also for you.

Quickstart
----------
In just six steps you can draw sprites to the screen.

**Step 1:** Setup. Include common header. Put ```/Assets``` one directory above where your executable will go, and don't forget to link against ```-lglfw -lGL -lGLU -lpng -lGLEW```
```c++
#include "tile2d.h"
```
**Step 2:** Create a window. It's important to note that the Window contains
a Renderer instance that does the actual rendering. It uses two FBOs, whose
resolution is independent of the Window itself. Therefore you'll see two
resolutions specified.
```c++
Window window = Window();
// Window::create(winH, winV, fboH, fboV, title)
window.create(1280, 800, 256, 240, (char*)"Tile2D Window");
```
**Step 3:** Add some stuff to the Renderer's Asset Manager. (This is where all graphics
        related Assets (Shaders and Textures) are stored, so they can be shared across
        Tiles.
```c++
Renderer* r = window.getRenderer();
r->getAssetManager()->addNewTexture((char*)"puppy", (char*)"path_to_puppy_picture.png");
r->getAssetManager()->addNewTexture((char*)"kitten", (char*)"path_to_kitten_picture.png");
r->getAssetManager()->addNewTexture((char*)"fish", (char*)"path_to_fish_picture.png");
r->getAssetManager()->addNewTexture((char*)"yumetarou", (char*)"path_to_yumetarou_frames_texture.png");
r->getAssetManager()->addNewShader((char*)"exPosTileShader", (char*)"path_to_vert_shader.vert",
                                                           (char*)"path_to_frag_shader.frag");
```
**Step 4:** Make some Tiles. Generally Tiles take a position, rendering plane, size, and Texture.
```c++
BGTile * bg = r->makeBGTile(0,0,1.5,1.5,false,"puppy");
PostTile * pt = r->makePostTile(.025, .25, PLANE_POS_1, .5, .5, false, (char*)"puppy", (char*)"kitten", NULL, NULL, (char*)"example_post_shader");
SceneTile * st = r->makeSceneTile(.25, 0, PLANE_NEG_2, .5, .5, false, (char*)"fish");
AnimTile * at = r->makeAnimTile(100.0, 40.0, PLANE_PLAYFIELD_A, 16.0, 20.0, true, (char*)"yumetarou", 6, 16, 20, 1.0/20.0);
```
**Step 5:** Add those tiles to the Renderer's rendering queue.
```c++
r->addToRenderQueue(BG_TILE, bg);
r->addToRenderQueue(POST_TILE, pt);
r->addToRenderQueue(SCENE_TILE, st);
r->addToRenderQueue(ANIM_TILE, at);
```
**Step 6:** Render! (In the style of GLFW)
```c++
glfwPollEvents();
glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
r->render(&window);
r->getCamera()->setX(sin(glfwGetTime()));
glfwSwapBuffers(window.getWindow());
++framecount;
```
Features
------------
- **PNG support.** It's a pretty good image format. Tile2D also autodetects alpha channels and optimizes on the presence
or absence of an alpha channel.
- **Targets OpenGL 2.1 (With FBO support).** Uses OpenGL 2.1 (GLSL 1.2) and its programmable pipeline for maximimum
compatibility.
- **Passive parallax scrolling.** Parallax scrolling is deferred to the vertex shader. Just set a plane and that's it.
- **Deferred rendering and post processing.** With PostTiles you can have your own vertex and fragment shader to do whatever you want with.
- **Proactive downsampling** The Renderer and Window have independent resolutions. Therefore you can render at lower
resolutions such as ```256```x```240```(NES) or ```320```x```224```(Genesis) and have that great pixelated appearance.
Better yet Tiles are renderered at this resolution--not rendered then downsampled--so overdraw is kept to a minimum.
- **Screenspace and normalized coordinate systems.** You can work in terms of pixels _or_ [-1,1].
- **Automatic optimization.** Add Tiles in whatever order you want. They're sorted each frame to minimize overdraw and
maximize culling.
- **Simplified window management.** Fullscreen rendering and resolution changing and OpenGL state management are all handled
behind the scenes. Want to go fullscreen or change resolution (of the window or framebuffers)? They're single function
calls.
- **Extensible.** PostTiles are programmable. GLFW, GLEW, and OpenGL are all visible. Tile2D's encapsulated GLFW and OpenGL objects can all be accessed directly. Want to set a crazy GLFW window option, or change the OpenGL state to do some wild stuff? Nothing is stopping you.

Dependencies
--------------
- **LibPNG** for image reading.
- **GLFW** for OpenGL context creation.
- **GLEW** for OpenGL extension wrangling.
- **OpenGL 2.1 with ```GL_ext_framebuffer```** so we can have a programmable pipeline and framebuffer objects.



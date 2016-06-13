# Tile2D
A Tile-based 2D rendering engine.
=================================
Ever just want a decent way to just make a window, load some sprites and blit them?
This is for you. Ever want a 2D engine that's super light, but also highly capable?
This is also for you.

Quickstart
----------
Step 1: Include common header.
```c++
#include "tile2d.h"
```
Step 2: Create a window. It's important to note that the Window contains
a Renderer instance that does the actual rendering. It uses two FBOs, whose
resolution is independent of the Window itself. Therefore you'll see two
resolutions specified.
```c++
Window window = Window();
// Window::create(winH, winV, fboH, fboV, title)
window.create(1280, 800, 256, 240, (char*)"Tile2D Window");
```
Step 3: Add some stuff to the Renderer's Asset Manager. (This is where all graphics
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
Step 4: Make some Tiles. Generally Tiles take a position, rendering plane, size, and Texture.
```c++
BGTile * bg = r->makeBGTile(0,0,1.5,1.5,false,"puppy");
PostTile * pt = r->makePostTile(.025, .25, PLANE_POS_1, .5, .5, false, (char*)"puppy", (char*)"kitten", NULL, NULL, (char*)"example_post_shader");
SceneTile * st = r->makeSceneTile(.25, 0, PLANE_NEG_2, .5, .5, false, (char*)"fish");
AnimTile * at = r->makeAnimTile(100.0, 40.0, PLANE_PLAYFIELD_A, 16.0, 20.0, true, (char*)"yumetarou", 6, 16, 20, 1.0/20.0);
```
Step 5: Add those tiles to the Renderer's rendering queue.
```c++
r->addToRenderQueue(BG_TILE, bg);
r->addToRenderQueue(POST_TILE, pt);
r->addToRenderQueue(SCENE_TILE, st);
r->addToRenderQueue(ANIM_TILE, at);
```
Step 6: Render! (In the style of GLFW)
```c++
glfwPollEvents();
glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
r->render(&window);
r->getCamera()->setX(sin(glfwGetTime()));
glfwSwapBuffers(window.getWindow());
++framecount;
```
                                                          

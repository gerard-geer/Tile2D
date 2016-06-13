#include <iostream>
#include <cmath>
#include "tile2d.h"

using namespace std;

int main(int argc, char **argv)
{
    Window window = Window();
    window.create(1000, 716, 256, 240, (char*)"WOO");
    Renderer * r = window.getRenderer();

    r->getAssetManager()->addNewTexture((char*)"pups", (char*)"/home/gerard/Pictures/mrpups.png");
    
    r->getAssetManager()->addNewTexture((char*)"kitten", (char*)"/home/gerard/Pictures/kitten512.png");
    
    r->getAssetManager()->addNewTexture((char*)"travis", (char*)"/home/gerard/Pictures/travis.PNG");
    
    r->getAssetManager()->addNewTexture((char*)"yumetarou", (char*)"/home/gerard/Pictures/yumetarou_frame16x20.png");
    
    r->getAssetManager()->addNewShader((char*)"example_post_shader", (char*)"../Assets/Rendering Assets/Shaders/ex_post_tile_shader.vert",
                                                           (char*)"../Assets/Rendering Assets/Shaders/ex_post_tile_shader.frag");
    BGTile * bg = r->makeBGTile(0,0,1.5,1.5,false,(char*)"pups");
    r->addToRenderQueue(BG_TILE, bg);
    
    PostTile * pt = r->makePostTile(.025, .25, PLANE_POS_1, .5, .5, false, (char*)"pups", (char*)"kitten", NULL, NULL, (char*)"example_post_shader");
    r->addToRenderQueue(POST_TILE, pt);
    
    SceneTile * st = r->makeSceneTile(.25, 0, PLANE_NEG_2, .5, .5, false, (char*)"travis");
    r->addToRenderQueue(SCENE_TILE, st);
    
    AnimTile * at = r->makeAnimTile(100.0, 40.0, PLANE_PLAYFIELD_A, 16.0, 20.0, true, (char*)"yumetarou", 6, 16, 20, 1.0/20.0);
    r->addToRenderQueue(ANIM_TILE, at);
    
    GLuint framecount = 0;
    while(!glfwWindowShouldClose(window.getWindow()))
    {
        glfwPollEvents();
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        r->render(&window);
        r->getCamera()->setX(sin(glfwGetTime()));
        glfwSwapBuffers(window.getWindow());
        ++framecount;
    }
    return 0;
}
    
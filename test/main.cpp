#include <iostream>
#include <cmath>
#include "tile2d.h"

using namespace std;

int main(int argc, char **argv)
{
    Window window = Window();
    window.create(1000, 700, 256, 224, (char*)"WOO");
    Renderer * r = window.getRenderer();
    AssetManager * a = r->getAssetManager();
    a->addNewTexture((char*)"puppy", (char*)"../Assets/Rendering Assets/Textures/puppy.png");
    
    a->addNewTexture((char*)"kitten", (char*)"../Assets/Rendering Assets/Textures/kitten.png");
    
    a->addNewTexture("fish", "../Assets/Rendering Assets/Textures/fish.png");
    
    a->addNewTexture((char*)"yumetarou", (char*)"../Assets/Rendering Assets/Textures/yumetarou_frame16x20.png");
    
    a->addNewShader((char*)"example_post_shader", (char*)"../Assets/Rendering Assets/Shaders/ex_post_tile_shader.vert",
                                                           (char*)"../Assets/Rendering Assets/Shaders/ex_post_tile_shader.frag");
    BGTile * bg = r->makeBGTile(0.0, 0.0, 1.5, 1.5, false, "puppy");
    r->addToRenderQueue(BG_TILE, bg);
    
    PostTile * pt = r->makePostTile(.025, .25, PLANE_POS_1, .5, .5, false, (char*)"kitten", (char*)"kitten", NULL, NULL, (char*)"example_post_shader");
    r->addToRenderQueue(POST_TILE, pt);
    
    SceneTile * st = r->makeSceneTile(.25, 0, PLANE_NEG_2, .5, .5, false, (char*)"fish");
    r->addToRenderQueue(SCENE_TILE, st);
    
    AnimTile * at = r->makeAnimTile(100.0, 40.0, PLANE_PLAYFIELD_A, 16.0, 20.0, true, (char*)"yumetarou", 6, 16, 20, 1.0/20.0);
    r->addToRenderQueue(ANIM_TILE, at);

    while(!glfwWindowShouldClose(window.getWindow()))
    {
        at->setRotation(at->getRotation()+.01);
        window.update();
    }
    window.destroy();
    return 0;
}
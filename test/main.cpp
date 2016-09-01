#include <iostream>
#include <cmath>
#include "tile2d.h"
#include "shader_source.h"
using namespace std;

int main(int argc, char **argv)
{
    Window window = Window();
    window.create(1000, 700, 256, 224, (char*)"WOO");
    Renderer * r = window.getRenderer();
    AssetManager * a = r->getAssetManager();
    
    a->addNewTexture("puppy", "../ExampleAssets/puppy.png");
    
    a->addNewTexture("kitten", "../ExampleAssets/kitten.png");
    
    a->addNewTexture("fish", "../ExampleAssets/fish.png");
    
    a->addNewTexture("yumetarou", "../ExampleAssets/yumetarou_frame16x20.png");
    
    a->addNewShader("example_def_shader", "../ExampleAssets/ex_def_tile_shader.vert",
                                                           "../ExampleAssets/ex_def_tile_shader.frag");
                                                           
    a->addNewShader("example_cust_comp_shader", "../ExampleAssets/ex_compositor_shader.vert",
                                                "../ExampleAssets/ex_compositor_shader.frag");
    r->setCustomShader("example_cust_comp_shader");
                                                           
    a->addNewShader("example_fwd_shader", "../ExampleAssets/ex_fwd_tile_shader.vert",
                                                           "../ExampleAssets/ex_fwd_tile_shader.frag");
    BGTile * bg = r->makeBGTile(0.0, 0.0, 1.5, 1.5, false, "puppy");
    r->addToRenderQueue(BG_TILE, bg);
    
    DefTile * dt = r->makeDefTile(.025, .25, PLANE_POS_1, .5, .5, false, "kitten", "kitten", NULL, NULL, "example_def_shader");
    r->addToRenderQueue(DEF_TILE, dt);
    
    FwdTile * ft = r->makeFwdTile(-.25, .25, PLANE_NEG_1, .7, .5, false, NULL, NULL, NULL, NULL, "example_fwd_shader");
    r->addToRenderQueue(FWD_TILE, ft);
    ft->setIgnoreScroll(true);
    ft->setTransparency(true);

    
    SceneTile * st = r->makeSceneTile(.25, 0, PLANE_NEG_2, .5, .5, false, "fish");
    r->addToRenderQueue(SCENE_TILE, st);
    st->setTransparency(true);
    st->setIgnoreScroll(true);
    
    AnimTile * at = r->makeAnimTile(100.0, 40.0, PLANE_PLAYFIELD_A, 16.0, 20.0, true, "yumetarou", 6, 16, 20, 1.0/20.0);
    r->addToRenderQueue(ANIM_TILE, at);

    while(!glfwWindowShouldClose(window.getWindow()))
    {
    	r->getCamera()->setPos(3.0*sin(glfwGetTime()*4.0), 0.0);
        at->setRotation(at->getRotation()+.01);
        window.update();
    }
    window.destroy();
    return 0;
}

#include <iostream>
#include <cmath>
#include "tile2d.h"
#include "shader_source.h"
using namespace std;

int main(int argc, char **argv)
{
    Window window = Window();
    window.create(1000, 600, 250, 150, (char*)"WOO");
    Renderer * r = window.getRenderer();
    AssetManager * a = r->getAssetManager();
        
    a->addNewTexture("grid", "../ExampleAssets/grid.png");
    a->addNewTexture("maingrid", "../ExampleAssets/maingrid.png");

    a->addNewShader("fwd", "../ExampleAssets/ex_fwd_tile_shader.vert",
                           "../ExampleAssets/ex_fwd_tile_shader.frag");

    a->addNewShader("def", "../ExampleAssets/ex_def_tile_shader.vert",
                           "../ExampleAssets/ex_def_tile_shader.frag");

    FwdTile * s0 = r->makeFwdTile(120,22, PLANE_BG, 16,16, true, "grid",NULL,NULL,NULL,"fwd");
    FwdTile * s1 = r->makeFwdTile(120,22, PLANE_NEG_4, 16,16, true, "grid",NULL,NULL,NULL,"fwd");
    FwdTile * s2 = r->makeFwdTile(120,22, PLANE_NEG_3, 16,16, true, "grid",NULL,NULL,NULL,"fwd");
    FwdTile * s3 = r->makeFwdTile(120,22, PLANE_NEG_2, 16,16, true, "grid",NULL,NULL,NULL,"fwd");
    FwdTile * s4 = r->makeFwdTile(120,22, PLANE_NEG_1, 16,16, true, "grid",NULL,NULL,NULL,"fwd");
    DefTile * s5 = r->makeDefTile(120,22, PLANE_PLAYFIELD_A, 16,16, true, "maingrid",NULL,NULL,NULL,"def");
    DefTile * s6 = r->makeDefTile(120,22, PLANE_PLAYFIELD_B, 16,16, true, "maingrid",NULL,NULL,NULL,"def");
    DefTile * s7 = r->makeDefTile(120,22, PLANE_PLAYFIELD_C, 16,16, true, "maingrid",NULL,NULL,NULL,"def");
    FwdTile * s8 = r->makeFwdTile(120,22, PLANE_POS_1, 16,16, true, "grid",NULL,NULL,NULL,"fwd");
    FwdTile * s9 = r->makeFwdTile(120,22, PLANE_POS_2, 16,16, true, "grid",NULL,NULL,NULL,"fwd");

    FwdTile * t0 = r->makeFwdTile(120,62, PLANE_BG, 16,16, true, "grid",NULL,NULL,NULL,"fwd");
    FwdTile * t1 = r->makeFwdTile(120,62, PLANE_NEG_4, 16,16, true, "grid",NULL,NULL,NULL,"fwd");
    FwdTile * t2 = r->makeFwdTile(120,62, PLANE_NEG_3, 16,16, true, "grid",NULL,NULL,NULL,"fwd");
    FwdTile * t3 = r->makeFwdTile(120,62, PLANE_NEG_2, 16,16, true, "grid",NULL,NULL,NULL,"fwd");
    FwdTile * t4 = r->makeFwdTile(120,62, PLANE_NEG_1, 16,16, true, "grid",NULL,NULL,NULL,"fwd");
    FwdTile * t5 = r->makeFwdTile(120,62, PLANE_PLAYFIELD_A, 16,16, true, "maingrid",NULL,NULL,NULL,"fwd");
    FwdTile * t6 = r->makeFwdTile(120,62, PLANE_PLAYFIELD_B, 16,16, true, "maingrid",NULL,NULL,NULL,"fwd");
    FwdTile * t7 = r->makeFwdTile(120,62, PLANE_PLAYFIELD_C, 16,16, true, "maingrid",NULL,NULL,NULL,"fwd");
    FwdTile * t8 = r->makeFwdTile(120,62, PLANE_POS_1, 16,16, true, "grid",NULL,NULL,NULL,"fwd");
    FwdTile * t9 = r->makeFwdTile(120,62, PLANE_POS_2, 16,16, true, "grid",NULL,NULL,NULL,"fwd");
    
    r->addToRenderQueue(FWD_TILE, s0);
    r->addToRenderQueue(FWD_TILE, s1);
    r->addToRenderQueue(FWD_TILE, s2);
    r->addToRenderQueue(FWD_TILE, s3);
    r->addToRenderQueue(FWD_TILE, s4);
    r->addToRenderQueue(DEF_TILE, s5);
    r->addToRenderQueue(DEF_TILE, s6);
    r->addToRenderQueue(DEF_TILE, s7);
    r->addToRenderQueue(FWD_TILE, s8);
    r->addToRenderQueue(FWD_TILE, s9);
    
    r->addToRenderQueue(FWD_TILE, t0);
    r->addToRenderQueue(FWD_TILE, t1);
    r->addToRenderQueue(FWD_TILE, t2);
    r->addToRenderQueue(FWD_TILE, t3);
    r->addToRenderQueue(FWD_TILE, t4);
    r->addToRenderQueue(FWD_TILE, t5);
    r->addToRenderQueue(FWD_TILE, t6);
    r->addToRenderQueue(FWD_TILE, t7);
    r->addToRenderQueue(FWD_TILE, t8);
    r->addToRenderQueue(FWD_TILE, t9);
    
    window.setFullscreen(false);

    while(!glfwWindowShouldClose(window.getWindow()))
    {
        window.update();

        r->getCamera()->setOffY( sin(glfwGetTime())*.8 );
        r->getCamera()->setX( sin( glfwGetTime()*1.75 )*.4 );
    }
    window.destroy();
    return 0;
}

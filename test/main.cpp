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

    a->addNewShader("def", "../ExampleAssets/ex_def_tile_shader.vert",
                           "../ExampleAssets/ex_def_tile_shader.frag");

    DefTile * s0 = r->makeDefTile(120,22, PLANE_BG, 16,16, true, "grid",NULL,NULL,NULL,"def");
    DefTile * s1 = r->makeDefTile(120,22, PLANE_NEG_4, 16,16, true, "grid",NULL,NULL,NULL,"def");
    DefTile * s2 = r->makeDefTile(120,22, PLANE_NEG_3, 16,16, true, "grid",NULL,NULL,NULL,"def");
    DefTile * s3 = r->makeDefTile(120,22, PLANE_NEG_2, 16,16, true, "grid",NULL,NULL,NULL,"def");
    DefTile * s4 = r->makeDefTile(120,22, PLANE_NEG_1, 16,16, true, "grid",NULL,NULL,NULL,"def");
    DefTile * s5 = r->makeDefTile(120,22, PLANE_PLAYFIELD_A, 16,16, true, "maingrid",NULL,NULL,NULL,"def");
    DefTile * s6 = r->makeDefTile(120,22, PLANE_PLAYFIELD_B, 16,16, true, "maingrid",NULL,NULL,NULL,"def");
    DefTile * s7 = r->makeDefTile(120,22, PLANE_PLAYFIELD_C, 16,16, true, "maingrid",NULL,NULL,NULL,"def");
    DefTile * s8 = r->makeDefTile(120,22, PLANE_POS_1, 16,16, true, "grid",NULL,NULL,NULL,"def");
    DefTile * s9 = r->makeDefTile(120,22, PLANE_POS_2, 16,16, true, "grid",NULL,NULL,NULL,"def");

    DefTile * t0 = r->makeDefTile(120,62, PLANE_BG, 16,16, true, "grid",NULL,NULL,NULL,"def");
    DefTile * t1 = r->makeDefTile(120,62, PLANE_NEG_4, 16,16, true, "grid",NULL,NULL,NULL,"def");
    DefTile * t2 = r->makeDefTile(120,62, PLANE_NEG_3, 16,16, true, "grid",NULL,NULL,NULL,"def");
    DefTile * t3 = r->makeDefTile(120,62, PLANE_NEG_2, 16,16, true, "grid",NULL,NULL,NULL,"def");
    DefTile * t4 = r->makeDefTile(120,62, PLANE_NEG_1, 16,16, true, "grid",NULL,NULL,NULL,"def");
    DefTile * t5 = r->makeDefTile(120,62, PLANE_PLAYFIELD_A, 16,16, true, "maingrid",NULL,NULL,NULL,"def");
    DefTile * t6 = r->makeDefTile(120,62, PLANE_PLAYFIELD_B, 16,16, true, "maingrid",NULL,NULL,NULL,"def");
    DefTile * t7 = r->makeDefTile(120,62, PLANE_PLAYFIELD_C, 16,16, true, "maingrid",NULL,NULL,NULL,"def");
    DefTile * t8 = r->makeDefTile(120,62, PLANE_POS_1, 16,16, true, "grid",NULL,NULL,NULL,"def");
    DefTile * t9 = r->makeDefTile(120,62, PLANE_POS_2, 16,16, true, "grid",NULL,NULL,NULL,"def");
    
    r->addToRenderQueue(DEF_TILE, s0);
    r->addToRenderQueue(DEF_TILE, s1);
    r->addToRenderQueue(DEF_TILE, s2);
    r->addToRenderQueue(DEF_TILE, s3);
    r->addToRenderQueue(DEF_TILE, s4);
    r->addToRenderQueue(DEF_TILE, s5);
    r->addToRenderQueue(DEF_TILE, s6);
    r->addToRenderQueue(DEF_TILE, s7);
    r->addToRenderQueue(DEF_TILE, s8);
    r->addToRenderQueue(DEF_TILE, s9);
    
    r->addToRenderQueue(DEF_TILE, t0);
    r->addToRenderQueue(DEF_TILE, t1);
    r->addToRenderQueue(DEF_TILE, t2);
    r->addToRenderQueue(DEF_TILE, t3);
    r->addToRenderQueue(DEF_TILE, t4);
    r->addToRenderQueue(DEF_TILE, t5);
    r->addToRenderQueue(DEF_TILE, t6);
    r->addToRenderQueue(DEF_TILE, t7);
    r->addToRenderQueue(DEF_TILE, t8);
    r->addToRenderQueue(DEF_TILE, t9);
    
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

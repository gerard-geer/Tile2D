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

    AnimTile * s0 = r->makeAnimTile(120,22, PLANE_BG, 16,16, true, "grid",1,16,16,1);
    AnimTile * s1 = r->makeAnimTile(120,22, PLANE_NEG_4, 16,16, true, "grid",1,16,16,1);
    AnimTile * s2 = r->makeAnimTile(120,22, PLANE_NEG_3, 16,16, true, "grid",1,16,16,1);
    AnimTile * s3 = r->makeAnimTile(120,22, PLANE_NEG_2, 16,16, true, "grid",1,16,16,1);
    AnimTile * s4 = r->makeAnimTile(120,22, PLANE_NEG_1, 16,16, true, "grid",1,16,16,1);
    AnimTile * s5 = r->makeAnimTile(120,22, PLANE_PLAYFIELD_A, 16,16, true, "maingrid",1,16,16,1);
    AnimTile * s6 = r->makeAnimTile(120,22, PLANE_PLAYFIELD_B, 16,16, true, "maingrid",1,16,16,1);
    AnimTile * s7 = r->makeAnimTile(120,22, PLANE_PLAYFIELD_C, 16,16, true, "maingrid",1,16,16,1);
    AnimTile * s8 = r->makeAnimTile(120,22, PLANE_POS_1, 16,16, true, "grid",1,16,16,1);
    AnimTile * s9 = r->makeAnimTile(120,22, PLANE_POS_2, 16,16, true, "grid",1,16,16,1);

    AnimTile * t0 = r->makeAnimTile(120,62, PLANE_BG, 16,16, true, "grid",1,16,16,1);
    AnimTile * t1 = r->makeAnimTile(120,62, PLANE_NEG_4, 16,16, true, "grid",1,16,16,1);
    AnimTile * t2 = r->makeAnimTile(120,62, PLANE_NEG_3, 16,16, true, "grid",1,16,16,1);
    AnimTile * t3 = r->makeAnimTile(120,62, PLANE_NEG_2, 16,16, true, "grid",1,16,16,1);
    AnimTile * t4 = r->makeAnimTile(120,62, PLANE_NEG_1, 16,16, true, "grid",1,16,16,1);
    AnimTile * t5 = r->makeAnimTile(120,62, PLANE_PLAYFIELD_A, 16,16, true, "maingrid",1,16,16,1);
    AnimTile * t6 = r->makeAnimTile(120,62, PLANE_PLAYFIELD_B, 16,16, true, "maingrid",1,16,16,1);
    AnimTile * t7 = r->makeAnimTile(120,62, PLANE_PLAYFIELD_C, 16,16, true, "maingrid",1,16,16,1);
    AnimTile * t8 = r->makeAnimTile(120,62, PLANE_POS_1, 16,16, true, "grid",1,16,16,1);
    AnimTile * t9 = r->makeAnimTile(120,62, PLANE_POS_2, 16,16, true, "grid",1,16,16,1);
    
    r->addToRenderQueue(ANIM_TILE, s0);
    r->addToRenderQueue(ANIM_TILE, s1);
    r->addToRenderQueue(ANIM_TILE, s2);
    r->addToRenderQueue(ANIM_TILE, s3);
    r->addToRenderQueue(ANIM_TILE, s4);
    r->addToRenderQueue(ANIM_TILE, s5);
    r->addToRenderQueue(ANIM_TILE, s6);
    r->addToRenderQueue(ANIM_TILE, s7);
    r->addToRenderQueue(ANIM_TILE, s8);
    r->addToRenderQueue(ANIM_TILE, s9);
    
    r->addToRenderQueue(ANIM_TILE, t0);
    r->addToRenderQueue(ANIM_TILE, t1);
    r->addToRenderQueue(ANIM_TILE, t2);
    r->addToRenderQueue(ANIM_TILE, t3);
    r->addToRenderQueue(ANIM_TILE, t4);
    r->addToRenderQueue(ANIM_TILE, t5);
    r->addToRenderQueue(ANIM_TILE, t6);
    r->addToRenderQueue(ANIM_TILE, t7);
    r->addToRenderQueue(ANIM_TILE, t8);
    r->addToRenderQueue(ANIM_TILE, t9);
    
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

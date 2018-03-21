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

    SceneTile * s0 = r->makeSceneTile(120,22, PLANE_BG, 16,16, true, "grid");
    SceneTile * s1 = r->makeSceneTile(120,22, PLANE_NEG_4, 16,16, true, "grid");
    SceneTile * s2 = r->makeSceneTile(120,22, PLANE_NEG_3, 16,16, true, "grid");
    SceneTile * s3 = r->makeSceneTile(120,22, PLANE_NEG_2, 16,16, true, "grid");
    SceneTile * s4 = r->makeSceneTile(120,22, PLANE_NEG_1, 16,16, true, "grid");
    SceneTile * s5 = r->makeSceneTile(120,22, PLANE_PLAYFIELD_A, 16,16, true, "maingrid");
    SceneTile * s6 = r->makeSceneTile(120,22, PLANE_PLAYFIELD_B, 16,16, true, "maingrid");
    SceneTile * s7 = r->makeSceneTile(120,22, PLANE_PLAYFIELD_C, 16,16, true, "maingrid");
    SceneTile * s8 = r->makeSceneTile(120,22, PLANE_POS_1, 16,16, true, "grid");
    SceneTile * s9 = r->makeSceneTile(120,22, PLANE_POS_2, 16,16, true, "grid");

    SceneTile * t0 = r->makeSceneTile(120,62, PLANE_BG, 16,16, true, "grid");
    SceneTile * t1 = r->makeSceneTile(120,62, PLANE_NEG_4, 16,16, true, "grid");
    SceneTile * t2 = r->makeSceneTile(120,62, PLANE_NEG_3, 16,16, true, "grid");
    SceneTile * t3 = r->makeSceneTile(120,62, PLANE_NEG_2, 16,16, true, "grid");
    SceneTile * t4 = r->makeSceneTile(120,62, PLANE_NEG_1, 16,16, true, "grid");
    SceneTile * t5 = r->makeSceneTile(120,62, PLANE_PLAYFIELD_A, 16,16, true, "maingrid");
    SceneTile * t6 = r->makeSceneTile(120,62, PLANE_PLAYFIELD_B, 16,16, true, "maingrid");
    SceneTile * t7 = r->makeSceneTile(120,62, PLANE_PLAYFIELD_C, 16,16, true, "maingrid");
    SceneTile * t8 = r->makeSceneTile(120,62, PLANE_POS_1, 16,16, true, "grid");
    SceneTile * t9 = r->makeSceneTile(120,62, PLANE_POS_2, 16,16, true, "grid");
    
    r->addToRenderQueue(SCENE_TILE, s0);
    r->addToRenderQueue(SCENE_TILE, s1);
    r->addToRenderQueue(SCENE_TILE, s2);
    r->addToRenderQueue(SCENE_TILE, s3);
    r->addToRenderQueue(SCENE_TILE, s4);
    r->addToRenderQueue(SCENE_TILE, s5);
    r->addToRenderQueue(SCENE_TILE, s6);
    r->addToRenderQueue(SCENE_TILE, s7);
    r->addToRenderQueue(SCENE_TILE, s8);
    r->addToRenderQueue(SCENE_TILE, s9);
    
    r->addToRenderQueue(SCENE_TILE, t0);
    r->addToRenderQueue(SCENE_TILE, t1);
    r->addToRenderQueue(SCENE_TILE, t2);
    r->addToRenderQueue(SCENE_TILE, t3);
    r->addToRenderQueue(SCENE_TILE, t4);
    r->addToRenderQueue(SCENE_TILE, t5);
    r->addToRenderQueue(SCENE_TILE, t6);
    r->addToRenderQueue(SCENE_TILE, t7);
    r->addToRenderQueue(SCENE_TILE, t8);
    r->addToRenderQueue(SCENE_TILE, t9);
    
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

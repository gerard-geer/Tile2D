#include <iostream>
#include <cmath>
#include "tile2d.h"
#include "shader_source.h"
using namespace std;

int main(int argc, char **argv)
{
    Window window = Window();
    window.create(1280, 800, 256, 160, (char*)"WOO");
    Renderer * r = window.getRenderer();
    AssetManager * a = r->getAssetManager();
        
    a->addNewTexture("bg",    "../ExampleAssets/bg.png");
    a->addNewTexture("neg1",  "../ExampleAssets/neg1.png");
    a->addNewTexture("neg2",  "../ExampleAssets/neg2.png");
    a->addNewTexture("neg3",  "../ExampleAssets/neg3.png");
    a->addNewTexture("neg4",  "../ExampleAssets/neg4.png");
    a->addNewTexture("playA", "../ExampleAssets/playA.png");
    a->addNewTexture("pos1",  "../ExampleAssets/pos1.png");
    a->addNewTexture("noise", "../ExampleAssets/noise.png");
    
    a->addNewShader("wrapping-tex", "../ExampleAssets/wrapping-tex.vert",
                                    "../ExampleAssets/wrapping-tex.frag");
    a->addNewShader("crepuscular",  "../ExampleAssets/crepuscular.vert",
                                    "../ExampleAssets/crepuscular.frag");
    a->addNewShader("example_cust_comp_shader", "../ExampleAssets/ex_compositor_shader.vert",
                                                "../ExampleAssets/ex_compositor_shader.frag");
    r->setCustomShader("example_cust_comp_shader");

    FwdTile * bg = r->makeFwdTile(0, 0, PLANE_BG, 192, 160, true, "bg", NULL, NULL, NULL, "wrapping-tex");
    FwdTile * neg1 = r->makeFwdTile(0, 0, PLANE_NEG_1, 192, 160, true, "neg1", NULL, NULL, NULL, "wrapping-tex");
    FwdTile * neg2 = r->makeFwdTile(0, 0, PLANE_NEG_2, 192, 160, true, "neg2", NULL, NULL, NULL, "wrapping-tex");
    FwdTile * neg3 = r->makeFwdTile(0, 0, PLANE_NEG_3, 192, 160, true, "neg3", NULL, NULL, NULL, "wrapping-tex");
    FwdTile * neg4 = r->makeFwdTile(0, 0, PLANE_NEG_4, 192, 160, true, "neg4", NULL, NULL, NULL, "wrapping-tex");
    FwdTile * playA = r->makeFwdTile(0, 0, PLANE_PLAYFIELD_A, 192, 160, true, "playA", NULL, NULL, NULL, "wrapping-tex");
    FwdTile * playC = r->makeFwdTile(0, 0, PLANE_PLAYFIELD_C, 192, 160, true, "pos1", NULL, NULL, NULL, "wrapping-tex");
    DefTile * crep = r->makeDefTile(0, 0, PLANE_NEG_2, 192, 160, true, "noise", NULL, NULL, NULL, "crepuscular");
    bg->setIgnoreScroll(true);
    neg1->setIgnoreScroll(true);
    neg2->setIgnoreScroll(true);
    neg3->setIgnoreScroll(true);
    neg4->setIgnoreScroll(true);
    playA->setIgnoreScroll(true);
    playC->setIgnoreScroll(true);
    crep->setIgnoreScroll(true);
    
    r->addToRenderQueue(FWD_TILE, bg);
    r->addToRenderQueue(FWD_TILE, neg1);
    r->addToRenderQueue(FWD_TILE, neg2);
    r->addToRenderQueue(DEF_TILE, crep);
    r->addToRenderQueue(FWD_TILE, neg3);
    r->addToRenderQueue(FWD_TILE, neg4);
    r->addToRenderQueue(FWD_TILE, playA);
    r->addToRenderQueue(FWD_TILE, playC);
    
    window.setFullscreen(true);
    while(!glfwWindowShouldClose(window.getWindow()))
    {
        window.update();
        r->getCamera()->setX(glfwGetTime()*.25);
    }
    window.destroy();
    return 0;
}

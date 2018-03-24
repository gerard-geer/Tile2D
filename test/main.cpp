
#include <iostream>
#include <cmath>
#include "tile2d.h"
#include "shader_source.h"
using namespace std;
 
int main(int argc, char **argv)
{
    Window window = Window();
    window.create(1280, 800, 320, 200, (char*)"WOO");
    window.setFullscreen(true);
    Renderer * r = window.getRenderer();
    AssetManager * a = r->getAssetManager();
         
    a->addNewTexture("bg",    "../ExampleAssets/bg.png");
    a->addNewTexture("neg1",  "../ExampleAssets/neg1.png");
    a->addNewTexture("neg2",  "../ExampleAssets/neg2.png");
    a->addNewTexture("neg3",  "../ExampleAssets/neg3.png");
    a->addNewTexture("neg4",  "../ExampleAssets/neg4.png");
    a->addNewTexture("playA", "../ExampleAssets/playA.png");
    a->addNewTexture("noise", "../ExampleAssets/noise.png");
    a->addNewTexture("ground", "../ExampleAssets/ground.png");
    a->addNewTexture("pos", "../ExampleAssets/pos.png");
    a->addNewTexture("pos1",  "../ExampleAssets/pos1.png");
    a->addNewTexture("pos2",  "../ExampleAssets/pos2.png");
     
    a->addNewShader("wrapping-tex", "../ExampleAssets/wrapping-tex.vert",
                                    "../ExampleAssets/wrapping-tex.frag");
    a->addNewShader("crepuscular",  "../ExampleAssets/crepuscular.vert",
                                    "../ExampleAssets/crepuscular.frag");
    a->addNewShader("sky-bloom",  "../ExampleAssets/sky-bloom.vert",
                                  "../ExampleAssets/sky-bloom.frag");
    a->addNewShader("example_cust_comp_shader", "../ExampleAssets/ex_compositor_shader.vert",
                                                "../ExampleAssets/ex_compositor_shader.frag");
    r->setCustomShader("example_cust_comp_shader");
 
    FwdTile * bg = r->makeFwdTile(0,    16, PLANE_BG, 320,200, true, "bg", NULL, NULL, NULL, "wrapping-tex");
    FwdTile * neg1 = r->makeFwdTile(0,  16, PLANE_NEG_1, 320,200, true, "neg1", NULL, NULL, NULL, "wrapping-tex");
    FwdTile * neg2 = r->makeFwdTile(0,  16, PLANE_NEG_2, 320,200, true, "neg2", NULL, NULL, NULL, "wrapping-tex");
    FwdTile * neg3 = r->makeFwdTile(0,  16, PLANE_NEG_3, 320,200, true, "neg3", NULL, NULL, NULL, "wrapping-tex");
    FwdTile * neg4 = r->makeFwdTile(0,  16, PLANE_NEG_4, 320,200, true, "neg4", NULL, NULL, NULL, "wrapping-tex");
    FwdTile * playA = r->makeFwdTile(0, 16, PLANE_PLAYFIELD_A, 320,200, true, "playA", NULL, NULL, NULL, "wrapping-tex");
    FwdTile * playC = r->makeFwdTile(0, 16, PLANE_PLAYFIELD_B, 320,200, true, "pos1", NULL, NULL, NULL, "wrapping-tex");
    DefTile * crepA = r->makeDefTile(0, 16, PLANE_NEG_1, 320,200, true, "noise", NULL, NULL, NULL, "crepuscular");
    DefTile * crepB = r->makeDefTile(0, 16, PLANE_PLAYFIELD_A, 320,200, true, "noise", NULL, NULL, NULL, "crepuscular");
    DefTile * bloom = r->makeDefTile(0, 16, PLANE_POS_2, 320,200, true, NULL, NULL, NULL, NULL, "sky-bloom");
    FwdTile * ground = r->makeFwdTile(0, 0, PLANE_PLAYFIELD_C, 320,200, true, "ground", NULL, NULL, NULL, "wrapping-tex");
    FwdTile * fore1 = r->makeFwdTile(0, 6, PLANE_POS_1, 320,200, true, "pos", NULL, NULL, NULL, "wrapping-tex");
    FwdTile * fore2 = r->makeFwdTile(0, 16, PLANE_POS_1, 320,200, true, "pos1", NULL, NULL, NULL, "wrapping-tex");
    FwdTile * fore3 = r->makeFwdTile(0, 8, PLANE_POS_2, 320,200, true, "pos2", NULL, NULL, NULL, "wrapping-tex");
    bg->setIgnoreScroll(true);
    neg1->setIgnoreScroll(true);
    neg2->setIgnoreScroll(true);
    neg3->setIgnoreScroll(true);
    neg4->setIgnoreScroll(true);
    playA->setIgnoreScroll(true);
    playC->setIgnoreScroll(true);
    crepA->setIgnoreScroll(true);
    crepB->setIgnoreScroll(true);
    bloom->setIgnoreScroll(true);
    ground->setIgnoreScroll(true);
    fore1->setIgnoreScroll(true);
    fore2->setIgnoreScroll(true);
    fore3->setIgnoreScroll(true);
     
    r->addToRenderQueue(FWD_TILE, bg);
    r->addToRenderQueue(FWD_TILE, neg1);
    r->addToRenderQueue(FWD_TILE, neg2);
    r->addToRenderQueue(DEF_TILE, crepA);
    r->addToRenderQueue(DEF_TILE, crepB);
    r->addToRenderQueue(FWD_TILE, neg3);
    r->addToRenderQueue(FWD_TILE, neg4);
    r->addToRenderQueue(FWD_TILE, playA);
    r->addToRenderQueue(FWD_TILE, playC);
    r->addToRenderQueue(FWD_TILE, ground);
    r->addToRenderQueue(DEF_TILE, bloom);
    r->addToRenderQueue(DEF_TILE, fore1);
    //r->addToRenderQueue(DEF_TILE, fore2);
    r->addToRenderQueue(DEF_TILE, fore3);

    r->getCamera()->setOffY((-(32.0/160.0))*1.25);
     
    while(!glfwWindowShouldClose(window.getWindow()))
    {
        window.update();
        r->getCamera()->setX(glfwGetTime()*-.25);
    }
    window.destroy();
    return 0;
}

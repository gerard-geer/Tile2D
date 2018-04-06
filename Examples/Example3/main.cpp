
#include <iostream>
#include <cmath>
#include "tile2d.h"
#include "shader_source.h"
using namespace std;
 
int main(int argc, char **argv)
{
    Window window = Window();
    window.create(640, 400, 320, 200, (char*)"WOO");
    window.setFullscreen(false);
    Renderer * r = window.getRenderer();
    AssetManager * a = r->getAssetManager();
         
    a->addNewTexture("bg",    "../assets/tex/sunnybg.png");
    a->addNewTexture("mask",    "../assets/tex/sunnybgmask.png");
    a->addNewTexture("neg1",  "../assets/tex/neg1.png");
    a->addNewTexture("neg2",  "../assets/tex/neg2.png");
    a->addNewTexture("neg3",  "../assets/tex/neg3.png");
    a->addNewTexture("neg4",  "../assets/tex/neg4.png");
    a->addNewTexture("playA", "../assets/tex/playA.png");
    a->addNewTexture("noise", "../assets/tex/noise.png");
    a->addNewTexture("ground", "../assets/tex/ground.png");
    a->addNewTexture("pos", "../assets/tex/pos.png");
    a->addNewTexture("pos1",  "../assets/tex/pos1.png");
    a->addNewTexture("pos2",  "../assets/tex/pos2.png");
     
    a->addNewShader("godrays",  "../assets/shaders/godrays.vert",
                                "../assets/shaders/godrays.frag");
    a->addNewShader("wrapping-tex", "../assets/shaders/wrapping-tex.vert",
                                    "../assets/shaders/wrapping-tex.frag");
    a->addNewShader("example_cust_comp_shader", "../assets/shaders/ex_compositor_shader.vert",
                                                "../assets/shaders/ex_compositor_shader.frag");
    r->setCustomShader("example_cust_comp_shader");
 
    BGTile * bg = r->makeBGTile(0,   0, 320,200, true, "bg");
    FwdTile * neg1 = r->makeFwdTile(0,  16, PLANE_NEG_1, 320,200, true, "neg1", NULL, NULL, NULL, "wrapping-tex");
    FwdTile * neg2 = r->makeFwdTile(0,  16, PLANE_NEG_2, 320,200, true, "neg2", NULL, NULL, NULL, "wrapping-tex");
    FwdTile * neg3 = r->makeFwdTile(0,  16, PLANE_NEG_3, 320,200, true, "neg3", NULL, NULL, NULL, "wrapping-tex");
    FwdTile * neg4 = r->makeFwdTile(0,  16, PLANE_NEG_4, 320,200, true, "neg4", NULL, NULL, NULL, "wrapping-tex");
    FwdTile * playA = r->makeFwdTile(0, 16, PLANE_PLAYFIELD_A, 320,200, true, "playA", NULL, NULL, NULL, "wrapping-tex");
    FwdTile * playC = r->makeFwdTile(0, 16, PLANE_PLAYFIELD_B, 320,200, true, "pos1", NULL, NULL, NULL, "wrapping-tex");
    FwdTile * ground = r->makeFwdTile(0, 0, PLANE_PLAYFIELD_C, 320,200, true, "ground", NULL, NULL, NULL, "wrapping-tex");
    FwdTile * fore1 = r->makeFwdTile(0, 6, PLANE_POS_1, 320,200, true, "pos", NULL, NULL, NULL, "wrapping-tex");
    FwdTile * fore2 = r->makeFwdTile(0, 8, PLANE_POS_2, 320,200, true, "pos2", NULL, NULL, NULL, "wrapping-tex");
    DefTile * godrays = r->makeDefTile(0, 16, PLANE_POS_1, 320,200, true, "bg", "mask", NULL, NULL, "godrays");
    
    bg->setIgnoreScroll(true);
    neg1->setIgnoreScroll(true);
    neg2->setIgnoreScroll(true);
    neg3->setIgnoreScroll(true);
    neg4->setIgnoreScroll(true);
    playA->setIgnoreScroll(true);
    playC->setIgnoreScroll(true);
    godrays->setIgnoreScroll(true);
    ground->setIgnoreScroll(true);
    fore1->setIgnoreScroll(true);
    fore2->setIgnoreScroll(true);
     
    r->addToRenderQueue(BG_TILE, bg);
    r->addToRenderQueue(FWD_TILE, neg1);
    r->addToRenderQueue(FWD_TILE, neg2);
    r->addToRenderQueue(DEF_TILE, godrays);
    r->addToRenderQueue(FWD_TILE, neg3);
    r->addToRenderQueue(FWD_TILE, neg4);
    r->addToRenderQueue(FWD_TILE, playA);
    r->addToRenderQueue(FWD_TILE, playC);
    r->addToRenderQueue(FWD_TILE, ground);
    r->addToRenderQueue(DEF_TILE, fore1);
    r->addToRenderQueue(DEF_TILE, fore2);

    r->getCamera()->setOffY((-(32.0/160.0))*1.25);
     
    while(!glfwWindowShouldClose(window.getWindow()))
    {
        window.update();
        r->getCamera()->setX(glfwGetTime()*-.25);
    }
    window.destroy();
    return 0;
}

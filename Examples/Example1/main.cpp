
#include <iostream>
#include <cmath>
#include "tile2d.h"
#include "shader_source.h"
using namespace std;
 
int main(int argc, char **argv)
{
    Window window = Window();
    window.create(512, 480, 512, 480, (char*)"WOO");
    window.setFullscreen(false);
    Renderer * r = window.getRenderer();
    AssetManager * a = r->getAssetManager();
         
    a->addNewTexture("bg", "../assets/tex/clouds.png");
    a->addNewTexture("water", "../assets/tex/water.png");
    a->addNewTexture("waterbg", "../assets/tex/waterbg.png");
    a->addNewTexture("tallplat", "../assets/tex/tallplat.png");
    a->addNewTexture("longplat", "../assets/tex/longplat.png");
    a->addNewTexture("wall", "../assets/tex/wall.png");
    a->addNewTexture("ceiling", "../assets/tex/ceiling.png");
    a->addNewTexture("splash", "../assets/tex/splash.png");
    a->addNewTexture("waves", "../assets/tex/waves.png");
    a->addNewTexture("corner", "../assets/tex/corner.png");

    a->addNewShader("reflectiveWater", "../assets/shaders/reflwater.vert", "../assets/shaders/reflwater.frag");
     

    // COORDINATES
    GLfloat x,y;

    //Create the background tiles.
    SceneTile **bg = (SceneTile**)malloc(sizeof(SceneTile*)*10);
    x = -128; y = 64;
    for(int i = 0 ; i < 10; ++i)
    {
        bg[i] = r->makeSceneTile(x,64,PLANE_BG,256,512,true,"bg");
        bg[i]->setIgnoreScroll(true);
        x+=256;
    }
    for(int i = 0; i < 10; ++i) r->addToRenderQueue(SCENE_TILE,bg[i]);


    // Create the water tiles.
    AnimTile ** tpwater = (AnimTile**)malloc(sizeof(AnimTile*)*64);
    x = 80, y = 72;
    for(int i = 0; i < 4; ++i)
    {
        for(int j = 0; j < 16;  ++j)
        {
            tpwater[(i*16)+j] = r->makeAnimTile(x,y,PLANE_PLAYFIELD_A,32,32,true,"water",4,64,64,false,1.0/12.5);
            y += 32;
        }
        x += 32;
        y = 128;
    }
    for(int i = 0; i < 64; ++i) r->addToRenderQueue(ANIM_TILE,tpwater[i]);
    AnimTile ** lpwater = (AnimTile**)malloc(sizeof(AnimTile*)*128);
    x = 320, y = 128;
    for(int i = 0; i < 4; ++i)
    {
        for(int j = 0; j < 16;  ++j)
        {
            tpwater[(i*16)+j] = r->makeAnimTile(x,y,PLANE_PLAYFIELD_A,32,32,true,"water",4,64,64,false,1.0/12.5);
            y += 32;
        }
        x += 32;
        y = 128;
    }
    for(int i = 0; i < 64; ++i) r->addToRenderQueue(ANIM_TILE,tpwater[i]);

    // Create the left wall tiles.
    SceneTile ** leftwall = (SceneTile**)malloc(sizeof(SceneTile*)*11);
    y = 64;
    for(int i = 0; i < 11; ++i)
    {
        leftwall[i] = r->makeSceneTile(-32,y,PLANE_PLAYFIELD_B,64,32,true,"wall");
        y+= 32;
    }
    for(int i = 0; i < 11; ++i) r->addToRenderQueue(SCENE_TILE,leftwall[i]);
    AnimTile ** lwsplash = (AnimTile**)malloc(sizeof(AnimTile*)*2);
    x = -8;
    for(int i = 0; i < 2; ++i)
    {
        lwsplash[i] = r->makeAnimTile(x,64,PLANE_PLAYFIELD_C,32,16,true,"splash",4,32,16,false,1/12.5);
        x+=16;
    }
    for(int i = 0; i < 2; ++i) r->addToRenderQueue(ANIM_TILE,lwsplash[i]);

    // Create the right wall tiles.
    SceneTile ** rightwall = (SceneTile**)malloc(sizeof(SceneTile*)*7);
    y = 192;
    for(int i = 0; i < 7; ++i)
    {
        rightwall[i] = r->makeSceneTile(480,y,PLANE_PLAYFIELD_B,64,32,true,"wall");
        y+= 32;
    }
    for(int i = 0; i < 7; ++i) r->addToRenderQueue(SCENE_TILE,rightwall[i]);

    // Create the ceiling tiles.
    SceneTile ** ceiling = (SceneTile**)malloc(sizeof(SceneTile*)*14);
    x = 32;
    for(int i = 0; i < 14; ++i)
    {
        ceiling[i] = r->makeSceneTile(x,416,PLANE_PLAYFIELD_B,32,64,true,"ceiling");
        x+= 32;
    }
    for(int i = 0; i < 14; ++i) r->addToRenderQueue(SCENE_TILE,ceiling[i]);

    // Create the tall platform and splashes.
    SceneTile * tallplat = r->makeSceneTile(96,64,PLANE_PLAYFIELD_B,128,128,true,"tallplat");
    r->addToRenderQueue(SCENE_TILE,tallplat);
    AnimTile ** tpsplash = (AnimTile**)malloc(sizeof(AnimTile*)*17);
    x = 96;
    for(int i = 0; i < 7; ++i)
    {
        tile_plane p = (i%2==0)?PLANE_PLAYFIELD_A:PLANE_PLAYFIELD_C;
        tpsplash[i] = r->makeAnimTile(x,192,p,32,16,true,"splash",4,32,16,false,1/12.5);
        x+=16;
    }
    x = 72;
    for(int i = 0; i < 10; ++i)
    {
        tpsplash[i+7] = r->makeAnimTile(x,64,PLANE_PLAYFIELD_C,32,16,true,"splash",4,32,16,false,1/12.5);
        x+=16;
    }
    for(int i = 0; i < 17; ++i) r->addToRenderQueue(ANIM_TILE,tpsplash[i]);

    // Create the long platform.
    SceneTile * longplat = r->makeSceneTile(288,64,PLANE_PLAYFIELD_B,288,64,true,"longplat");
    r->addToRenderQueue(SCENE_TILE,longplat);
    AnimTile ** lpsplash = (AnimTile**)malloc(sizeof(AnimTile*)*22);
    x = 320;
    for(int i = 0; i < 7; ++i)
    {
        tile_plane p = (i%2==0)?PLANE_PLAYFIELD_A:PLANE_PLAYFIELD_C;
        lpsplash[i] = r->makeAnimTile(x,128,p,32,16,true,"splash",4,32,16,false,1/12.5);
        x+=16;
    }
    x = 272;
    for(int i = 0; i < 15; ++i)
    {
        lpsplash[i+7] = r->makeAnimTile(x,64,PLANE_PLAYFIELD_C,32,16,true,"splash",4,32,16,false,1/12.5);
        x+=16;
    }
    for(int i = 0; i < 22; ++i) r->addToRenderQueue(ANIM_TILE,lpsplash[i]);

    SceneTile * lcorner = r->makeSceneTile(-32,414,PLANE_PLAYFIELD_B,64,64,true,"corner");
    SceneTile * rcorner = r->makeSceneTile(480,414,PLANE_PLAYFIELD_B,64,64,true,"corner");
    r->addToRenderQueue(SCENE_TILE,lcorner);
    r->addToRenderQueue(SCENE_TILE,rcorner);

    // Create the reflective water tile.
    DefTile * water = r->makeDefTile(0,0,PLANE_PLAYFIELD_A,512, 80, true, "waves",NULL,NULL,NULL,"reflectiveWater");
    r->addToRenderQueue(DEF_TILE,water);
     
    while(!glfwWindowShouldClose(window.getWindow()))
    {
        //r->getCamera()->setX(sin(glfwGetTime()));
        window.update();
    }
    window.destroy();
    return 0;
}

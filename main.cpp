#include <iostream>
#include <cmath>
#include "tile2d.h"

using namespace std;

int main(int argc, char **argv)
{
    Window window = Window();
    window.create(1000, 716, 256, 224, (char*)"WOO");
    Renderer * r = window.getRenderer();

    /*r->getAssetManager()->addNewTexture((char*)"puppy", (char*)"../Assets/Rendering Assets/Textures/puppy.png");
    
    r->getAssetManager()->addNewTexture((char*)"kitten", (char*)"../Assets/Rendering Assets/Textures/kitten.png");
    
    r->getAssetManager()->addNewTexture((char*)"fish", (char*)"../Assets/Rendering Assets/Textures/fish.png");
    
    r->getAssetManager()->addNewTexture((char*)"yumetarou", (char*)"../Assets/Rendering Assets/Textures/yumetarou_frame16x20.png");
    
    r->getAssetManager()->addNewShader((char*)"example_post_shad
    er", (char*)"../Assets/Rendering Assets/Shaders/ex_post_tile_shader.vert",
                                                           (char*)"../Assets/Rendering Assets/Shaders/ex_post_tile_shader.frag");
    BGTile * bg = r->makeBGTile(0,0,1.5,1.5,false,"puppy");
    r->addToRenderQueue(BG_TILE, bg);
    
    PostTile * pt = r->makePostTile(.025, .25, PLANE_POS_1, .5, .5, false, (char*)"kitten", (char*)"kitten", NULL, NULL, (char*)"example_post_shader");
    r->addToRenderQueue(POST_TILE, pt);
    
    SceneTile * st = r->makeSceneTile(.25, 0, PLANE_NEG_2, .5, .5, false, (char*)"fish");
    r->addToRenderQueue(SCENE_TILE, st);
    
    AnimTile * at = r->makeAnimTile(100.0, 40.0, PLANE_PLAYFIELD_A, 16.0, 20.0, true, (char*)"yumetarou", 6, 16, 20, 1.0/20.0);
    r->addToRenderQueue(ANIM_TILE, at);*/
    r->getAssetManager()->addNewTexture("mockup", "/home/gerard/Pictures/watermockup.png");
    
    r->getAssetManager()->addNewTexture((char*)"fish", (char*)"../Assets/Rendering Assets/Textures/fish.png");
    
    r->getAssetManager()->addNewShader("reflections", "../Assets/Rendering Assets/Shaders/basicReflections.vert",
                                       "../Assets/Rendering Assets/Shaders/basicReflections.frag");
                                       
    r->getAssetManager()->addNewShader("hudmask", "../Assets/Rendering Assets/Shaders/hudmask.vert",
                                       "../Assets/Rendering Assets/Shaders/hudmask.frag");
    
    AnimTile * mk = r->makeAnimTile(0, 0, PLANE_PLAYFIELD_B, 256, 224, true, "mockup", 4, 256, 224, 1.0/4.0);
    r->addToRenderQueue(BG_TILE, mk);
    
    PostTile * rf = r->makePostTile(0, 0, PLANE_PLAYFIELD_B, 256, 56, true, NULL, NULL, NULL, NULL, "reflections");
    SceneTile * st = r->makeSceneTile(0,0, PLANE_PLAYFIELD_C, .5, .5, false, "fish");
    PostTile * hud_01 = r->makePostTile(10, 14, PLANE_PLAYFIELD_C, 164, 20, true, NULL, NULL, NULL, NULL, "hudmask");
    PostTile * hud_02 = r->makePostTile(178, 14, PLANE_PLAYFIELD_C, 20, 20, true, NULL, NULL, NULL, NULL, "hudmask");
    PostTile * hud_03 = r->makePostTile(202, 14, PLANE_PLAYFIELD_C, 20, 20, true, NULL, NULL, NULL, NULL, "hudmask");
    PostTile * hud_04 = r->makePostTile(226, 14, PLANE_PLAYFIELD_C, 20, 20, true, NULL, NULL, NULL, NULL, "hudmask");
    //PostTile * rf = r->makePostTile(0, 0, PLANE_PLAYFIELD_C, 256, 56, true, NULL, NULL, NULL, NULL, "hudmask");
    //PostTile * rf = r->makePostTile(0, 0, PLANE_PLAYFIELD_C, 256, 56, true, NULL, NULL, NULL, NULL, "hudmask");
    st->setTextureFlip(Tile::FLIP_VERT | Tile::FLIP_HORIZ);
    r->addToRenderQueue(POST_TILE, rf);
    r->addToRenderQueue(POST_TILE, st);
    r->addToRenderQueue(POST_TILE, hud_01);
    r->addToRenderQueue(POST_TILE, hud_02);
    r->addToRenderQueue(POST_TILE, hud_03);
    r->addToRenderQueue(POST_TILE, hud_04);
    
    GLuint framecount = 0;
    while(!glfwWindowShouldClose(window.getWindow()))
    {
        glfwPollEvents();
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        r->render(&window);
        r->setTileSSDim(hud_01, 164 + sin(glfwGetTime()), 20);
        //r->getCamera()->setX(sin(glfwGetTime()));
        glfwSwapBuffers(window.getWindow());
        ++framecount;
    }
    return 0;
}
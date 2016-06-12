#include <iostream>
#include <cmath>
#include "Window.h"
#include "Renderer.h"
#include "BGTile.h"
#include "SceneTile.h"
#include "AnimTile.h"
#include "PostTile.h"
#include "Shader.h"
#include "Texture.h"
#include "Framebuffer.h"

using namespace std;

int main(int argc, char **argv)
{
    Window window = Window();
    window.create(1000, 716, (char*)"WOO", 1);
    Renderer * r = window.getRenderer();
    
    Texture * pups = new Texture();
    pups->load((char*)"/home/gerard/Pictures/mrpups.png");
    r->getAssetManager()->add("pups", (Asset*)pups);
    
    Texture * kitten = new Texture();
    kitten->load((char*)"/home/gerard/Pictures/kitten512.png");
    r->getAssetManager()->add("kitten", (Asset*)kitten);
    
    Texture * travis = new Texture();
    travis->load((char*)"/home/gerard/Pictures/travis.PNG");
    r->getAssetManager()->add("travis", (Asset*)travis);
    
    Texture * yumetarou = new Texture();
    yumetarou->load((char*)"/home/gerard/Pictures/yumetarou_frame16x20.png");
    r->getAssetManager()->add("yumetarou", (Asset*)yumetarou);
    
    BGTile * bg = new BGTile();
    bg->init(0,0,1.5,1.5, "pups");
    r->addToRenderQueue(BG_TILE, bg);
    
    Shader * exPostShader = PostTile::createPostTileShader((char*)"../Shaders/ex_post_tile_shader.vert",
                                                           (char*)"../Shaders/ex_post_tile_shader.frag");
    r->getAssetManager()->add("example_post_shader", (Asset*)exPostShader);
    
    PostTile * pt = new PostTile();
    pt->init(0, 0, PLANE_PLAYFIELD_A, .5, .5, "pups", "kitten", NULL, NULL, "example_post_shader");
    r->addToRenderQueue(POST_TILE, pt);
    
    SceneTile * st = new SceneTile();
    st->init(0, .0, PLANE_NEG_2, .3, .25, false, "travis");
    r->addToRenderQueue(SCENE_TILE, st);
    
    AnimTile * at = new AnimTile();
    at->init(.4, .4, PLANE_POS_1, .6, .6, false, "yumetarou", 6, 16, 20, 1.0/20.0);
    r->addToRenderQueue(ANIM_TILE, at);
    
    GLuint framecount = 0;
    while(!glfwWindowShouldClose(window.getWindow()))
    {
        
        if(framecount == 10)
        {
            //window.setResolution(1280,800);
          //  window.setFullscreen(true);
        }
        
        /*if(framecount == 100)
        {
            window.setResolution(1024,720);
            window.setFullscreen(true);
            
        }
        
        if(framecount == 150)
        {
            window.setResolution(480,300);
            window.setFullscreen(false);
            
        }*/
        
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        r->render(&window);
        r->getCamera()->setX(sin(glfwGetTime()));
        glfwSwapBuffers(window.getWindow());
        ++framecount;
    }
    return 0;
}
    
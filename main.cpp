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
    window.create(720, 480, (char*)"WOO", 8);
    Renderer * r = window.getRenderer();
    
    if(GLEW_ARB_vertex_array_object) cout << "WE CAN DO VAOs" << endl;
    if(GLEW_ARB_texture_non_power_of_two) cout << "WE CAN DO NPOT" << endl;
    if(GLEW_EXT_framebuffer_object) cout << "WE CAN DO FBO" << endl;
    if(GLEW_ARB_framebuffer_object) cout << "WE CAN REALLY DO FBO" << endl;
    cout << glGetString(GL_VERSION) << endl;
    cout << GL_TEXTURE0 << ' ' << GL_TEXTURE1 << ' ' << GL_TEXTURE2 << endl;
    
        
    Texture * yum24 = new Texture();
    yum24->load((char*)"/home/gerard/Pictures/mrpups.png");
    r->getAssetManager()->add("yum24", (Asset*)yum24);
    
    Shader * s = new Shader();
    s = PostTile::createPostTileShader((char*)"shaders/example_post_tile_shader.vert", (char*)"shaders/example_post_tile_shader.frag");
    r->getAssetManager()->add("post_shader", (Asset*)s);
    PostTile * pt = new PostTile();
    pt->init(0,0, PLANE_NEG_2, .5, .5, NULL, NULL, NULL, NULL, "post_shader");
    
    window.setAsRenderTarget();
    
    // Texture * yum32 = new Texture();
    // yum32->load((char*)"/home/gerard/Pictures/yumetarou32bit.png");
    
    
    BGTile * bg = new BGTile();
    bg->init(0,0,1.5,1.5, "yum24");
    r->addToRenderQueue(BG_TILE, bg);
    r->addToRenderQueue(POST_TILE, pt);
    
   /* Texture * kitten = new Texture();
    kitten->load((char*)"/home/gerard/Pictures/kitten512.png");
    Texture * letter = new Texture();
    letter->load((char*)"/home/gerard/Pictures/PURPLE_C.png");
    Texture * yumetarou = new Texture();
    yumetarou->load((char*)"/home/gerard/Pictures/yumetarou32bit.png");
    cout << kitten->getWidth() << "x" << kitten->getHeight() << endl;
    
    Renderer renderer = Renderer();
    renderer.init();
    
    renderer.getAssetManager()->add("kitten_tex", (Asset*)kitten);
    renderer.getAssetManager()->add("purple_c", (Asset*)letter);
    renderer.getAssetManager()->add("yumetarou", (Asset*)yumetarou);
    
    BGTile * bgt = new BGTile();
    bgt->init(0.0,0.0,.5,.5,"kitten_tex");
    renderer.addToRenderQueue(BG_TILE, (Tile*)bgt);
    
    SceneTile * sct = new SceneTile();
    sct->init(0,0,PLANE_NEG_3, .25, .25, true, "purple_c");
    renderer.addToRenderQueue(SCENE_TILE, (Tile*)sct);
    AnimTile * ant = new AnimTile();
    ant->init(0.1, 0.0, PLANE_PLAYFIELD_B, .16, .20, false, "yumetarou", 6, 16, 20
    , .05);
    renderer.addToRenderQueue(ANIM_TILE, (Tile*)ant);*/
    
    GLuint framecount = 0;
    while(!glfwWindowShouldClose(window.getWindow()))
    {
        //window.setSSFactor((int)(abs(16*sin(glfwGetTime()))+1));
        
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
    
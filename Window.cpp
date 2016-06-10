#include "Window.h"

/**
 * @brief This is a simple placeholder key callback to guarantee any window
 *        created by this class is closable with ESC.
 * @param window The window.
 * @param key The code of the key pressed.
 * @param scancode The scancode pressed.
 * @param action How it was pressed.
 * @param mods Potential modifier keys, OR'd together if necessary.
 */
 
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if ( (key == GLFW_KEY_ESCAPE || key == GLFW_KEY_ENTER) && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

static void error_callback(int error, const char* desc)
{
    fputs(desc, stderr);
}

void Window::resize_callback(GLFWwindow * window, int width, int height)
{
    // Reset OpenGL to consider the new screen resolution.
    Window::initGLState(width, height);
}

Window::Window()
{
    // Give everything a nice friendly value to tide things over until
    // init is called.
    this->baseWindow = NULL;
    this->width = 0;
    this->height = 0;
    this->title = (char*)"GRenderer Window :)";
    this->fullscreen = false;
}

Window::~Window()
{
}

window_error Window::initGLState(unsigned int width, unsigned int height)
{
    // Set the clear color to magenta.
    glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Set the front face to CCW.
    glFrontFace(GL_CCW);
    glShadeModel(GL_FLAT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
    // Enable textures.
    glEnable(GL_TEXTURE_2D);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    // Enable alpha blending.
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_VERTEX_ARRAY);
    
    // Set up the initial projection matrix.
    glViewport(0, 0, width, height);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float aspect = (float)width/(float)height;
    glOrtho(0.0f, 1.0f*aspect, 1.0f, 0.0f, 0.0f, 1.0f);
    
    return WIN_NO_ERROR;

}

window_error Window::create(unsigned int width, unsigned int height, char* title, ss_factor ssFactor)
{
    // Store the width and height and title for later use.
    this->width = width;
    this->height = height;
    this->title = title;
    this->ssFactor = ssFactor;
    
    // Initialize GLFW.
    if(!glfwInit()) return WIN_COULD_NOT_INIT_GLFW;
    
    glfwSetErrorCallback(error_callback);
    
    // Set window hints.
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_FOCUSED, GL_TRUE);
    
    // Create the GLFW window.
    this->baseWindow = glfwCreateWindow(this->width, this->height, this->title, NULL, NULL);
    if(!this->baseWindow) return WIN_COULD_NOT_CREATE_WINDOW;
    glfwSetWindowSizeCallback(this->baseWindow, this->resize_callback);
    
    // Make the context current so we can set up some OpenGL stuff.
    // (And also initialize GLEW.)
    glfwMakeContextCurrent(this->baseWindow);
    
    // Now that we have a current OpenGL context we can initialize GLEW.
    // Otherwise we'd get the dreadful "Missing GL version" error.
    glewExperimental = GL_TRUE;
    GLenum glewErr = glewInit();
    if(glewErr != GLEW_OK) return WIN_COULD_NOT_INIT_GLFW;
    
    // Implement the base callback. Do not forget to call
    // glfwPollEvents()!
    glfwSetKeyCallback(this->baseWindow, key_callback);
    
    // At this point we should also create the renderer.
    this->renderer = NULL;
    
    // Initialize OpenGL itself.
    window_error glErr = this->initGLState(this->width, this->height);
    if(glErr) return glErr;
    
    // Initialize the renderer.
    this->renderer = new Renderer();
    bool rErr = this->renderer->init(this->width/this->ssFactor, this->height/this->ssFactor);
    return WIN_NO_ERROR;
}

void Window::setResolution(unsigned int width, unsigned int height)
{
    this->width = width;
    this->height = height;
    glfwSetWindowSize(this->baseWindow, this->width, this->height);
    this->renderer->resize(this->width/this->ssFactor, this->height/this->ssFactor);
}

void Window::setSSFactor(ss_factor ssFactor)
{
    this->ssFactor = ssFactor;
    this->renderer->resize(this->width/this->ssFactor, this->height/this->ssFactor);
}

ss_factor Window::getSSFactor()
{
    return this->ssFactor;
}

window_error Window::setFullscreen(bool fullscreen)
{
    // Don't want to waste time doing stuff we don't need to,
    // do we?
    if( this->fullscreen == fullscreen ) return WIN_NO_ERROR;
    
    // VAOs disappear when sharing context. Let's go ahead and get
    // rid of ours so it doesn't sit and rot in a gulag.
    this->renderer->destroyTileVAO();
    
    // Whether or not a window is fullscreen depends on the video mode
    // it was constructed with. Since it's not possible to change video
    // mode after creation, we have to destroy and recreate the
    // window.
    // However there is a problem with that. The OpenGL context, (the shaders
    // texture objects, etc..) is tied to the window. Destroying the window
    // destroys the context. To get around this, we create a new window
    // that is set up to share (more like siphon) context from our current
    // window. Then we simply destroy the old one, with the new having
    // inherited everything.
    GLFWwindow * newWindow = glfwCreateWindow(this->width,
                                              this->height,
                                              this->title,
                                              ((fullscreen) ? glfwGetPrimaryMonitor() : NULL),
                                              this->baseWindow);
                                              
    // If the window wasn't created successfully, we need to report an error.
    if( !newWindow ) return WIN_COULD_NOT_CREATE_WINDOW;
    
    // Destroy the old window.
    glfwDestroyWindow(this->baseWindow);
    
    // Swap 'em out.
    this->baseWindow = newWindow;
    
    // Make the new window's context current.
    glfwMakeContextCurrent(this->baseWindow);
    
    // Give it a resize callback.
    glfwSetWindowSizeCallback(this->baseWindow, Window::resize_callback);
    
    // Give it the keyboard callback.
    glfwSetKeyCallback(this->baseWindow, key_callback);
    
    // Remind OpenGL how to go about its affairs.
    this->initGLState(this->width, this->height);
    
    // Oh man, let's recreate the Tile VAO.
    this->renderer->initTileVAO();
    
    // We should probably resize the framebuffers to recreate them.
    this->renderer->resize(this->width/this->ssFactor, this->height/this->ssFactor);
    
    // Now that we've successfully actually changed the fullscreen status
    // we can say something about it.
    this->fullscreen = fullscreen;
    
    // Oh, we should probably give the all-clear.
    return WIN_NO_ERROR;
}

GLFWwindow* Window::getWindow()
{
    return this->baseWindow;
}

Renderer * Window::getRenderer()
{
    return this->renderer;
}

void Window::setAsRenderTarget()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0,0,this->width, this->height);
}

void Window::destroy()
{
    glfwDestroyWindow(this->baseWindow);
    this->renderer->destroy();
}




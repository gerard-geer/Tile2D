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

/**
 * @brief A callback used by GLFW in the event of an error.
 * @param error
 * @param desc
 */
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
    // Set the clear color to have an alpha of zero
    // so that any pixel not rendered to has an easily
    // identifiable attribute. This makes mixing the
    // two framebuffers much easier later on.
    glClearColor(0.0f, 1.0f, 1.0f, 0.0f);
    
    // Clear the depth to a very large value.
    glClearDepth(110.0f);
    
    // Set the front face to CCW.
    glFrontFace(GL_CCW);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
    // Enable textures.
    glEnable(GL_TEXTURE_2D);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    // Enable alpha blending.
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Enable alpha testing.
    glAlphaFunc(GL_GREATER, 0.005);
    glEnable(GL_ALPHA_TEST);
    
    // Enable use of vertex arrays, so that we can pack our vertex positions
    // and texture coordinates into a single OpenGL object.
    glEnable(GL_VERTEX_ARRAY);
        
    glViewport(0, 0, width, height);
    
    glfwSwapInterval(1);
    
    
    return WIN_NO_ERROR;

}

window_error Window::create(unsigned int windowW, unsigned int windowH,
                            unsigned int fbW, unsigned int fbH, char* title)
{
    #ifdef T2D_WINDOW_INFO
    std::cout << "Creating Tile2D window \""<< title << "\":" << std::endl;
    #endif
    
    // A window_error in case we need it.
    window_error e = WIN_NO_ERROR; // = 0
    
    // Store the width and height and title for later use.
    this->width = windowW;
    this->height = windowH;
    this->title = title;
    
    // Initialize GLFW.
    if(!glfwInit()) e = WIN_COULD_NOT_INIT_GLFW;
    
    if(!e) glfwSetErrorCallback(error_callback);
    
    // Set window hints.
    if(!e) glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    if(!e) glfwWindowHint(GLFW_FOCUSED, GL_TRUE);
    
    // Create the GLFW window.
    this->baseWindow = NULL;
    if(!e) this->baseWindow = glfwCreateWindow(this->width, this->height, this->title, NULL, NULL);
    if(!this->baseWindow) e = WIN_COULD_NOT_CREATE_WINDOW;
    
    // Make the context current so we can set up some OpenGL stuff.
    // (And also initialize GLEW.)
    if(!e) glfwMakeContextCurrent(this->baseWindow);
    
    // Now that we have a current OpenGL context we can initialize GLEW.
    // Otherwise we'd get the dreadful "Missing GL version" error.
    if(!e)
    {
        glewExperimental = GL_TRUE;
        GLenum glewErr = glewInit();
        if(glewErr != GLEW_OK) e = WIN_COULD_NOT_INIT_GLEW;
    }
    
    // Implement the base callback. Do not forget to call
    // glfwPollEvents()!
    if(!e) glfwSetWindowSizeCallback(this->baseWindow, this->resize_callback);
    
    // Also set our keyboard callback.
    if(!e) glfwSetKeyCallback(this->baseWindow, key_callback);
    
    // At this point we should also create the renderer.
    this->renderer = NULL;
    
    // Initialize OpenGL itself.
    if(!e) e = this->initGLState(this->width, this->height);
    
    // Initialize the renderer.
    this->renderer = NULL;
    if(!e) this->renderer = new Renderer();
    bool rSuccess = false;
    if(!e && this->renderer) rSuccess = this->renderer->init(fbW, fbH);
    e = (rSuccess)? e : WIN_COULD_NOT_INIT_RENDERER;
    
    #ifdef T2D_WINDOW_INFO
    bool fbo = ( glewIsSupported("GL_ARB_framebuffer_object") || glewIsSupported("GL_EXT_framebuffer_object") )
              && (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_UNSUPPORTED);
	float fboW = this->renderer ? this->renderer->getWidth() : -1;
	float fboH = this->renderer ? this->renderer->getHeight() : -1;
    std::cout << (e ? "  -WINDOW CREATION ERROR: " : "" ) << (e ? Window::getErrorDesc(e) : "  -No creation errors." ) << std::endl; 
    std::cout << "  -Window res:  " << this->width << "x"<<this->height << std::endl;
    std::cout << "  -FBO Res:     " << fboW << "x" << fboH << std::endl;
    std::cout << "  -OpenGL ver.: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "  -FBO support: " << (fbo?"true":"false") << std::endl;
    #endif
    
    return e;
}

void Window::setResolution(unsigned int width, unsigned int height)
{
	#ifdef T2D_WINDOW_INFO
	std::cout << "Changing window resolution to " << width << "x" << height << std::endl;
	#endif
    this->width = width;
    this->height = height;
    glfwSetWindowSize(this->baseWindow, this->width, this->height);
}

void Window::setFBResolution(unsigned int width, unsigned int height)
{
	#ifdef T2D_WINDOW_INFO
	std::cout << "Resizing internal framebuffers to " << width << "x" << height << std::endl;
	#endif
    this->renderer->resize(width, height);
}

window_error Window::setFullscreen(bool fullscreen)
{
    #ifdef T2D_WINDOW_INFO
	std::cout << "Window \"" << this->title << "\" switching to " << (fullscreen ? "fullscreen" : "windowed") << " mode." << std::endl;
	#endif
	
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
    if( !newWindow )
    {
		#ifdef T2D_WINDOW_INFO
    	std::cout << "  -ERROR: GLFW could not recreate the window upon fullscreen switching." << std::endl;
		#endif
    	return WIN_COULD_NOT_CREATE_WINDOW;
    }
    
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
    
    // Oh man, let's recreate the Tile VAO since we fried the OpenGL state.
    this->renderer->initTileVAO();
    
    // We should probably resize the framebuffers to recreate them.
    this->renderer->resize(this->renderer->getWidth(), this->renderer->getHeight());
    
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

unsigned int Window::getWidth() const
{
    return this->width;
}

unsigned int Window::getHeight() const
{
    return this->height;
}

bool Window::isFullscreen() const
{
    return this->fullscreen;
}

unsigned long Window::framecount() const
{
    return this->fc;
}

Renderer * Window::getRenderer()
{
    return this->renderer;
}

void Window::setAsRenderTarget()
{
    // Bind teh current framebuffer to zero, which tells OpenGL
    // to not actually use a user-defined framebuffer, but to
    // fall back to the window context itself.
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    // Update the OpenGL state to recognize the dimensions of
    // the window.
    glViewport(0,0,this->width, this->height);
}

void Window::update()
{
    // Make sure we poll for keyboard and such events.
    glfwPollEvents();
    
    // Clear the buffer.
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    // Tell the Renderer to do its thing.
    this->renderer->render(this);
    
    // Flip front/back buffers.
    glfwSwapBuffers(this->getWindow());
    
    // Increment the framecount.
    ++fc;
}

const char * Window::getErrorDesc(window_error e)
{
    switch(e)
    {
		case WIN_NO_ERROR: return "WIN_NO_ERROR";
		case WIN_COULD_NOT_INIT_GLFW: return "WIN_COULD_NOT_INIT_GLFW";
		case WIN_COULD_NOT_CREATE_WINDOW: return "WIN_COULD_NOT_CREATE_WINDOW";
		case WIN_COULD_NOT_INIT_GLEW: return "WIN_COULD_NOT_INIT_GLEW";
		case WIN_COULD_NOT_INIT_RENDERER: return "WIN_COULD_NOT_INIT_RENDERER";
		default: return "Undefined error.";
    }
}

void Window::destroy()
{
    if(fullscreen) this->setFullscreen(false);
    this->renderer->destroy();
    glfwDestroyWindow(this->baseWindow);
}




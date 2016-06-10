#ifndef TEST_WINDOW_H
#define TEST_WINDOW_H

#define GLFW_INCLUDE_GLU
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "Renderer.h"

/**
 * @class Window
 * @author Gerard Geer
 * @date 05/17/16
 * @file Window.h
 * @brief This class encapsulates the creation and destruction
 *        of a GLFW window.
 */

/*
 * Error codes:
 * WIN_NO_ERROR: No error occured.
 * WIN_COULD_NOT_INIT_GLFW: GLFW itself couldn't be initialized.
 * WIN_COULD_NOT_CREATE_WINDOW The window could not be created.
 *     Perhaps some hard-constraint window hints couldn't be
 *     satisfied?
 * WIN_COULD_NOT_INIT_GLEW: GLEW could not be initialized.
 */
enum window_error
{
    WIN_NO_ERROR,
    WIN_COULD_NOT_INIT_GLFW,
    WIN_COULD_NOT_CREATE_WINDOW,
    WIN_COULD_NOT_INIT_GLEW,
    WIN_COULD_NOT_INIT_RENDERER
};

/*
 * We don't have to render the scene at full pixel density.
 * We can cut that down so that you can have a nice, pixelated
 * view. ***Note that these are values >=1 that are used as
 * follows:
 * framebuffer_resolution = (window_resolution)/ss_factor;
 */
typedef unsigned int ss_factor;

class Window
{
private:

    /*
     * The width of the window.
     */
    unsigned int width;
    
    /*
     * The height of the window.
     */
    unsigned int height;
    
    /*
     * The subsampling factor used to make the framebuffers of the
     * renderer a lower resolution than the window.
     */
    ss_factor ssFactor;
    
    /*
     * The title of the window.
     */
    char * title;
    
    /*
     * Whether or not the window is currently fullscreen.
     */
    bool fullscreen;
    
    /*
     * the underlying GLFWwindow.
     */
    GLFWwindow * baseWindow;
    
    /*
     * The window's renderer.
     */
    Renderer * renderer;
    
    /**
     * @brief Initializes various OpenGL facets.
     * @return A window_error, if any.
     */
    static window_error initGLState(unsigned int width, unsigned int height);
    
    /**
     * @brief A callback called upon window resize. This resets OpenGL's viewport
     *        to the new resolution.
     * @param window
     * @param width
     * @param height
     */
    static void resize_callback(GLFWwindow* window, int width, int height);
    
public:
    /**
     * @brief Constructor. Simply initializes class members.
     */
    Window();
    
    /**
     * @brief Destructor. Doesn't do much right now.
     */
    ~Window();
    
    /**
     * @brief Initializes GLFW and creates the window.
     * @param x Width of the window.
     * @param y Height of the window.
     * @param title The title of the window.
     * @param ssFactor The pixel density of the rendered image, relative to the window.
     */
    window_error create(unsigned int width, unsigned int height, char* title, ss_factor ssFactor);
    
    /**
     * @brief Changes the resolution of the window. This can also be used to change
     *        fullscreen resolution.
     * @param width The new width of the window.
     * @param height
     */
    void setResolution(unsigned int width, unsigned int height);
    
    /**
     * @brief Changes the subsampling factor used to pixelate the 
     *        rendering result.
     * @param ssFactor The new subsampling factor.
     */
    void setSSFactor(ss_factor ssFactor);
    
    /**
     * @brief Returns the current subsampling factor.
     * @return The current subsampling factor.
     */
    ss_factor getSSFactor();
    
    /**
     * @brief Sets the fullscreen status of this window.
     * @param fullscreen Whether or not you want this window to become fullscreen.
     * @return A window_error, if any.
     */
    window_error setFullscreen(bool fullscreen);
    
    /**
     * @brief Returns the window handle for further manipulation.
     * @return The window handle for further manipulation.
     */
    GLFWwindow * getWindow();
    
    /**
     * @brief Returns a reference to this window's Renderer.
     * @return A reference to this window's Renderer.
     */
    Renderer * getRenderer();
    
    /**
     * @brief Sets the Window's backbuffer as the current render target.
     *        (Meaning that rendering is done to the screen rather than
     *        any framebuffer.)
     */
    void setAsRenderTarget();
    
    /**
     * @brief Destroys the window, clearing its resources, and the OpenGL 
     *        context bound to it.
     */
    void destroy();
};

#endif
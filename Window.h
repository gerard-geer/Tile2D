#ifndef TEST_WINDOW_H
#define TEST_WINDOW_H

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
     * @param windowW Horizontal resolution of the window.
     * @param windowH Vertical resolution of the window.
     * @param fbW Horizontal resolution of the framebuffer.
     * @param fbH Vertical resolution of the framebuffer.
     * @param title The title of the window.
     */
    window_error create(unsigned int windowW, unsigned int windowH,
                        unsigned int fbW, unsigned int fbH, char* title);
    
    /**
     * @brief Changes the resolution of the window. This can also be used to change
     *        fullscreen resolution.
     * @param width The new width of the window.
     * @param height The new height of the window.
     */
    void setResolution(unsigned int width, unsigned int height);
    
    /**
     * @brief Changes the resolution of the underlying framebuffer.
     * @param width The new width of the window.
     * @param height The new height of the window.
     */
    void setFBResolution(unsigned int width, unsigned int height);
    
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
     * @brief Returns the width/horizontal resolution of the Window.
     * @return The width/horizontal resolution of the Window.
     */
    unsigned int getWidth() const;
    
    /**
     * @brief Returns the height/vertical resolution of the Window.
     * @return The height/vertical resolution of the Window.
     */
    unsigned int getHeight() const;
    
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
     * @brief Tells the window to draw a frame.
     */
    void update();
    
    /**
     * @brief Destroys the window, clearing its resources, and the OpenGL 
     *        context bound to it.
     */
    void destroy();
};

#endif
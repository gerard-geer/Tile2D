#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

class Framebuffer
{
private:

    /*
     * The width of the Framebuffer.
     */
    GLuint width;
    
    /*
     * The height of the Framebuffer.
     */
    GLuint height;
    
    /*
     * The handle to the Framebuffer itself.
     */
    GLuint framebuffer;
    
    /*
     * The render-to-texture texture of the Framebuffer.
     */
    GLuint renderbuffer;
    
    /*
     * The depth component of the Framebuffer.
     */
    GLuint depthbuffer;
    
public:

    /**
     * @brief Constructs a new Framebuffer. Does not initialize it.
     */
    Framebuffer();
    
    /**
     * @brief Destructs the Framebuffer. Don't forget to call destroy first!
     */
    ~Framebuffer();
    
    /**
     * @brief Initializes this Framebuffer.
     * @param width The width of the Framebuffer.
     * @param height The height of the Framebuffer.
     */
    bool init(GLuint width, GLuint height);
    
    /**
     * @brief Reinitializes this framebuffer to have a different size.
     *        Call this on window size/resolution changes with the new
     *        resolution.
     * @param width The new width of the Framebuffer.
     * @param height The new height of the Framebuffer.
     */
    bool resize(GLuint width, GLuint height);
    
    /**
     * @brief Sets this framebuffer as the current rendering target.
     */
    void setAsRenderTarget();
    
    /**
     * @brief Returns the width of this Framebuffer.
     * @return The width of this Framebuffer.
     */
    GLuint getWidth();
    
    /**
     * @brief Returns the height of this Framebuffer.
     * @return The width of this Framebuffer.
     */
    GLuint getHeight();
    
    
    /**
     * @brief Returns a handle to the current render-to-texture texture.
     * @return A handle to the current render-to-texture texture.
     */
    GLuint getRenderTexture();
    
    /**
     * @brief Returns a handle to the current depth component texture.
     * @return A handle to the current depth component texture.
     */
    GLuint getDepthTexture();
    
    /**
     * @brief Destroys all of this Framebuffer's holdings on the GPU.
     */
    void destroy();
};

#endif // FRAMEBUFFER_H

#include "Framebuffer.h"

Framebuffer::Framebuffer()
{
    this->width = 0;
    this->height = 0;
    this->framebuffer = 0;
    this->renderbuffer = 0;
    this->depthbuffer = 0;
}

Framebuffer::~Framebuffer()
{
}

const char * getFramebufferError(GLenum e)
{
    switch(e)
    {
        case GL_FRAMEBUFFER_UNDEFINED : return "GL_FRAMEBUFFER_UNDEFINED";
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT : return "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT : return "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT"; 
        case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER : return "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER";
        case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER : return "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER";
        case GL_FRAMEBUFFER_UNSUPPORTED : return "GL_FRAMEBUFFER_UNSUPPORTED";
        case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE : return "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE";
        case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS : return "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS";
        case GL_FRAMEBUFFER_COMPLETE : return "GL_FRAMEBUFFER_COMPLETE";
        case 0 : 
        {   // Oh snap an error occured when calling glCheckFramebufferStatus()
            int error = glGetError();
            switch(error)
            {
                case GL_INVALID_ENUM : return "GL_INVALID_ENUM";
                case GL_INVALID_OPERATION : return "GL_INVALID_OPERATION";
                default: return "NO ERROR";
            }break;
        }
        default: return "unknown";
    }
}

bool Framebuffer::init(GLuint width, GLuint height)
{
    // First, store the width and height.
    this->width = width;
    this->height = height;
    
    // Create the color attachment.
    glGenTextures(1, &this->renderbuffer);
    glBindTexture(GL_TEXTURE_2D, this->renderbuffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, this->width, this->height,
                 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);
    
    // Create the depth attachment.
    glGenTextures(1, &this->depthbuffer);
    glBindTexture(GL_TEXTURE_2D, this->depthbuffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, this->width, this->height,
                 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
                 
    // Create the framebuffer.
    glGenFramebuffers(1, &this->framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, this->framebuffer);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
    glBindTexture(GL_TEXTURE_2D, this->renderbuffer);
    // Attach the color texture as the framebuffer's color attachment.
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D, this->renderbuffer, 0);
    
    // Attach the depth texture as the framebuffer's depth attachment.
    glBindTexture(GL_TEXTURE_2D, this->depthbuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, 
                           GL_TEXTURE_2D, this->depthbuffer, 0);
    
    // Enable depth testing.
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
    // Best practice to check framebuffer completeness.
    GLenum e = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER);
    
    // Go ahead and set the render target back to the window's backbuffer.
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return e == GL_FRAMEBUFFER_COMPLETE;
}

bool Framebuffer::resize(GLuint width, GLuint height)
{
    // Delete the old stuff...
    this->destroy();
    // ...To initialize it anew.
    return this->init(width,height);
    
}

void Framebuffer::setAsRenderTarget()
{
    // Tell OpenGL to do what we want.
    glBindFramebuffer(GL_FRAMEBUFFER, this->framebuffer);
    
    // Make sure that we're rendering to the framebuffer's dimensions.
    glViewport(0,0, this->width, this->height);
    
}

GLuint Framebuffer::getWidth()
{
    return this->width;
}

GLuint Framebuffer::getHeight()
{
    return this->height;
}

GLuint Framebuffer::getRenderTexture()
{
    return this->renderbuffer;
}

GLuint Framebuffer::getDepthTexture()
{
    return this->depthbuffer;
}

void Framebuffer::destroy()
{
    // Delete everything we had on the GPU.
    glDeleteFramebuffers(1, &this->framebuffer);
    glDeleteRenderbuffers(1, &this->depthbuffer);
    glDeleteTextures(1, &this->renderbuffer);
}



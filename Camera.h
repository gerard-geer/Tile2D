#ifndef CAMERA_H
#define CAMERA_H

#include <GLFW/glfw3.h>
/**
 * @class Camera
 * @author Gerard Geer
 * @date 05/31/16
 * @file Camera.h
 * @brief This is a simple class to encapsulate a camera for the renderer.
 */
class Camera
{
private:
    /*
     * The X position of the Camera.
     */
    GLfloat x;
    /*
     * The Y position of the Camera.
     */
    GLfloat y;
public:

    /**
     * @brief Constructs the Camera.
     * @param x The x position of the Camera.
     * @param y The y position of the Camera.
     */
    Camera(GLfloat x, GLfloat y);
    
    /**
     * @brief Camera destructor. Doesn't have much to destruct.
     */
    ~Camera();
    
    /**
     * @brief Sets the X position of the Camera.
     * @param x The new X position of the Camera.
     */
    void setX(GLfloat x);
    
    /**
     * @brief Sets the Y position of the Camera.
     * @param y The new X position of the Camera.
     */
    void setY(GLfloat y);
    
    /**
     * @brief Sets the position of the Camera.
     * @param x The new X position of the Camera.
     * @param y The new Y position of the Camera.
     */
    void setPos(GLfloat x, GLfloat y);
    
    /**
     * @brief Returns the X position of the Camera.
     * @return The X position of the Camera.
     */
    GLfloat getX() const;
    
    /**
     * @brief Returns the Y position of the Camera.
     * @return The Y position of the Camera.
     */
    GLfloat getY() const;
};

#endif // CAMERA_H

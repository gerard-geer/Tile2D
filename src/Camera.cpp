#include "Camera.h"

Camera::~Camera()
{
}

Camera::Camera(GLfloat x, GLfloat y)
{
    this->x = x;
    this->y = y;
    this->offx = 0;
    this->offy = 0;
}

void Camera::setX(GLfloat x)
{
    this->x = x;
}

void Camera::setY(GLfloat y)
{
    this->y = y;
}

void Camera::setOffX(GLfloat x)
{
    this->offx = x;
}

void Camera::setOffY(GLfloat y)
{
    this->offy = y;
}

void Camera::setPos(GLfloat x, GLfloat y)
{
    this->x = x;
    this->y = y;
}

void Camera::setOffset(GLfloat x, GLfloat y)
{
    this->offx = x;
    this->offy = y;
}

GLfloat Camera::getX() const
{
    return this->x;
}

GLfloat Camera::getY() const
{
    return this->y;
}

GLfloat Camera::getOffX() const
{
    return this->offx;
}

GLfloat Camera::getOffY() const
{
    return this->offy;
}

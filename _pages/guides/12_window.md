---
layout: featuredoc
title: The Tile2D Window
featuretitle: The Tile2D Window
permalink: /guides/window/
isComponentGuide: true
---

The Window
----------
{: .floating-header}

The Tile2D Window encapsulates all Tile2D functionality. It contains the Renderer, which
in turn contains the AssetManager, render queue, and compositor.

Drawing
-------

To tell the window to draw its current state to the screen, call:

```cpp
void Window::udpdate();
```

This will tell the Window's renderer to draw its render queue to the forward and deferred passes
and the compositor to combine them and draw to the screen.

Resolution
----------

One of the most important features of Tile2D is custom resolution scaling. The internal
framebuffers to which Tiles are drawn have a resolution that's independent of that of the
Window itself. Therefore you'll see that the ```create()``` requires two resolutions to be
specified, and that setting fullscreen doesn't change the rendering resolution.

<h3>Changing Window Resolution</h3>

To change the resolution of the window, call:

```cpp
void Window::setResolution(unsigned int width, unsigned int height);
```

|  ```width``` | The new width of the window. |
|  ```height``` | The new height of the window. |

This has two effects:
When in full screen mode, it simply changes the resolution the compositor outputs. When in
windowed mode, it changes the dimensions of the window.

<h3>Changing Rendering Resolution</h3>

To change the resolution of the forward and deferred framebuffers:

```cpp
void Window::setFBResolution(unsigned int width, unsigned int height);
```

| ```width``` | The new horizontal resolution of the internal framebuffers. |
| ```height``` | The new vertical resolution of the internal framebuffers. |

Like when resizing the window, this causes the framebuffers to be recreated and reinitialized, but in practice that's not
terribly expensive when working with low-resolution framebuffers.

<h3>Toggling Fullscreen Mode</h3>

To toggle between full screen and windowed modes, call:

```cpp
window_error Window::setFullscreen(bool fullscreen);
```

| ```fullscreen``` | Whether or not you want this window to become fullscreen. |
| Returns | A window_error, if any. |

Due to some implementation choices in GLFW, this forces the entire window to be destroyed
then recreated from its ashes. It's a cumbersome process, and may error out. Check the
error if things go wrong.

To know programmatically whether or not the window is currently in fullscreen mode, call:

```cpp
bool Window::isFullscreen();
```

Setting and Getting
-------------------

<h3>A Pointer to The Underlying GLFW Window</h3>
To get a pointer to the underlying window, call:

```cpp
GLFWwindow * Window::getWindow();
```

| Returns | A pointer to the underlying GLFWwindow* |

<h3>Getting Window Dimensions</h3>

```cpp
unsigned int Window::getWidth() const;
```

| Returns | The width/horizontal resolution of the Window. |
    
```cpp
unsigned int Window::getHeight() const;
```

| Returns | The height/vertical resolution of the Window. |

The Renderer
------------

To retrieve the Renderer instance governed by this Window:

```cpp
Renderer * Window::getRenderer();
```

| Returns | A pointer to the Window's personal Renderer instance. |

To learn more about Renderer, visit its [documentation.](/Tile2D/guides/renderer)

Advanced Usage
--------------

<h3> Resetting The Window as The Current Render Target</h3>

If you happened to be modifying a render target that's not the window, and need to bind back to it:

```cpp
void Window::setAsRenderTarget();
```

<h3>Getting Error Descriptions</h3>

To get a description of an error if you got one, pass it to the static function:

```cpp
static const char * Window::getErrorDesc(window_error e);
```

| ```e``` | The window_error to describe. |
| Returns | A const char* describing the error. |

<h3>Using the Framecounter</h3>

The Window has an internal framecounter that keeps track of how many frames
have been renderered.

```cpp
unsigned long Window::framecount() const;
```

| Returns | The number of frames that have been rendered, or rather the calls to ```Window::update()```.|
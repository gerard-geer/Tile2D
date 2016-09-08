---
layout: page
title: Features
navtitle: Features
permalink: /features/
---
Features
--------
{: .floating-header}
- **PNG support.** It's a pretty good image format. Tile2D also autodetects alpha
channels and optimizes on the presence or absence of an alpha channel.
- **Targets OpenGL 2.1 (With FBO support).** Uses OpenGL 2.1 (GLSL 1.2) and its
programmable pipeline for maximimum compatibility.
- **Passive parallax scrolling.** Parallax scrolling is deferred to the vertex
shader. Just set a plane and that's it.
- **Rotation.** Not too many sprite-based rendering libraries give you clear and
simple rotation. (Also deferred to the vertex shader.)
- **Extensive programability.** Custom shader code can be written for FwdTiles, 
DefTiles and the screen compositor (which is what combines both passes). Even
further, you can extend the base Tile class to create whatever kind of Tile you
want. Further yet, GLFW, GLEW, and OpenGL are all visible. Tile2D's encapsulated
GLFW and OpenGL objects can all be accessed directly. Want to set a crazy GLFW
window option, or change the OpenGL state to do some wild stuff? Nothing is
stopping you.
- **Deferred rendering and post processing.** With DefTiles you can write your own
shader code, with access to the depth and color buffer of the forward rendering pass.
Post-processed and deferred effects await!
- **Proactive downsampling** The Renderer and Window have independent resolutions. 
Therefore you can render at lower resolutions such as ```256x240```(NES) or ```320x224```
(Genesis) and have that great pixelated appearance.
Better yet Tiles are renderered at this resolution--not rendered then downsampled--so
overdraw is kept to a minimum.
- **Screenspace and normalized coordinate systems.** You can work in terms of pixels 
_or_ [-1,1].
- **Automatic optimization.** Add Tiles in whatever order you want. They're sorted each
frame to minimize overdraw and maximize culling.
- **Simplified window management.** Fullscreen rendering and resolution changing and OpenGL
state management are all handled behind the scenes. Want to go fullscreen or change
resolution (of the window or framebuffers)? They're single function calls.
- **Profiling and debugging.** Tile2D can be built with several debug options that send
extra output to ```stdout```. Want to see what Tiles are drawn each frame? What are your
render times? The properties of the textures you've loaded? What the current screen and
buffer resolutions are? This and more can be specified to be displayed.
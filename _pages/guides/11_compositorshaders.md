---
layout: featuredoc
title: Writing Compositor Shaders
featuretitle: Writing Compositor Shaders
permalink: /guides/compositorshaders/
isShaderGuide: true
---

{::options parse_block_html="true" /}

Writing Compositor Shaders
-----------------------
{: .floating-header}

The compositor is the functionality that stitches together both the forward and deferred passes into a
coherent image. The clear color of the render buffers has an alpha of zero, and when the fragment shader
of a DefTile fails its depth-test, it outputs zero as well. This greatly simplifies the logic regarding
the decision of which buffer to display for a given fragment. It's simply:

```glsl
gl_FragColor.rgb = vec4( mix(fwdFB.rgb, defFB.rgb, defFB.a), 1.0 );
```

The default shader does this quite well, but sadly, that's all it does. It's very reasonable to want
to add some sort of effects to the final image, and to allow for this, custom shaders are allowed.

Provided Uniform Variables
--------------------------

When using a custom shader, these uniform variables are defined and populated:

| ```uniform sampler2D fwdFB``` | The color buffer of the forward pass'' framebuffer. |
| ```uniform sampler2D defFB``` | The color buffer of the deferred pass' framebuffer. |
| ```uniform vec2 winResolution``` | The resolution of the window. |
| ```uniform vec2 fbResolution``` | The resolution of the renderer's framebuffers. |
| ```uniform float time``` | The time since the OpenGL context was created. |

When writing the shader, be sure to perform the composition:

```glsl
gl_FragColor.rgb = vec4( mix(fwdFB.rgb, defFB.rgb, defFB.a), 1.0 );
```

Beyond that, go wild!

In order to use your custom shader it must be added to the AssetManager, then you must call:

```cpp
void Renderer::setCustomShader(char * customCompositor);
```

| customCompositor | The key of the Shader to use. |

If the shader does not exist or is removed then the Renderer will default back to the stock shader.
If it has been removed, one must call ```setCustomShader()``` again to resume use.
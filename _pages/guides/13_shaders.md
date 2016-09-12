---
layout: featuredoc
title: Shaders in Tile2D
featuretitle: Shaders in Tile2D
permalink: /guides/shaders/
isShaderGuide: true
---

{::options parse_block_html="true" /}

Shaders in Tile2D
-----------------------
{: .floating-header}

Shaders are often a hassle. You have to load ASCII source code, send it to a secondary processing
unit, tell that unit to compile it, then tell the unit what its values should be and when to use it.

Tile2D shaders try to make things a little bit better.


Uniform Variables
--------------------------

<h3>The ShaderUniform Class</h3>

Normally one needs to remember the exact name of a uniform as well as its type in order to set it
via one of the ```glUniformXX()``` functions. 

Tile2D abstracts this away via the *ShaderUniform* class. 

ShaderUniforms encapsulate two important datums: uniform variable location and type (via an emum).

This simplifies uniform updating to a single, type agnostic function.

<h3>ShaderUniforms and Shaders</h3>

When a Shader is loaded, it's parsed to find each uniform variable in the source. For each uniform, 
a ShaderUniform is created and added to an associative array belonging to the Shader, binding the
uniform name with the appropriate ShaderUniform.

Management of this array is wrapped by the Shader class, leaving uniform variable updating up to a
single function:

```cpp
void Shader::setUniform(char * name, void * data);
```

| name | The name of the uniform variable. |
| data | A pointer to the data to populate the variable with. |

Note that the data you pass in must match the type of uniform.

| Type of Uniform | Type *data points to |
|:--------|:-------:|
| int | integer |
| float | float |
| vec2 | float* (of length two) |
| vec3 | float* (of length three) |
| vec4 | float* (of length four) |
| mat2 | float* (of length four) |
| mat3 | float* (of length nine) |
| mat4 | float* (of length 16) |

Additionally, since textures take a texture ID and a texture unit, they have their own function:

```cpp
void setTextureUniform(char * name, GLuint texID, GLuint texUnit);
```

| name | The name of the uniform variable. |
| texID | The texture ID. |
| texUnit | The texture unit to bind the texture to. |

Note that this completely abstracts away the need to match the value sent to the uniform witth
the texture unit. Simply say which texture unit, and it works.

You can also delete uniforms and check for uniform presence via:

```cpp
bool Shader::removeUniform(char * name);
```

| name | The name of the uniform to remove. |
| Returns | Whether or not this uniform was removed. If false, it didn't exist. |

```cpp
bool Shader::hasUniform(char * name);
```

| name | The name of the uniform to remove. |
| Returns | Whether or not this uniform exists for the Shader instance. |

Shader Objects
--------------

<h3>Creation</h3>

Shader instances are created and stored in the AssetManager via two factory methods:

```cpp
shader_error addNewShader(char * key, char * vertPath, char * fragPath);
```

| key | The key to associate this Shader with.|
| vertPath | The filepath to the vertex shader source.|
| fragPath | The filepath to the fragment shader source.|
| Returns | A shader_error. If no error occurred, SHADER_NO_ERROR is returned.|

One can also load shaders from strings, such as if your shader source is stored as
macro ```#definitions```:

```cpp
shader_error addNewShaderStrings(const char * key, const char * vertString,
                                 const char * fragString);
```

| key | The key to associate this Shader with.|
| vertString | The vertex shader source.|
| fragString | The fragment shader source.|
| Returns | A shader_error. If no error occurred, SHADER_NO_ERROR is returned.|

<h3>Usage</h3>

Normally, when used by a Tile or the compositor, usage is handled internally. However, one can tell OpenGL
to use a Shader directly via:

```cpp
void Shader::use();
```

<h3>Error Checking</h3>

If ```addNewShader()``` returns a value other than zero (SHADER_NO_ERROR), then something went wrong. To decode the
error message, pass it to the static function:

```cpp
static const char* Shader::getErrorDesc(shader_error e);
```

| e | The shader_error in question. |
| Returns | A const char* describing the error. |
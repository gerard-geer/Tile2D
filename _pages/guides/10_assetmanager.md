---
layout: featuredoc
title: The AssetManager
featuretitle: The AssetManager
permalink: /guides/assetmanager/
isComponentGuide: true
---

The AssetManager
-------------
{: .floating-header}

The AssetManager is a shared pool of resources, or *Assets*. An Asset is a type that subclasses
the abstract Asset class. Tile2D has two such Asset subclasses: *Shader* and *Texture*.


Using the AssetManager
----------------------

<h3>Setting Up The AssetManager</h3>

The AssetManager is a member of the Window's Renderer, and is created at Window initialization when
the Renderer is initialized.

<h3>Adding Assets To The AssetManager</h3>

To add either Shaders or Textures to the AssetManager, use the factory functions available in
AssetManager.

Loading and adding a Texture:

```cpp
tex_error addNewTexture(char * key, char * filepath);
```

| key | The key to associate this Texture with.|
| filepath | The path to the texture image file.|
| Returns | A tex_error. If no error occurred, TEX_NO_ERROR is returned.|

Loading and adding a Shader from source files:

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
shader_error addNewShaderStrings(const char * key, const char * vertString, const char * fragString);
```

| key | The key to associate this Shader with.|
| vertString | The vertex shader source.|
| fragString | The fragment shader source.|
| Returns | A shader_error. If no error occurred, SHADER_NO_ERROR is returned.|

<h3>Verifying the Presence of an Asset</h3>

To verify an Asset exists in the AssetManager, use ```contains()```

```cpp
bool AssetManager::contains(char * key);
```

| key | The key associated with the Asset.
| Returns | Whether or not the Asset exists in the Manager.

<h3>Retrieving an Asset from the AssetManager</h3>

Retrieving Assets is trivial, as long as you remember to cast your retrieval back to the correct type.

```cpp
Asset * AssetManager::get(char * key);
```

| key | The key associated with the Asset. |
| Returns | A pointer to the Asset if it exists, NULL otherwise. |

Example:

```cpp
Shader * s = (Shader*) renderer->assetManager->get("example-shader-key");
```

<h3>Removing an Asset From The AssetManager</h3>

To remove an Asset from the AssetManager, call ```remove()```. Remember though, this will render
any entities that rely on this Asset a bit broken. Be sure to tie up loose ends before deleting
an Asset.

```cpp
bool AssetManager::remove(char * key);
```

| key | The key associated with the Asset. |
| Returns | Whether or not the deletion occured. If not, it didn't exist in the first place. |

<h3>Clearing the AssetManager</h3>

**Warning: this will break anything that uses a non-vital Asset including all Tiles, and will also
force the compositor to revert to its stock shader, forcing one to re-set-up its custom shader if
applicable.**

Deletes everything in the AssetManager all at once.

```cpp
void AssetManager::clear();
```
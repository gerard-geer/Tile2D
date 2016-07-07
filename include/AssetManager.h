#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <string>
#include <map>
#include "Asset.h"
#include "Shader.h"
#include "Texture.h"

class Texture;
class Shader;

/**
 * @class AssetManager
 * @author Gerard Geer
 * @date 05/28/16
 * @file AssetManager.h
 * @brief All Textures and Shaders are are Assets. (Literally, they subclass it.)
 *        This class is a Manager that stores all of them for reuse across multiple
 *        Tiles.
 */
class AssetManager
{
private:

    /*
     * The underlying map that holds all the Assets.
     */
    std::map<std::string, Asset*> assetHash;
    
    /**
     * @brief Adds an Asset pointer to the manager.
     * @param key The key to associate this Asset with.
     */
    void add(char * key, Asset * asset);
	void add(const char * key, Asset * asset);
    
public:
    
    /*
     * Constructor. Doesn't do squat.
     */
    AssetManager();
    
    /*
     * Destructor. Also doesn't do anything.
     */
    ~AssetManager();
    
    /**
     * @brief Loads a Texture and adds it to the asset manager under the given key.
     *        If an error occurs during loading the asset is not added.
     * @param key The key to associate this Texture with.
     * @param filepath The path to the texture image file.
     * @return A tex_error. If no error occurred, TEX_NO_ERROR is returned.
     */
    tex_error addNewTexture(char * key, char * filepath);
    
    /**
     * @brief An error-suppressing overload of addNewTexture(). 
     * @param key The key to associate this Texture with.
     * @param filepath The path to the texture image file.
     * @return A tex_error. If no error occurred, TEX_NO_ERROR is returned.
     */
    tex_error addNewTexture(const char * key, const char * filepath);
    
    /**
     * @brief Loads a Shader and adds it to the asset manager under the given key.
     *        If an error occurs during loading the asset is not added.
     * @param key The key to associate this Shader with.
     * @param vertPath The filepath to the vertex shader source.
     * @param fragPath The filepath to the fragment shader source.
     * @return A shader_error. If no error occurred, SHADER_NO_ERROR is returned.
     */
    shader_error addNewShader(char * key, char * vertPath, char * fragPath);
    
    /**
     * @brief An error-suppressing overload of addNewShader().
     * @param key The key to associate this Shader with.
     * @param vertPath The filepath to the vertex shader source.
     * @param fragPath The filepath to the fragment shader source.
     * @return A shader_error. If no error occurred, SHADER_NO_ERROR is returned.
     */
    shader_error addNewShader(const char * key, const char * vertPath, const char * fragPath);
    
    /**
     * @brief Tests whether an Asset exists in the Manager.
     * @param key The key associated with the Asset.
     * @return Whether or not the Asset exists in the Manager.
     */
    bool contains(char * key);
	bool contains(const char * key);
    
    /**
     * @brief Retrieves an Asset from the Manager.
     * @param key The key associated with the Asset.
     * @return A pointer to the Asset if it exists, NULL otherwise.
     */
    Asset * get(char * key);
	Asset * get(const char * key);
    
    /**
     * @brief Removes and deletes an Asset from the Manager.
     * @param key The key associated with the Asset.
     * @return Whether or not the deletion occured. If not, it didn't
     *         exist in the first place.
     */
    bool remove(char * key);
	bool remove(const char * key);
    
    /**
     * @brief Clears out every entry in the Manager.
     */
    void clear();

};

#endif // ASSETMANAGER_H

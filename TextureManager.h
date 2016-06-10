#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <map>
#include <string.h>
#include <GLFW/glfw3.h>
#include "Texture.h"

using namespace std;
    
    // TODO Make Texture.destroy() a friend class of this so they can only be
    // freed on the GPU by calling remove here.
/**
 * @class TextureManager
 * @author Gerard Geer
 * @date 05/25/16
 * @file TextureManager.h
 * @brief This class is meant to decouple Textures from Tiles. With this Tiles
 *        can share Texture instances. It's essentially just a string-keyed
 *        associative array storing Texture instances so Tiles can access textures
 *        by name rather than holding their own instances. 
 */
class TextureManager
{
private:
    /*
     * The underlying stdl map that holds the Texture instances.
     */
    map <string, Texture*> texHash;
public:

    /**
     * @brief Constructor. Doesn't really do much.
     */
    TextureManager();
    /**
     * @brief Destructor. Also doesn't really do much.
     */
    ~TextureManager();
    
    /**
     * @brief Loads a texture and adds it to the manager, associating it with a name.
     * @param name The name to associate this Texture instance with.
     * @param filename The filename to find the texture image at.
     * @return A tex_error from the loading operation, if any.
     */
    tex_error add(char * name, char * filename);
    
    /**
     * @brief Returns whether or not the given texture exists.
     * @param name The name of the texture.
     * @return Whther or not the given texture exists.
     */
    bool contains(char * name);
    
    /**
     * @brief Removes the texture associated with the given name from the manager,
     *        additionally freeing it on the GPU as well.
     * @param name The name of the texture.
     * @return Whether or not the texture was deleted. If it wasn't it's because it
     *         doesn't exist in the manager.
     */
    bool remove(char * name);
    
    /**
     * @brief Deletes every Texture in the manager.
     */
    void clear();
    
    /**
     * @brief Returns a pointer to the Texture associated with the given name,
     *        NULL if it doesn't exist.
     * @param name The name of the Texture.
     * @return A pointer to the requested Texture, NULL otherwise.
     */
    Texture * get(char * name);

};

#endif // TEXTUREMANAGER_H

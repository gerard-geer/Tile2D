#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <string>
#include <map>
#include "Asset.h"

using namespace std;

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
    map<string, Asset*> assetHash;
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
     * @brief Adds an Asset pointer to the manager.
     * @param key The key to associate this Asset with.
     */
    void add(char * key, Asset * asset);
    
    /**
     * @brief Tests whether an Asset exists in the Manager.
     * @param key The key associated with the Asset.
     * @return Whether or not the Asset exists in the Manager.
     */
    bool contains(char * key);
    
    /**
     * @brief Retrieves an Asset from the Manager.
     * @param key The key associated with the Asset.
     * @return A pointer to the Asset if it exists, NULL otherwise.
     */
    Asset * get(char * key);
    
    /**
     * @brief Removes and deletes an Asset from the Manager.
     * @param key The key associated with the Asset.
     * @return Whether or not the deletion occured. If not, it didn't
     *         exist in the first place.
     */
    bool remove(char * key);
    
    /**
     * @brief Clears out every entry in the Manager.
     */
    void clear();

};

#endif // ASSETMANAGER_H

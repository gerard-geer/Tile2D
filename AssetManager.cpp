#include "AssetManager.h"

AssetManager::AssetManager()
{
}

AssetManager::~AssetManager()
{
}

void AssetManager::add(char * key, Asset * asset)
{
    if( !this->contains(key) )
        this->assetHash.insert(pair<string,Asset*>(key,asset));
}

bool AssetManager::contains(char * key)
{    
    // The ol' annoying iterator check.
    return ( this->assetHash.find(key) != this->assetHash.end() );
}

Asset * AssetManager::get(char * key)
{
    if( this->contains(key) )
    {
        return this->assetHash[key];
    }
    else return NULL;
}

bool AssetManager::remove(char * key)
{
    if( this->contains(key) )
    {
        this->assetHash[key]->destroy();
        delete (this->assetHash[key]);
        return true;
    }
    return false;
}

void AssetManager::clear()
{
    // This is super ugly.
    for( std::map<std::string, Asset*>::iterator i = this->assetHash.begin(); 
         i != this->assetHash.end();
         ++i ) {
        
        // Destroy the Asset.
        i->second->destroy();
        // Delete the asset instance.
        delete i->second;
    }
}
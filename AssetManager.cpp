#include "AssetManager.h"

AssetManager::AssetManager()
{
}

AssetManager::~AssetManager()
{
}

void AssetManager::add(char * key, Asset * asset)
{
    std::cout << "adding: " << key << std::endl;
    if( !this->contains(key) )
        this->assetHash.insert(std::pair<std::string,Asset*>(key,asset));
}

tex_error AssetManager::addNewTexture(char * key, char * filepath)
{
    Texture * t = new Texture();
    tex_error e;
    if(filepath == NULL) e = t->createEmpty();
    else e = t->load(filepath);
    if( e == TEX_NO_ERROR ) this->add(key,(Asset*)t);
    return e;
}

shader_error AssetManager::addNewShader(char * key, char * vertPath, char * fragPath)
{
    Shader * s = new Shader();
    shader_error e = s->load(vertPath, fragPath);
    if( e == SHADER_NO_ERROR ) this->add(key,(Asset*)s);
    return e;
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
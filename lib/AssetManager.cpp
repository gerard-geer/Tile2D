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
        this->assetHash.insert(std::pair<std::string,Asset*>(key,asset));
    else std::cout << "Warning: " << key << " already exists and was not re-added." << std::endl;
}

void AssetManager::add(const char* key, Asset * asset)
{
	this->add((char*)key, asset);
}

tex_error AssetManager::addNewTexture(char * key, char * filepath)
{
	// An error in case we need it.
    tex_error e;
    
	// Instantiate the new Texture to be added.
    Texture * t = new Texture();
    
    // If the path supplied is NULL, then we create an empty Texture.
    if(filepath == NULL) e = t->createEmpty();
    
    // Otherwise we load from file.
    else e = t->load(filepath);
    
    // If no errors happened, then we add the Texture to the hash.
    if( e == TEX_NO_ERROR ) this->add(key,(Asset*)t);
    
    // Otherwise it's time to alert the user to the problems.
    else  std::cout << "Error: " << key << ": " << Texture::getErrorDesc(e) << std::endl;
    return e;
}

tex_error AssetManager::addNewTexture(const char * key, const char * filepath)
{
    return this->addNewTexture((char*)key, (char*)filepath);
}

shader_error AssetManager::addNewShader(char * key, char * vertPath, char * fragPath)
{
	// Create the new Shader instance.
    Shader * s = new Shader();
    
    // Try loading it.
    shader_error e = s->load(vertPath, fragPath);
    
    // If there was no error, we can add it to the hash.
    if( e == SHADER_NO_ERROR ) this->add(key,(Asset*)s);
    
    // Otherwise it's time to report the bad news.
    else  std::cout << "Error: " << key << ": " << Shader::getErrorDesc(e) << std::endl;
    return e;
}

shader_error AssetManager::addNewShader(const char * key, const char * vertPath, const char * fragPath)
{
    return this->addNewShader((char*)key, (char*)vertPath, (char*)fragPath);
}

shader_error AssetManager::addNewShaderStrings(const char * key, const char * vertString, const char * fragString)
{
	// Create the instance.
    Shader * s = new Shader();
    
    // Try loading as strings.
    shader_error e = s->loadStrings(vertString, fragString);
    
    // If it worked out, yay!
    if( e == SHADER_NO_ERROR ) this->add(key,(Asset*)s);
    
    // Otherwise...
    else  std::cout << "Error: " << key << ": " << Shader::getErrorDesc(e) << std::endl;
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

Asset * AssetManager::get(const char * key)
{
	return this->get((char*)key);
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

bool AssetManager::remove(const char * key)
{
	return this->remove((char*)key);
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
    
    // Now that all the asset hash contains has been deleted, we can
    // clear out the underlying map.
    this->assetHash.erase(this->assetHash.begin(), this->assetHash.end());
}

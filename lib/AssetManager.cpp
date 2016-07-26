#include "AssetManager.h"

AssetManager::AssetManager()
{
}

AssetManager::~AssetManager()
{
}

void AssetManager::add(char * key, Asset * asset)
{
        std::cout << "tryna " << key << " to AssetManager." << std::endl;
    if( !this->contains(key) )
    {
        std::cout << "Adding " << key << " to AssetManager." << std::endl;
        this->assetHash.insert(std::pair<std::string,Asset*>(key,asset));
    }
    else std::cout << "Not adding " << key << " to AssetManager." << std::endl;
}

void AssetManager::add(const char* key, Asset * asset)
{
	this->add((char*)key, asset);
}

tex_error AssetManager::addNewTexture(char * key, char * filepath)
{
    Texture * t = new Texture();
    tex_error e;
    if(filepath == NULL) e = t->createEmpty();
    else e = t->load(filepath);
    if( e == TEX_NO_ERROR ) this->add(key,(Asset*)t);
    else std::cout << key << " Texture loading error: " << Texture::getErrorDesc(e) << std::endl;
    return e;
}

tex_error AssetManager::addNewTexture(const char * key, const char * filepath)
{
    return this->addNewTexture((char*)key, (char*)filepath);
}

shader_error AssetManager::addNewShader(char * key, char * vertPath, char * fragPath)
{
    Shader * s = new Shader();
    shader_error e = s->load(vertPath, fragPath);
    if( e == SHADER_NO_ERROR ) this->add(key,(Asset*)s);
    return e;
}

shader_error AssetManager::addNewShaderFromStrings(const char * key, const char * vertSource, const char * fragSource)
{
	std::cout << "Adding shader!" << std::endl;
	Shader * s = new Shader();
	shader_error e = s->loadFromStrings(vertSource, fragSource);
	std::cout << "ARE WE HERE YET?" << std::endl;
	if( e == SHADER_NO_ERROR ) this->add(key,(Asset*)s);
	std::cout << "Done addding shader!" << std::endl;
	return e;
}

shader_error AssetManager::addNewShader(const char * key, const char * vertPath, const char * fragPath)
{
    return this->addNewShader((char*)key, (char*)vertPath, (char*)fragPath);
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

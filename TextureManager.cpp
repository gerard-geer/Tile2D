#include "TextureManager.h"

TextureManager::TextureManager()
{
    // Since the map was already created, we don't really need to do anything.
}

TextureManager::~TextureManager()
{
    // Not much to do here either.
}

tex_error TextureManager::add(char* name, char* filename)
{
    // Create and load the texture.
    Texture * t = new Texture();
    tex_error e = t->load(filename);
    
    // If there was an error, we quit.
    if(e)
    {
        // First we have to delete the old instance.
        delete t;
        // Now we can return the error.
        return e;
    }
    
    // Otherwise we insert.
    this->texHash.insert(pair<string,Texture*>(name,t));
    return TEX_NO_ERROR;
}

bool TextureManager::contains(char* name)
{
    // The ol' annoying iterator check.
    return ( this->texHash.find(name) != texHash.end() );
}

bool TextureManager::remove(char* name)
{
    // If it doesn't exist, we quit early.
    if(!contains(name)) return false;
    
    // Delete the texture from the graphics card.
    this->texHash[name]->destroy();
    
    // Free the instance.
    delete this->texHash[name];
    
    // Remove it from the manager.
    this->texHash.erase(name);
    
    // Nothing went wrong.
    return true;
}

void TextureManager::clear()
{
    // For each element in the map...
    for( std::map<std::string, Texture*>::iterator i = this->texHash.begin(); 
         i != this->texHash.end();
         i++ ) {
        // Delete the texture instance.
        delete i->second;
    }
    // Now that we've cleared the entries, we can get rid of the entries.
    this->texHash.clear();
}

Texture * TextureManager::get(char * name)
{
    return this->texHash[name];
}

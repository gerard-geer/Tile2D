#ifndef ASSET_H
#define ASSET_H

/**
 * @class Asset
 * @author Gerard Geer
 * @date 05/29/16
 * @file Asset.h
 * @brief A wrapper class for defining things as Assets that can be stored
 *        in the AssetManager. Functions defined here are required for the
 *        asset manager to work correctly.
 */
class Asset
{
public: 
    virtual ~Asset() = 0;
    
    /**
     * @brief Called when the AssetManager deletes an Asset, before calling
     *        delete. Use this function to clear out any holdings an instance
     *        might have, say on the GPU. The reason this exists and things
     *        aren't taken care of in the destructor is that OpenGL holdings
     *        must be initialized after object creation, and thus the default
     *        values for members meant to store OpenGL handles might not refer
     *        to the correct item at destruction should the Asset not have
     *        been initialized.
     */virtual void destroy() = 0;
};

#endif // ASSET_H

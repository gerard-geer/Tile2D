#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <png.h>
#include <GLFW/glfw3.h>
#include "Asset.h"

/**
 * @class Texture
 * @author Gerard Geer
 * @date 05/18/16
 * @file Texture.h
 * @brief This class encapsulates an OpenGL texture as well as the
 *        loading of that texture. It also includes a simple error
 *        API since we're working with files. Note, this requires
 *        OpenGL to be fully initialized before use.
 *        An additional note is that since my primary test device
 *        doesn't support new enough OpenGL to have sampler objects,
 *        this doesn't use them.
 */
 
/*
 * Error codes:
 * TEX_COULD_NOT_OPEN_FILE:  Could not open file.
 * TEX_HEADER_VERIFY_FAIL:   PNG header verification failed.
 * TEX_READER_CREATION_FAIL: PNG reading struct could not be created.
 * TEX_INFO_STRUCT_FAIL:     PNG file info struct could not be created.
 * TEX_IO_JUMP_WIDTH_FAIL:   IO error. Could not set jump width.
 * TEX_FAIL_READ_START:      Error while starting image read.
 */ 
enum tex_error
{
    TEX_NO_ERROR = 0,
    TEX_COULD_NOT_OPEN_FILE = 1,
    TEX_HEADER_VERIFY_FAIL = 2,
    TEX_READER_CREATION_FAIL = 3,
    TEX_INFO_STRUCT_FAIL = 4,
    TEX_IO_JUMP_WIDTH_FAIL = 5,
    TEX_FAIL_READ_START = 6
};

class Texture : public Asset
{
private:
    
    /*
     * The texture ID given to us by OpenGL.
     */
    GLuint texID;
    
    /*
     * The width of this texture.
     */
    GLuint width;
    
    /*
     * The height of this texture.
     */
    GLuint height;
    
    /*
     * The color type of this texture.
     */
    png_byte colorType;
    
    /*
     * The color depth of this texture.
     */
    png_byte colorDepth;
    
    /*
     * Whether or not this texture contains an
     * alpha channel.
     */
    bool alpha;
    
    /**
     * @brief Loads the PNG file.
     * @param filename The filename of the PNG file to load.
     * @param data A pointer to the array that will be populated with the
     *        data of the image.
     * @return An error code, if any.
     */
    tex_error loadPNG(char* filename, png_bytep** data);
    
public:

    /**
     * @brief Construtor. Simply initializes some values. Use
     *        load() to actually load the texture from file.
     */
    Texture();

    /**
     * @brief Destructor. Doesn't really need to do much.
     */
    ~Texture();

    /**
     * @brief Loads a texture from file, and hands it directly
     *        OpenGL.
     * @param filename The filename of the imaqe.
     * @return A tex_error, if any.
     */
    tex_error load(char* filename);

    /**
     * @brief Creates a blank, empty texture. Useful for making 
     *        placeholder textures.
     * @return A tex_error, if any.
     */
    tex_error createEmpty();

    /**
     * @brief Takes an error code and gives you a description.
     * @return A description of an error code's error.
     */
    static const char* getErrorDesc(int error);
    
    /**
     * @brief Returns a text description of a png_color_type.
     * @param t The png_byte containing the color type.
     * @return A string containing a text description of the png_color_type.
     */
    static const char* getColorType(png_byte t);
    
    /**
     * @brief Returns the texture ID given to us by OpenGL.
     * @return Returns the texture ID given to us by OpenGL.
     */
    GLuint getID();
    
    /**
     * @brief Returns the width of this texture.
     * @return The width of this texture.
     */
    GLuint getWidth();
    
    /**
     * @brief Returns the height of this texture.
     * @return The height of this texture.
     */
    GLuint getHeight();
    
    /**
     * @brief Returns whether or not this texture has an alpha channel.
     * @return Whether or not this texture has an alpha channel.
     */
    bool hasAlpha();
    
    /**
     * @brief Frees texture resources. Call this when the GPU will no
     * longer need this texture data.
     */
    void destroy();
};

#endif // TEXTURE_H

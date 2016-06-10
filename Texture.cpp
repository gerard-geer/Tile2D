#include "Texture.h"

Texture::Texture()
{
    // Give everything an initial value.
    this->texID = 0;
    this->width = 0;
    this->height = 0;
    this->colorType = 0;
    this->colorDepth = 0;
    this->alpha = false;
}

Texture::~Texture()
{
    // Not much to do here.
}

const char* Texture::getColorType(png_byte t)
{
    switch(t)
    {
        case PNG_COLOR_TYPE_GRAY: return "PNG_COLOR_TYPE_GRAY";
        case PNG_COLOR_TYPE_GRAY_ALPHA: return "PNG_COLOR_TYPE_GRAY_ALPHA";
        case PNG_COLOR_TYPE_PALETTE: return "PNG_COLOR_TYPE_PALETTE";
        case PNG_COLOR_TYPE_RGB: return "PNG_COLOR_TYPE_RGB";
        case PNG_COLOR_TYPE_RGBA: return "PNG_COLOR_TYPE_RGBA";
        default: "some other type.";
    }
}


tex_error Texture::loadPNG(char* filename, png_bytep** data)
{
    // LibPNG does some not-so-robust buffered reading, and can only
    // handle if the file pointer isn't incremented more than 8 characters.
    // Fortunately for us, that's just enough for us to be able to peek at
    // the file and check to see if the header matches that of a PNG file.

    char header[8]; // Storage space for the header.
    FILE *fp = fopen(filename, "rb"); // "Read Bytes" mode.
    
    // Let's go ahead and make sure we can open the file.
    if( !fp ) return TEX_COULD_NOT_OPEN_FILE;
    
    // Test the header.
    fread(header,1,8,fp);
    if( png_sig_cmp((png_const_bytep)header,
                    (png_size_t)0,
                    (png_size_t)8) ) return TEX_HEADER_VERIFY_FAIL;
    
    // Initialize the struct libPNG uses for reading.
    png_structp pngReadStruct = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    
    // Make sure that that went okay.
    if( !pngReadStruct ) return TEX_READER_CREATION_FAIL;
    
    // Create the struct that libPNG uses to store info about the PNG file.
    png_infop pngInfoStruct = png_create_info_struct(pngReadStruct);
    if( !pngInfoStruct ) return TEX_INFO_STRUCT_FAIL;
    
    // TODO: Figure out what is actually going on here.
    if( setjmp( png_jmpbuf(pngReadStruct) ) ) return TEX_IO_JUMP_WIDTH_FAIL;
    
    png_init_io(pngReadStruct,fp);
    png_set_sig_bytes(pngReadStruct,8);
    
    // Read file info from the file.
    png_read_info(pngReadStruct, pngInfoStruct);
    
    // Store that file info now that we've plopped it into the structures.
    this->width = png_get_image_width(pngReadStruct, pngInfoStruct);
    this->height = png_get_image_height(pngReadStruct, pngInfoStruct);
    this->colorType = png_get_color_type(pngReadStruct, pngInfoStruct);
    this->colorDepth = png_get_bit_depth(pngReadStruct, pngInfoStruct);
    
    //  Store whether or not this texture has an alpha channel.
    this->alpha = this->colorType == PNG_COLOR_TYPE_RGBA;
    
    // Test for interlacing, and store whether or not we need to in the
    // read and info structs.
    png_set_interlace_handling(pngReadStruct);
    png_read_update_info(pngReadStruct, pngInfoStruct);
    
    // Actually do some file reading.
    if( setjmp( png_jmpbuf(pngReadStruct) ) ) return TEX_FAIL_READ_START;
    
    // Create space for the data.
    (*data) = (png_bytep*) malloc( sizeof(png_bytep) * this->height );

    // Oh we still have to inflate each row.
    for(int i = 0; i < this->height; ++i)
    {
        (*data)[i] = (png_byte*) malloc( png_get_rowbytes(pngReadStruct,pngInfoStruct) );
    }
    
    // Now we can actually do the reading of the image.
    png_read_image(pngReadStruct, (*data));
    
    // Close the file.
    fclose(fp);
    
    // Return no error.
    return TEX_NO_ERROR;
}

tex_error Texture::load(char* filename)
{
    // Declare a pointer for the data.
    png_bytep* rows = NULL;  
    
    // Oh god let's load the image into memory.
    tex_error error = this->loadPNG(filename, &rows);
    
    // If that process errored out, we just pass that error right on up.
    if(error) return error;
    
    // The data we get from loadPNG is a 2D list of individual color channel values.
    // These values are bytes.
    GLubyte * data;
    // The way data is stored differs based on color type.
    if(this->colorType == PNG_COLOR_TYPE_RGB)
    {
        data = (GLubyte*) malloc( sizeof(GLubyte) * this->width * this->height * 3);
        for( int i = 0; i < this->height; ++i ) // For each row...
        {
            for( int j = 0; j < this->width; ++j ) // For each pixel...
            {
                // For each color channel. 
                data[i*this->width*3 + j*3 + 0] = rows[i][j*3 + 0];
                data[i*this->width*3 + j*3 + 1] = rows[i][j*3 + 1];
                data[i*this->width*3 + j*3 + 2] = rows[i][j*3 + 2];
            }
            // Now that we're done with that row, we can get rid of it.
            free(rows[i]);
        }
    }
    else // PNG_COLOR_TYPE_RGBA
    {
        data = (GLubyte*) malloc( sizeof(GLubyte) * this->width * this->height * 4);
        for( int i = 0; i < this->height; ++i )
        {
            for( int j = 0; j < this->width; ++j )
            {
                data[i*this->width*4 + j*4 + 0] = rows[i][j*4 + 0];
                data[i*this->width*4 + j*4 + 1] = rows[i][j*4 + 1];
                data[i*this->width*4 + j*4 + 2] = rows[i][j*4 + 2];
                data[i*this->width*4 + j*4 + 3] = rows[i][j*4 + 3];
            }
        }
    }

    // Now that we've copied the data from our 2D list we can kill it.
    free(rows);
    
    // Get a texture ID from OpenGL.
    glGenTextures(1,&this->texID);
    
    // Bind to that texture object so that future functions modify it.
    glBindTexture(GL_TEXTURE_2D, this->texID);
    
    // Set up some generic texture parameters.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // Send our linear data to the graphics card.
    GLint type = (this->colorType == PNG_COLOR_TYPE_RGB) ? GL_RGB : GL_RGBA;
    glTexImage2D(GL_TEXTURE_2D, 0, type, this->width, this->height, 0, type, GL_UNSIGNED_BYTE, data);
    
    // Now that that data is stored on the card, we can get rid of our CPU-side copy.
    free(data);
    
    // Return no error.
    return TEX_NO_ERROR;
}

tex_error Texture::createEmpty()
{
    // Assemble the club members to tell them stuff to do.
    this->width = 64; // The docs for glTexImage2D state "All implementations
    this->height = 64;// support 2D textures that are at least 64 texels wide."
    this->colorDepth = 8;

    // Get a texture ID from OpenGL.
    glGenTextures(1,&this->texID);
    
    // Bind to that texture object so that future functions modify it.
    glBindTexture(GL_TEXTURE_2D, this->texID);
    
    // Set up some generic texture parameters.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // Send the GPU an empty texture, and make it single-channel to save memory.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->width, this->height, 
                 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
    return TEX_NO_ERROR;
    
}

const char* Texture::getErrorDesc(int error) // Static function
{
    switch(error)
    {
        // Oh why look at that we don't need to "break" these cases.
        case TEX_NO_ERROR: return "No error.";
        case TEX_COULD_NOT_OPEN_FILE : return "TEX_COULD_NOT_OPEN_FILE";
        case TEX_HEADER_VERIFY_FAIL : return "TEX_HEADER_VERIFY_FAIL";
        case TEX_READER_CREATION_FAIL : return "TEX_READER_CREATION_FAIL";
        case TEX_INFO_STRUCT_FAIL : return "TEX_INFO_STRUCT_FAIL";
        case TEX_IO_JUMP_WIDTH_FAIL : return "TEX_IO_JUMP_WIDTH_FAIL";
        case TEX_FAIL_READ_START : return "TEX_FAIL_READ_START";
        default: return "Undefined error.";
    }
}

GLuint Texture::getID()
{
    return this->texID;
}

GLuint Texture::getWidth()
{
    return this->width;
}

GLuint Texture::getHeight()
{
    return this->height;
}

bool Texture::hasAlpha()
{
    return this->alpha;
}

void Texture::destroy()
{
    glDeleteTextures(1, &this->texID);
}
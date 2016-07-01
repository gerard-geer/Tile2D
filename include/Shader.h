#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <map>
#include <stdlib.h>
#include "Asset.h"
#include "ShaderUniform.h"

/*
 * Error codes:
 * SHADER_NO_ERROR: No errors were encountered.
 * SHADER_COULD_NOT_OPEN: The source file could not be opened for some reason.
 * SHADER_COULD_NOT_COMP: The shader could not be compiled successfully.
 * SHADER_COULD_NOT_LINK: The shader program could not be linked for some reason.
 */
enum shader_error{
    SHADER_NO_ERROR = 0,
    SHADER_COULD_NOT_OPEN = 1,
    SHADER_COULD_NOT_COMP = 2,
    SHADER_COULD_NOT_LINK = 3
};

/**
 * @class Shader
 * @author Gerard Geer
 * @date 06/13/16
 * @file Shader.h
 * @brief Ties up all the boiler plate with setting up Shaders. Simply pass a shader
 *        two filenames, and it will be set up entirely automatically. Just call 
 *        setUniform() to set the value of uniform variables, and use() to use it.
 *        This is meant to target GLSL 1.2, but any shader without fixed position
 *        uniforms should work. (E.g., any shader that declares uniforms in this way:
 *        "uniform <type> <identifier>;"
 */
class Shader : public Asset
{
private:
    /*
     * The shader ID given to us by OpenGL.
     */
    GLuint id;
    
    /*
     * A list of uniforms that we can populate.
     */
    std::map<std::string, ShaderUniform*> uniforms;
    
    /**
     * @brief Returns a string telling us what type of shader we're working with.
     * @param type The shader type enum that we've got.
     * @return A string telling us in plain English what stage we're working with.
     */
    static const char * getShaderType(GLenum type);
    
    /**
     * @brief Scans a single line of source for "uniform <type> <identifier>;"
     * @param line The line to scan.
     */
    void scanLineForUniforms(char* line);
    
    /**
     * @brief Scans all of a source file for uniforms.
     * @param source The array of strings that make up the source flie.
     * @param numLines How many lines are in that array.
     */
    void scanSourceForUniforms(char** source, int numLines);
    
    /**
     * @brief Loads source code from a text file, and dumps the result into source,
     *        and the length of that source into length. If shader loading fails here,
     *        the pointers passed in are freed. No need to worry about cleanup if
     *        failure occurs here.
     * @param filename The path to the file.
     * @param source A pointer to an array of strings that can be manipulated.
     * @param numLines The number of lines of code loaded from file.
     * @return A shader_error, if any.
     */
    static shader_error loadSource(char* filename, char*** source, int* numLines);
    
    /**
     * @brief Loads and compiles a shader object. Any compilation errors are directed to stderr.
     * @param filename The filename of the shader source code.
     * @param type The type of shader.
     * @param shaderID A pointer to a GLuint where the new shader ID will be placed.
     * @return A shader_error, if any.
     */
    static shader_error initShader(char *filename, GLenum type, GLuint *shaderID);
    
    /**
     * @brief Takes the IDs of two compiled shader stages and links them into the shader
     *        program this class aims to represent.
     * @param vertID The ID of the vertex shader.
     * @param fragID The ID of the fragment shader.
     * @return A shader_error, if any.
     */
    shader_error linkShaders(GLuint vertID, GLuint fragID);
    
public:

    /**
     * @brief Constructs a new Shader. Doesn't really do much.
     */
    Shader();
    
    /**
     * @brief Kills of this Shader. Call destroy() first.
     */
    ~Shader();
    
    /**
     * @brief Loads the shader sources from file, compiles and links them. This
     *        should only be called by the AssetManager and its addNewShader() function.
     * @param vertFile The path to the vertex shader source.
     * @param fragFile The path to the fragment shader source.
     * @return A shader_error, if any.
     */
    shader_error load(char* vertFile, char* fragFile);
    
    /**
     * @brief Creates a new ShaderUniform instance and adds it to this shader
     *        for later use. Any errors this function may generate will be
     *        caught by glGetError().
     * @param name The name of the uniform as found in the shader source.
     * @param type The data type of the uniform.
     */
    void addUniform(char * name, uniform_type type);
    void addUniform(const char * name, uniform_type type);
    
    /**
     * @brief Removes a uniform. Main use case? Uniform adding didn't go well
     *        (as told by glGetError()), and you want to remove that baggage.
     * @param name The name of the uniform.
     * @return Whether or not the uniform was removed. If not, it just didn't
     *         exist in the first place.
     */
    bool removeUniform(char * name);
    bool removeUniform(const char * name);
    
    /**
     * @brief Returns whether or not this Shader has the specified uniform
     *        associated with it.
     * @param name The name of the uniform.
     * @return Whethe or not this shader has the specified uniform associated
     *         with it.
     */
    bool hasUniform(char * name);
    bool hasUniform(const char * name);
    
    /**
     * @brief Sets the value of a ShaderUniform, and as such the uniform it
     *        represents.
     * @param name The name of the uniform.
     * @param data The data to be sent to the uniform. This is a pointer to
     *             data appropriate for the datatype of the uniform. Is the
     *             uniform an int? data points to an int. Is it a vec3? data
     *             points to a 3 element array of floats. Mat4? 16 element
     *             array of floats. Additionally, any errors this function
     *             may generate will by caught by glGetError().
     */
    void setUniform(char * name, void * data);
    void setUniform(const char * name, void * data);
    
    /**
     * @brief Setting texture uniforms takes a bit more than the other types.
     *        This swaps to the desired texture unit, binds the texture, then
     *        sends the number of the texture unit to the uniform.
     * @param name The name of the uniform.
     * @param texID The ID of the texture given to us by OpenGL.
     * @param texUnit The desired texture unit.
     */
    void setTextureUniform(char * name, GLuint texID, GLuint texUnit);
    void setTextureUniform(const char * name, GLuint texID, GLuint texUnit);
    
    /**
     * @brief Returns the ID of this shader.
     * @return The ID of this shader.
     */
    GLuint getID();
    
    /**
     * @brief Tells OpenGL to use this shader.
     */
    void use();
    
    /**
     * @brief Returns an error description given a shader_error.
     * @param e The shader_error one is curious about.
     * @return An error description.
     */
    static const char* getErrorDesc(shader_error e);
    
    /**
     * @brief Deletes this shader from the GPU.
     */
    void destroy();

};

#endif // SHADER_H

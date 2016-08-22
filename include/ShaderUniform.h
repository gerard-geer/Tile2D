#ifndef SHADERUNIFORM_H
#define SHADERUNIFORM_H

#include <cstring>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

/**
 * @class uniform_type
 * @author Gerard Geer
 * @date 05/28/16
 * @file ShaderUniform.h
 * @brief Enumerates a subset of GLSL uniform types.
 */
enum uniform_type
{
    UNI_FLOAT,  // Float.
    UNI_VEC2,   // 2D vector.
    UNI_VEC3,   // 3D vector.
    UNI_VEC4,   // 4D vector.
    UNI_INT,    // Integer.
    UNI_TEX,    // Redundancy of INT, but specifically for specifying texture units.
    UNI_MAT2,   // 2x2 floating point matrix.
    UNI_MAT3,   // 3x3 floating point matrix.
    UNI_MAT4,   // 4x4 floating point matrix.
};

/**
 * @class ShaderUniform
 * @author Gerard Geer
 * @date 05/28/16
 * @file ShaderUniform.h
 * @brief Encapsulates multiple shader uniforms so that they can be located and
 *        set in a type-agnostic manner.
 */
class ShaderUniform
{
private:
    /*
     * This ShaderUniform's data type.
     */
    uniform_type type;
    /*
     * This uniform's location in the shader program.
     */
    GLint location;
    
public:
    /*
     * Constructor. Doesn't do much.
     */
    ShaderUniform();
    
    /*
     * Destructor. Also doesn't do much.
     */
    ~ShaderUniform();
    
    /**
     * @brief Takes a string representing a shader uniform type, 
     *        and returns the enumerated equivalent.
     * @param typeAsText The string representing the type.
     * @return The enumerated equivalent.
     */
    static uniform_type getType(char * typeAsText);
    
    /**
     * @brief Locates this uniform within the shader program and stores
     *        its location. This is required before use of this uniform.
     * @param program The shader program identifier given to us by OpenGL.
     * @param type The data type of the uniform.
     * @param name The name of this uniform as found in the shader source.
     */
    void init(GLuint program, uniform_type type, char * name);
    
    /**
     * @brief Assign a value to the uniform. This requires extra care.
     *        No matter what uniform type, pass a pointer to it that has been
     *        cast to void *. Depending on the shader type, it will be cast
     *        back to the correct type.
     *        Additionally, the multiple values required by vector and
     *        matrix types must be passed in as a pointer to a 1D list of
     *        values.
     * @param data The data to be assigned to the uniform.
     */
    void set(void * data);
    
};

#endif // SHADERUNIFORM_H

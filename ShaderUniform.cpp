#include "ShaderUniform.h"

ShaderUniform::ShaderUniform()
{
}

ShaderUniform::~ShaderUniform()
{
}

#include <iostream>
uniform_type ShaderUniform::getType(char * typeAsText)
{
    std::cout << "type being added: " << typeAsText << std::endl;
    if( strcmp(typeAsText, "int") == 0 ) return UNI_INT;
    if( strcmp(typeAsText, "float") == 0 ) return UNI_FLOAT;
    if( strcmp(typeAsText, "vec2") == 0 ) return UNI_VEC2;
    if( strcmp(typeAsText, "vec3") == 0 ) return UNI_VEC3;
    if( strcmp(typeAsText, "vec4") == 0 ) return UNI_VEC4;
    if( strcmp(typeAsText, "mat2") == 0 ) return UNI_MAT2;
    if( strcmp(typeAsText, "mat3") == 0 ) return UNI_MAT3;
    if( strcmp(typeAsText, "mat4") == 0 ) return UNI_MAT4;
    if( strcmp(typeAsText, "sampler2D") == 0 ) return UNI_TEX;
    return UNI_INT;
}

void ShaderUniform::init(GLuint program, uniform_type type, char * name)
{
    this->type = type;
    glUseProgram(program);
    this->location = glGetUniformLocation(program, name);    
    
}

void ShaderUniform::set(void * data)
{
    // Welcome to This Is Hinky Sketchy World!
    switch(this->type)
    {
        // TODO: Add glUniform() calls.
        case UNI_FLOAT: 
        {   // I want independent scopes per case. That's why I'm using brackets.
            float* d = (float*) data;
            glUniform1f(this->location, *d);
            break;
        }
        case UNI_VEC2: 
        {
            float** d  = (float**) data;
            glUniform2f(this->location, (*d)[0], (*d)[1]);
            break;
        }
        case UNI_VEC3: 
        {
            float** d = (float**) data;
            glUniform3f(this->location, (*d)[0], (*d)[1], (*d)[2]);
            break;
        }
        case UNI_VEC4: 
        {
            float** d = (float**) data;
            glUniform4f(this->location, (*d)[0], (*d)[1], (*d)[2], (*d)[3]);
            break;
        }
        case UNI_INT: 
        {
            int* d = (int*) data; 
            glUniform1i(this->location, *d);
            break;
        }
        case UNI_MAT2: 
        {
            float** d = (float**) data;
            glUniformMatrix2fv(this->location,1, (GLboolean) true, *d);
            break;
        }
        case UNI_MAT3: 
        {
            float** d = (float**) data;
            glUniformMatrix3fv(this->location,1, (GLboolean) true, *d);
            break;
        }
        case UNI_MAT4: 
        {
            float** d = (float**) data;
            glUniformMatrix4fv(this->location,1, (GLboolean) true, *d);
            break;
        }
        case UNI_TEX: 
        {
            int* d = (int*) data; 
            glUniform1i(this->location, *d);
            break;
        }
        default: 
            break;
    }
}

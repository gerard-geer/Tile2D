#include "ShaderUniform.h"

ShaderUniform::ShaderUniform()
{
}

ShaderUniform::~ShaderUniform()
{
}

#include <iostream>
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

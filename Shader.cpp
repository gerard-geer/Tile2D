#include "Shader.h"

Shader::Shader()
{
    // Default value for the shader ID.
    this->id = 0;
}

Shader::~Shader()
{
    // It's lonely here.
}

const char * Shader::getShaderType(GLenum type)
{
    switch(type)
    {
        case GL_VERTEX_SHADER: return "GL_VERTEX_SHADER";
        case GL_FRAGMENT_SHADER: return "GL_FRAGMENT_SHADER";
        default: return "SOME_OTHER_STAGE_IUNNO";
    }
}

shader_error Shader::loadSource(char* filename, char*** source, int* numLines)
{
    // Initialize numLines to zero.
    *numLines = 0;
    
    // Initialize the source strings array.
    (*source) = (char**)malloc(sizeof(char*)*2);
    
    // Create an std::string to store the current line of the file.
    std::string line;
    
    // Try creating an std::ifstream to read the source file.
    std::ifstream sourceFile(filename);
    
    // If the file was opened successfully, we read from it.
    if( sourceFile.is_open() )
    {
        // For as long as we can get lines, we do.
        while( getline(sourceFile, line) )
        {
            // Increment the number of lines.
            ++(*numLines);
            
            // Append a newline to the current line.
            line.push_back('\n');

            // Expand the source string array.
            (*source) = (char**) realloc( (*source), sizeof(char*) * (*numLines) );

            // Allocate the string at the current index.
            (*source)[(*numLines)-1] = (char*) malloc(sizeof(char) * line.length()+1);
            
            // Copy the read line to the source code lines array.
            strcpy((*source)[(*numLines)-1], line.c_str());
        }
        // Don't forget to close the file!
        sourceFile.close();
        
        // Return no error.
        return SHADER_NO_ERROR;
    }
    // If we weren't able to open the file, we have to return an error.
    else
    {
        free(*source); // We didn't initialize the individiual pointers yet.
        return SHADER_COULD_NOT_OPEN;
    }
}

shader_error Shader::initShader(char* filename, GLenum type, GLuint* shaderID)
{
    // An unsigned int to store the number of lines loaded.
    int numLines;
    
    // An array of strings to store the lines of the source code.
    char** source = NULL;
    
    // Actually load the shader.
    shader_error e = Shader::loadSource(filename, &source, &numLines);
    
    if(e) return e; // Return if there was an error. The pointers were cleaned
                    // up by loadSource, since it was where things went sideways.
    
    // Create a shader and store it in the given ID pointer.
    *shaderID = glCreateShader(type);
    
    // Send the source code to the GPU for compilation.
    glShaderSource(*shaderID, numLines, source, NULL);
    
    // Actually do that compilation.
    glCompileShader(*shaderID);
    
    // IT'S TESTING TIME. Did it actually compile?
    GLint compiled = 0;
    // Get whether or not the shader compiled.
    glGetShaderiv(*shaderID, GL_COMPILE_STATUS, &compiled);
    if( compiled == GL_FALSE ) // If it didn't...
    {
        // First we print out the shader source for review.
        for( unsigned int i = 0; i < numLines; ++i )
        {
            printf("%4d: ",i+1); // Print the line number.
            std::cerr << source[i]; // We already have a newline! No std::endl today.
        }
        
        // Create an integer to store the length of the info log.
        GLint infoLength = 0;
        
        // Get the length of the info log and store it in that GLint.
        glGetShaderiv(*shaderID, GL_INFO_LOG_LENGTH, &infoLength);
        
        // Now that we have that we can create a string to store the info log.
        GLchar* errorLog = (GLchar*)malloc(sizeof(GLchar)*infoLength );
        
        // Get the shader info log.
        glGetShaderInfoLog(*shaderID, infoLength, &infoLength, errorLog);
        
        // Print the info log out.
        std::cerr << "Shader compilation error: " << "(" << Shader::getShaderType(type) << ")" << std::endl;
        std::cerr << errorLog << std::endl;

        // Free it from memory.
        free(errorLog);
        
        // Delete the shader and source from the GPU.
        glDeleteShader(*shaderID);
        
        // Free the source and line length arrays.
        for( int i = 0; i < numLines; ++i ) free(source[i]);
        free(source);
        
        return SHADER_COULD_NOT_COMP;
    }
        
    // Free the source and line length arrays.
    for( int i = 0; i < numLines; ++i ) free(source[i]);
    free(source);
    
    // If nothing went wrong we return no error.
    return SHADER_NO_ERROR;
}

shader_error Shader::linkShaders(GLuint vertID, GLuint fragID)
{
    // Oh man we're making our actual shader program now!
    this->id = glCreateProgram();
    
    // Attach the individual shaders to the program.
    glAttachShader(this->id, vertID);
    glAttachShader(this->id, fragID);
    
    // Since this step has to be done now, I guess we do it here.
    // Link attribute location 0 with "vertPosition". This means that
    // any vertex shader for this engine needs to have a vec3
    // attribute variable named aPosition.
    glBindAttribLocation(this->id, 0, "vertPos");
    
    // Oh wait, we also need to link the UV buffer.
    glBindAttribLocation(this->id, 1, "vertUV");
    
    // Time to link, guys.
    glLinkProgram(this->id);
    
    // Error checking.
    GLint isLinked = 0;
    glGetProgramiv(this->id, GL_LINK_STATUS, &isLinked);
    if( isLinked == GL_FALSE )
    {
        // A place to store the length of the info log.
        GLint infoLength = 0;
        
        // Get the length of the info log and store it in that var.
        glGetProgramiv(this->id, GL_INFO_LOG_LENGTH, &infoLength);
        
        // Now that we have that we can create a string to store the info log.
        GLchar* errorLog = (GLchar*) malloc(sizeof(GLchar)*infoLength);
        
        // Copy the info log into that string we just made.
        glGetProgramInfoLog(this->id, infoLength, &infoLength, errorLog);
        
        // Print out the error to stderr.
        std::cerr << "Shader linking error: " << std::endl;
        std::cerr << errorLog << std::endl;
        
        // Free the memory the error log was hogging.
        free(errorLog);
        
        // Detach the shader object code.
        glDetachShader(this->id, vertID);
        glDetachShader(this->id, fragID);
        
        // Delete them.
        glDeleteShader(vertID);
        glDeleteShader(fragID);
        
        // Now we can delete the program as well.
        glDeleteProgram(this->id);
        
        // Return an error.
        return SHADER_COULD_NOT_LINK;        
    }
    
    // Oh wait, the individual shaders are just source code and object files.
    // Let's get rid of them now that they're no longer needed.
    glDetachShader(this->id, vertID);
    glDetachShader(this->id, fragID);
    glDeleteShader(vertID);
    glDeleteShader(fragID);
    
    // Return no error.
    return SHADER_NO_ERROR;
}

shader_error Shader::load(char* vertFile, char* fragFile)
{
    // Create individual IDs for each shader stage.
    GLuint vertID = 0, fragID = 0;
    
    // Load and compile those shader stages.
    shader_error e = SHADER_NO_ERROR;
    e = Shader::initShader(vertFile, GL_VERTEX_SHADER, &vertID);
    if(e) return e;
    e = Shader::initShader(fragFile, GL_FRAGMENT_SHADER, &fragID);
    if(e) return e;
    // Now that we've compiled the shaders, we can link them.
    e = linkShaders(vertID, fragID);
    if(e) return e;

    // Return the no_error that was returned last.
    return e;
}



void Shader::addUniform(char * name, uniform_type type)
{
    // Create a new ShaderUniform.
    ShaderUniform* s = new ShaderUniform();
    
    // Initialize it.
    s->init(this->id, type, name);
    
    // Speed date->court->marry the string and add it to uniforms.
    this->uniforms.insert(std::pair<std::string,ShaderUniform*>(name,s));
}

bool Shader::removeUniform(char * name)
{
    // Check to see if the uniform exists.
    if( this->uniforms.find(name) != uniforms.end() )
    {
        // If so, delete the ShaderUniform instance.
        delete this->uniforms[name];
        // Clear it out of the map.
        this->uniforms.erase(name);
        // Return "Yes sir. It's taken care of."
        return true;
    }
    // Otherwise we return to papa for a spanking.
    return false;
}

bool Shader::hasUniform(char * name)
{
    return ( this->uniforms.find(name) != this->uniforms.end() );
}

void Shader::setUniform(char * name, void * data)
{
    if(!this->hasUniform(name))
    {
        std::cout << "This shader doesn't have a " << name << "uniform." << std::endl;
        return;
    }
    // Pretty straightforward really. It's so straight
    // forward that it has not one but two arrows pointing
    // straight forward! How about that.
    this->uniforms[name]->set(data);
}

void Shader::setTextureUniform(char * name, GLuint texID, GLuint texUnit)
{
    glActiveTexture(GL_TEXTURE0+texUnit);
    glBindTexture(GL_TEXTURE_2D, texID);
    int tu = texUnit;
    //std::cout << this->hasUniform(name) << " asdf" << std::endl;
    this->setUniform(name, &tu);
}

GLuint Shader::getID()
{
    return this->id;
}

const char* Shader::getErrorDesc(shader_error e)
{
    switch(e)
    {
        case SHADER_NO_ERROR: return "SHADER_NO_ERROR";
        case SHADER_COULD_NOT_OPEN: return "SHADER_COULD_NOT_OPEN";
        case SHADER_COULD_NOT_COMP: return "SHADER_COULD_NOT_COMP";
        case SHADER_COULD_NOT_LINK: return "SHADER_COULD_NOT_LINK";
        default: return "Undefined error.";
    }
}

void Shader::destroy()
{
    glDeleteProgram(this->id);
}


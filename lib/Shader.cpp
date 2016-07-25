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

int Shader::splitOnNewlines(char * src, char *** dst)
{
	// The current number of lines.
	int numLines = 0;
	
	// A pointer to the most recently cleaved token.
	char *curLine;
	
	// It's possible that the src string is constant, which would
	// cause strtok to fail. Therefore we need to make our own
	// copy.
	char * data = strdup(src);
	
	std::cout << "SOURCE: \n"<< data << std::endl;

	// Loop while there are tokens to be had...
	do
	{
		
		// Expand the destination array.
		(*dst) = (char**) realloc((*dst), sizeof(char*) * (numLines+1) );
		std::cout << "Made it to realloc" << std::endl;
		
		// Duplicate the current line into the current slot in the destination array.
		(*dst)[numLines] = strdup(strtok(data, "\n"));
		
		std::cout << numLines + 1 << ": " << (*dst)[numLines] << std::endl;

		// Increment the number of lines stored. 
		numLines++;
		
	}while( (*dst)[numLines-1] != NULL );
	
	std::cout << "Done splitting" << std::endl;
	
	// Finally we return the number of lines parsed.
	return numLines;
		
		
		
	
}

void Shader::scanLineForUniforms(char* line)
{
    // Number of tokens found in the line.
    unsigned int numTokens = 0;
    // The current token.
    char * token;
    // An array to store all the tokens in.
    char ** tokens = (char**) malloc( sizeof(char*) * 3 );
    
    // Get the first token.
    token = strtok(line, " ;,/*+-^&!()\n=?.!{}[]");
    
    // While we've no more than three tokens, and the prior token
    // read returned something. (Since all uniform declarations
    // in GLSL 1.2 and earlier are three tokens, we only need the
    // first three.)
    while( numTokens < 3 && token != NULL )
    {
        // Store the token.
        tokens[numTokens] = token;
        // Update how many tokens we have.
        ++numTokens;
        // Get the next token.
        token = strtok(NULL, " ;,/*+-^&!()\n=?.!{}[]");
    }
    
    // If we have three tokens, there's a chance this is a uniform!
    if(numTokens == 3)
    {
        // If the first token is "uniform" then the next should be
        // the type and the third is the variable name.
        if( strcmp(tokens[0], "uniform") == 0 )
        {
            this->addUniform( tokens[2], ShaderUniform::getType(tokens[1]) );
        }
    }
    
    // Gotta free the token array.
    free(tokens);
}
void Shader::scanSourceForUniforms(char** source, int numLines)
{
    // Just go through and check each line.
    for( unsigned int i = 0; i < numLines; ++i )
    {
        this->scanLineForUniforms((source)[i]);
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
        std::cout << "Could not load " << filename << std::endl;
        free(*source); // We didn't initialize the individiual pointers yet.
        return SHADER_COULD_NOT_OPEN;
    }
}

shader_error Shader::initShader(char** source, int numLines, GLenum type, GLuint* shaderID)
{
    
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
    
    // Load and compile the vertex shader.
    shader_error e = SHADER_NO_ERROR;
    char ** vertSourceLines = NULL; int numVertLines = 0;
    e = loadSource(vertFile, &vertSourceLines, &numVertLines);
    if(e) return e;
    e = Shader::initShader(vertSourceLines, numVertLines, GL_VERTEX_SHADER, &vertID);
    if(e) return e;
    
    // Load and compile the fragment shader.
    char ** fragSourceLines = NULL; int numFragLines = 0;
    e = loadSource(fragFile, &fragSourceLines, &numFragLines);
    if(e) return e;
    e = Shader::initShader(fragSourceLines, numFragLines, GL_FRAGMENT_SHADER, &fragID);
    if(e) return e;
    
    // Now that we've compiled the shaders, we can link them.
    e = linkShaders(vertID, fragID);
    if(e) return e;
    
    // Now we need to parse for uniforms.
    this->scanSourceForUniforms(vertSourceLines, numVertLines);
    this->scanSourceForUniforms(fragSourceLines, numFragLines);
    
	// All that's left to do is clean up.
    for(unsigned int i = 0; i < numVertLines; ++i) free(vertSourceLines[i]);
    for(unsigned int i = 0; i < numFragLines; ++i) free(fragSourceLines[i]);
    free(vertSourceLines);
    free(fragSourceLines);

    // Return the no_error that was returned last.
    return e;
}

shader_error Shader::loadFromStrings(const char* vertSource, const char* fragSource)
{
	std::cout << "Loading shader from strings" << std::endl;
    // Create individual IDs for each shader stage.
    GLuint vertID = 0, fragID = 0;
    
    // An error just in case.
    shader_error e = SHADER_NO_ERROR;
    
    // We need to split the source code on newlines since much of
    // the loading process laid out by this class operates on arrays
    // of lines.
    char ** vertSourceLines = NULL; int numVertLines = 0;
    char ** fragSourceLines = NULL; int numFragLines = 0;
    
    // Do the actual splitting.
    numVertLines = splitOnNewlines((char*)vertSource, &vertSourceLines);
    std::cout << "Done splitting lines." << std::endl;
    numFragLines = splitOnNewlines((char*)fragSource, &fragSourceLines);
    
    // Try to initialize the shaders. Oooooooh boy
    e = Shader::initShader(vertSourceLines, numVertLines, GL_VERTEX_SHADER, &vertID);
    if(e) return e;
    e = Shader::initShader(fragSourceLines, numFragLines, GL_FRAGMENT_SHADER, &vertID);
    if(e) return e;
    
    // Now that the shader stages are compiled, we can link them.
    e = linkShaders(vertID, fragID);
    
    // Now we need to parse for uniforms.
    this->scanSourceForUniforms(vertSourceLines, numVertLines);
    this->scanSourceForUniforms(fragSourceLines, numFragLines);
    
	// All that's left to do is clean up.
    for(unsigned int i = 0; i < numVertLines; ++i) free(vertSourceLines[i]);
    for(unsigned int i = 0; i < numFragLines; ++i) free(fragSourceLines[i]);
    free(vertSourceLines);
    free(fragSourceLines);
    
	std::cout << "Done loading shader from strings" << std::endl;
    // Return the most recent error result. (Which should be SHADER_NO_ERROR at this point.)
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

void Shader::addUniform(const char * name, uniform_type type)
{
    this->addUniform((char*)name, type);
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

bool Shader::removeUniform(const char * name)
{
    return this->removeUniform((char*)name);
}

bool Shader::hasUniform(char * name)
{
    return ( this->uniforms.find(name) != this->uniforms.end() );
}

bool Shader::hasUniform(const char * name)
{
    return this->hasUniform((char*)name);
}

void Shader::setUniform(char * name, void * data)
{
    if(!this->hasUniform(name)) return;
    // Pretty straightforward really. It's so straight
    // forward that it has not one but two arrows pointing
    // straight forward! How about that.
    this->uniforms[name]->set(data);
}

void Shader::setUniform(const char * name, void * data)
{
    this->setUniform((char*)name, data);
}

void Shader::setTextureUniform(char * name, GLuint texID, GLuint texUnit)
{
    glActiveTexture(GL_TEXTURE0+texUnit);
    glBindTexture(GL_TEXTURE_2D, texID);
    int tu = texUnit;
    //std::cout << this->hasUniform(name) << " asdf" << std::endl;
    this->setUniform(name, &tu);
}

void Shader::setTextureUniform(const char * name, GLuint texID, GLuint texUnit)
{
    this->setTextureUniform((char*)name, texID, texUnit);
}

GLuint Shader::getID()
{
    return this->id;
}

void Shader::use()
{
    glUseProgram(this->id);
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


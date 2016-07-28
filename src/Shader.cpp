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

void Shader::replaceChar(char * src, char old, char replacement)
{
	for( int i = strlen(src)-1; i >= 0; --i )
	{
		if( src[i] == old ) src[i] = replacement;
	}
}

int Shader::parseSourceString(char * src, char *** dst)
{
	// How many lines we've encountered so far.
	int lines = 0;
	
	// The current line. I really hope there's not a single line longer than 2000 chars.
	char * curLine = (char*) malloc( sizeof(char) * 2000 );
	
	// Initialize the destination array meant to hold all the lines we cut out.
	(*dst) = (char**) malloc( sizeof(char*) * (lines+1) );
	
	// Go ahead and get the first line.
	curLine = strtok(src, "\n");
	
	// While we still have tokens to get.
	while( curLine != NULL )
	{
		// Duplicate the current line into the current array index.
		(*dst)[lines] = strdup(curLine);
		
		// Now that we've gotten a line, we can increment how many we have.
		++lines;
		
		// Extend the array.
		(*dst) = (char**) realloc( (*dst), sizeof(char*) * (lines+1) );
		
		// Get the next line. WE MIGHT NEED TO CLEAR IT FIRST.
		curLine = strtok(NULL, "\n");
	}
	
	// Let's get rid of that giant string real quick.
	free(curLine);
	
	// Now we can go through and replace the $ tokens with newlines.
	for( int i = 0; i < lines; ++i )
	{
		Shader::replaceChar((*dst)[i], '$', '\n');
	}	
	
	// Finally we return the number of lines that we've loaded.
	return lines;	
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
    else return SHADER_COULD_NOT_OPEN;
}

shader_error Shader::initShader(char*** source, int numLines, GLenum type, GLuint* shaderID)
{
    
    // Create a shader and store it in the given ID pointer.
    *shaderID = glCreateShader(type);
    
    // Send the source code to the GPU for compilation.
    glShaderSource(*shaderID, numLines, *source, NULL);
    
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
        
        return SHADER_COULD_NOT_COMP;
    }
    
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
	// A shader_error in case we need it.
	shader_error e = SHADER_NO_ERROR; // = 0, by the way.
	
    // Create individual IDs for each shader stage.
    GLuint vertID = 0, fragID = 0;
    
    // String arrays for each loaded source.
    char ** vertSource = NULL; char ** fragSource = NULL;
    
    // Line counts for each shader as well.
    int vertLines = 0, fragLines = 0;
    
    // Load the source of each shader.
    if(!e) e = loadSource(vertFile, &vertSource, &vertLines);
    if(!e) e = loadSource(fragFile, &fragSource, &fragLines);
    
    // Compile those shader stages.
    if(!e) e = Shader::initShader(&vertSource, vertLines, GL_VERTEX_SHADER, &vertID);
    if(!e) e = Shader::initShader(&fragSource, fragLines, GL_FRAGMENT_SHADER, &fragID);
    
    // Now that we've compiled the shaders, we can link them.
    if(!e) e = linkShaders(vertID, fragID);
    
    // Now we need to parse for uniforms.
    this->scanSourceForUniforms(vertSource, vertLines);
    this->scanSourceForUniforms(fragSource, fragLines);
    
    // Now that we're done with the source code we need to get rid of it.
    if(vertSource)
    {
    	for(unsigned int i = 0; i < vertLines; ++i) free(vertSource[i]);
    	free(vertSource);
    }
    if(fragSource)
    {
		for(unsigned int i = 0; i < fragLines; ++i) free(fragSource[i]);
		free(fragSource);
	}

    // Return the no_error that was returned last.
    return e;
}

shader_error Shader::loadStrings(const char* vertString, const char* fragString)
{
	// A shader_error in case we need it.
	shader_error e = SHADER_NO_ERROR; // = 0, by the way.
	
	// Create modifiable versions of the given strings.
	char * vs = strdup(vertString); char * fs = strdup(fragString);
	
    // Create individual IDs for each shader stage.
    GLuint vertID = 0, fragID = 0;
	
    // String arrays for each shader's source.
    char ** vertSource = NULL; char ** fragSource = NULL;
    
    // Line counts for each shader as well.
    int vertLines = 0, fragLines = 0;
    
    // Parse the shader source. 
    vertLines = Shader::parseSourceString(vs, &vertSource);
    fragLines = Shader::parseSourceString(fs, &fragSource);
    
    // Coompile those shader stages.
    if(!e) e = Shader::initShader(&vertSource, vertLines, GL_VERTEX_SHADER, &vertID);
    if(!e) e = Shader::initShader(&fragSource, fragLines, GL_FRAGMENT_SHADER, &fragID);
    
    // Now that we've compiled the shaders, we can link them.
    if(!e) e = linkShaders(vertID, fragID);
    
    // Now we need to parse for uniforms.
    this->scanSourceForUniforms(vertSource, vertLines);
    this->scanSourceForUniforms(fragSource, fragLines);
    
    // Now that we're done with the source code we need to get rid of it.
    for(unsigned int i = 0; i < vertLines; ++i) free(vertSource[i]);
    for(unsigned int i = 0; i < fragLines; ++i) free(fragSource[i]);
    free(vertSource);
    free(fragSource);
    free(vs);
    free(fs);
    
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


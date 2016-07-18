#!/usr/local/bin/python

import sys
import os

"""
File: glsl-to-header.py
Author: Gerard Geer (gerarddgeer@gmail.com)

This script simply takes the filenames passed to it as arguments, and
spits out their contents into several #define directives in a single
file. This allows the binary to not require a specific directory containing
the stock shaders for the BGTile, SceneTile, and AnimTile to be distributed
alongside it.

Since the generated header file is not meant to be human readable (If you
want to read the shader source, go read the shader source) this removes
all trailing and leading whitespace as well as documentation.

usage:
    glsl-to-header <output filepath> <filepath A> <filepath B> ...
    
Note: It is highly recommended that you differ your vertex and fragment
shaders by file extension alone, e.g., use .frag and .vert.
"""

"""
Opens a given filename, and stores its contents as a #define macro
within a returned String. This also gets rid of whitespace and
documentation. If you're looking at this as the shader source instead
of the shader source itself, you deserve to have a bad time.

Parameters:
    name (String): The name of the directive.
    file (String): The filepath of the file to #define-ify.
    
Returns:
    The directive as a Python String.
"""
def createDefineDirective(name, file):

    # Start off the define macro.
    result = "#define " + str(name) + " "
    
    # Open the file.
    f = open(file)
    
    # Whether or not we're within a multiline comment.
    multiline = False
    
    # Go through the file appending each line to the macro.
    for line in f:
        
        # First we strip beginning and trailing whitespace.
        line = line.strip()
        
        # Next we check to see if we're beginning or end of a multiline
        # comment.
        if( line.startswith('/*') ):
            multiline = True;
        # If we're in a multiline comment and this line contains the 
        # end token, we need to delete everything up to and including
        # the end token.
        if( '*/' in line and multiline ):
            line = line[line.index('*/')+2:]
            multiline = False;
            
        # If we're in a multiline comment we don't need to do anything.
        if( multiline ):
            continue
        
        # If we're not we need to remove any segment comments.
        # The ones that are like this /* stuff commented out */
        if( not multiline ):
            if( '/*' in line and '*/' in line ):
                startIndex = line.index('/*');
                endIndex = line.index('*/');
                line = line[:startIndex].join(line[endIndex+2:])
        
        # Finally we check for single line comments.
        if( line.startswith('//') ):
            continue
        
        # Those comments can end lines too.
        if( '//' in line ):
            line = line[:line.index('//')]
                
        
        # We need to strip the newline out of the original line so we can
        # place a closing " and tab before the carriage return.
        line = line.translate(None, "\n")
        if( len(line) == 0 ):
            continue
        result += '"' + str(line) + '"\t\\\n'
     
    # Finally we return the resultant macro, without the last continutation
    # character or newline.
    return result[:-2] if ( len(result)>2 ) else result
    
"""
Filenames normally can't be used as variable or macro names, because they
can have spaces or other barred characters, not to mention extensions.
This clears all that up.

Parameters:
    filename (String): The filename to convert to a macro name.

Returns:
    The macro name.
"""
def filenameToMacroName(filename):

    # Get the name of the file from the path.
    name = os.path.basename(filename)
    
    # Split the name and extension.
    split = os.path.splitext(name)
    
    # Since shader type is often only differed by extension,
    # we need to append the extension to the name if it's
    # .frag or .glsl
    if(len(split) > 1): # If we even had an extension.
        # If the filename was frag or vert
        if(split[1] == '.vert' or split[1] == '.frag'):
            # Replaces the . with an underscore.
            name = split[0] + '_' + split[1][1:]
        else:
            name = split[0]
    
    # Replace spaces with underscores.
    name = name.replace(' ', '_')
    
    # Remove invalid characters.
    name = name.translate(None, "!@#$%^&*()+-=,.<>/?;:'[{]}|`~\n\t\\\0\"\b\a\f\v");
    
    # Remove leading numbers.
    while(name[0].isdigit()):
        name = name[1:]
    
    # Finally we return the results of our labor.
    return name
        
"""
Simply prints usage methodology.

Parameters: None.

Returns: None.
"""
def printUsage():
    print("usage:")
    print("\tglsl-to-header <output filepath> <filepath A> <filepath B> ...")
    
"""
Main function of the script.
"""
def main():

    # Provide the user with help if requested.
    if("-h" in sys.argv or "--help" in sys.argv):
        printUsage()
        return
        
    # Check to make sure we've got the correct number of arguments.
    if(len(sys.argv) == 1):
        printUsage()
    if(len(sys.argv) == 2):
        print("It seems you've either not specified even one GLSL file, or a single GLSL file and no output file. See -h or --help for usage.")
        
    # Now we go ahead and make the header file.
    output = ''
    for filename in sys.argv[2:]:
        print('Currently converting "'+str(filename)+'"')
        output += createDefineDirective(filenameToMacroName(filename),filename)
        output += '\n\n'
    
    # Now that the header file is made, we need to write it to a file.
    print('Writing to file "'+sys.argv[1]+'"')
    
    # If you try to write to a directory that doesn't exist, things tend to
    # blow up. Let's stop that before such happens.
    try:
        file = open(sys.argv[1], 'w')
    except IOError:
        print("Error: Could not create output file. Tip: If writing to a directory, it must exist first.")
        return
        
    # Write the header to the output file.
    file.write(output)
    print("Done creating shader header file. "+str(len(sys.argv[2:]))+" files were consolidated.")
    
"""
If we've been called directly, we need to execute.
"""
if( __name__ == "__main__"):
    main()
    

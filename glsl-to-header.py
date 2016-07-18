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

usage:
    glsl-to-header <output filepath> <filepath A> <filepath B> ...
"""

"""
Opens a given filename, and stores its contents as a #define macro
within a returned String.

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
    
    # Go through the file appending each line to the macro.
    for line in f:
        # We need to strip the newline out of the original line so we can
        # place a closing " and tab before the carriage return.
        line = line.translate(None, "\n")
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
    
    # Chop off the file extension.
    name = os.path.splitext(name)[0]
    
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
        print("Currently converting "+str(filename))
        output += createDefineDirective(filenameToMacroName(filename),filename)
    
    # Now that the header file is made, we need to write it to a file.
    print("Writing to file \""+sys.argv[1]+"\"")
    file = open(sys.argv[1], 'w')
    file.write(output)
    print("Done creating shader header file. "+str(len(sys.argv[2:]))+" files were consolidated.")
    
"""
If we've been called directly, we need to execute.
"""
if( __name__ == "__main__"):
    main()
    

#!/usr/local/bin/python

import sys

"""
File: glsl-to-header.py
Author: Gerard Geer (gerarddgeer@gmail.com)

This script simply takes the filenames passed to it as arguments, and
spits out their contents into several #define directives in a single
file.

usage:
    glsl-to-header <output filepath> <filepath A> <filepath B> ...
"""

"""
Opens a given filename, and stores its contents as a #define macro
within a returned String.

Parameters:
    name (String): The name of the directive.
    file (String): The filepath of the file to #define-ify.
"""
def createDefineDirective(name, file):

    # Start off the define macro.
    result = "#define " + str(name) + " "
    
    # Open the file.
    f = open(file)
    
    # Go through the file appending each line to the macro.
    for line in f:
        result += '"' + str(line) + '"  \\\n'
     
    # Finally we return the resultant macro.
    return result
        
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
    
    # Check to make sure we've got the correct number of arguments.
    if(len(sys.argv) == 0):
        printUsage()
    if(len(sys.argv) == 1):
        print("It seems you've either not specified even one GLSL file, or a single GLSL file and no output file. See -h or --help for usage.")
        
    # Provide the user with help if requested.
    if("-h" in sys.argv or "--help" in sys.argv):
        printUsage()
        return
    

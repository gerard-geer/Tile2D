# Set the compiler to Clang.
CC=clang++

# The linker.
LINK=ld

# The output name of the static library:
ST_NAME=libTile2d.a

# The output nmame of the dynamic library:
DY_NAME=libTile2d.so

# The source directory.
SRC_DIR=lib/

# The header directory.
HDR_DIR=include/

# The build directory.
BLD_DIR=bin/

# The test code directory.
TST_DIR=test/

# Compilation flags. Specifies to only compile (and not to link), as well as
# a custom include directory of HDR_DIR.
CFLAGS= -c -I $(HDR_DIR) 

# Linking flags to make sure everything is bound up tight.
LFLAGS= -lglfw -lGL -lGLU -lpng -lGLEW -lm -lz -ldl

all:
	@echo "Welcome to the Tile2D makefile!"
	@echo "This makes a static or shared library, a pile of .o files, or tests for each."
	@echo "Please run again with one of the rules below:"
	@echo "clean        - Clears out the build directory \"$(BLD_DIR)\""
	@echo "OBJ_FILES    - Compiles Tile2D into .o files. For those who enjoy linking."
	@echo "STATIC       - Compiles Tile2D into a static library called \"$(ST_NAME)\"."
	@echo "DYNAMIC      - Compiles Tile2D into a dynamic library named \"$(DY_NAME)\""
	@echo "TEST_STATIC  - Creates and tests the static library against a test program."
	@echo "TEST_DYNAMIC - Creates and tests the dynamic library against a test program."

# Cleans the build directory and deletes it.
clean:
	@echo "Cleaning out and deleting build directory \"$(BLD_DIR)\"."
	@rm -r -f $(BLD_DIR)
	@echo "Done cleaning."
	
# Creates the build directory if it doesn't exist, then navigates to
# it.
setup_dir:
	@echo "Creating build directory \"$(BLD_DIR)\"."
	@mkdir -p $(BLD_DIR)
	@echo "Done creating build directory."

# Compiles all of Tile2D's source into .o files.
OBJ_FILES: setup_dir 
	
	@echo "Compiling PIC object files and placing them in the $(BLD_DIR) directory."
	@echo "  -Asset.cpp"
	@$(CC) $(CFLAGS) $(SRC_DIR)Asset.cpp -o $(BLD_DIR)Asset.o -fPIC
	@echo "  -AssetManager.cpp"
	@$(CC) $(CFLAGS) $(SRC_DIR)AssetManager.cpp -o $(BLD_DIR)AssetManager.o -fPIC
	@echo "  -Texture.cpp"
	@$(CC) $(CFLAGS) $(SRC_DIR)Texture.cpp -o $(BLD_DIR)Texture.o -fPIC
	@echo "  -Camera.cpp"
	@$(CC) $(CFLAGS) $(SRC_DIR)Camera.cpp -o $(BLD_DIR)Camera.o -fPIC
	@echo "  -ShaderUniform.cpp"
	@$(CC) $(CFLAGS) $(SRC_DIR)ShaderUniform.cpp -o $(BLD_DIR)ShaderUniform.o -fPIC
	@echo "  -Shader.cpp"
	@$(CC) $(CFLAGS) $(SRC_DIR)Shader.cpp -o $(BLD_DIR)Shader.o -fPIC
	@echo "  -BasicMatrix.cpp"
	@$(CC) $(CFLAGS) $(SRC_DIR)BasicMatrix.cpp -o $(BLD_DIR)BasicMatrix.o -fPIC
	@echo "  -Tile.cpp"
	@$(CC) $(CFLAGS) $(SRC_DIR)Tile.cpp -o $(BLD_DIR)Tile.o -fPIC
	@echo "  -BGTile.cpp"
	@$(CC) $(CFLAGS) $(SRC_DIR)BGTile.cpp -o $(BLD_DIR)BGTile.o -fPIC
	@echo "  -SceneTile.cpp"
	@$(CC) $(CFLAGS) $(SRC_DIR)SceneTile.cpp -o $(BLD_DIR)SceneTile.o -fPIC
	@echo "  -AnimTile.cpp"
	@$(CC) $(CFLAGS) $(SRC_DIR)AnimTile.cpp -o $(BLD_DIR)AnimTile.o -fPIC
	@echo "  -PostTile.cpp"
	@$(CC) $(CFLAGS) $(SRC_DIR)PostTile.cpp -o $(BLD_DIR)PostTile.o -fPIC
	@echo "  -Framebuffer.cpp"
	@$(CC) $(CFLAGS) $(SRC_DIR)Framebuffer.cpp -o $(BLD_DIR)Framebuffer.o -fPIC
	@echo "  -Renderer.cpp"
	@$(CC) $(CFLAGS) $(SRC_DIR)Renderer.cpp -o $(BLD_DIR)Renderer.o -fPIC
	@echo "  -Window.cpp"
	@$(CC) $(CFLAGS) $(SRC_DIR)Window.cpp -o $(BLD_DIR)Window.o -fPIC
	@echo "Done creating object files."
	
# Compiles all of Tile2D into object files then archives them into a
# static library.
STATIC: OBJ_FILES
	@echo "Using ar to create static library in \"$(BLD_DIR)\" preserving original timestamps."
	@ar rc $(BLD_DIR)$(ST_NAME) $(BLD_DIR)Asset.o $(BLD_DIR)AssetManager.o \
							   $(BLD_DIR)Texture.o $(BLD_DIR)ShaderUniform.o \
							   $(BLD_DIR)Shader.o $(BLD_DIR)Camera.o \
							   $(BLD_DIR)Framebuffer.o $(BLD_DIR)Renderer.o \
							   $(BLD_DIR)Window.o $(BLD_DIR)BasicMatrix.o \
							   $(BLD_DIR)Tile.o $(BLD_DIR)BGTile.o \
							   $(BLD_DIR)SceneTile.o $(BLD_DIR)AnimTile.o \
							   $(BLD_DIR)PostTile.o -o -v
	@echo "Done archiving."
							  
# Compiles Tile2D and links it up with its dependencies (you better have them)
# into a dynamic library.
DYNAMIC: OBJ_FILES
	@echo "Creating shared library \"$(DY_NAME)\" in \"$(BLD_DIR)\"."
	@$(CC) -shared $(BLD_DIR)Asset.o $(BLD_DIR)AssetManager.o \
				$(BLD_DIR)Texture.o $(BLD_DIR)ShaderUniform.o \
				$(BLD_DIR)Shader.o $(BLD_DIR)Camera.o \
				$(BLD_DIR)Framebuffer.o $(BLD_DIR)Renderer.o \
				$(BLD_DIR)Window.o $(BLD_DIR)BasicMatrix.o \
				$(BLD_DIR)Tile.o $(BLD_DIR)BGTile.o \
				$(BLD_DIR)SceneTile.o $(BLD_DIR)AnimTile.o \
				$(BLD_DIR)PostTile.o \
				$(LFLAGS) -o $(BLD_DIR)$(DY_NAME)
	@echo "Done creating shared library."
	
# Compiles main.cpp
COMP_MAIN: OBJ_FILES
	@echo "Compiling test program into PIC object file."
	@$(CC) $(CFLAGS) $(TST_DIR)main.cpp -o $(BLD_DIR)main.o
	

# Compiles the library and runs the test application. No libraries involved.
TEST: OBJ_FILES COMP_MAIN
	@echo "Linking object files."
	$(CC) -o $(BLD_DIR)test $(BLD_DIR)*.o -lglfw -lGL -lGLU -lpng -lGLEW
	@echo "Adding execute permission."
	@chmod +x $(BLD_DIR)test
	@echo "Done creating test program. Run with command ./test from $(BLD_DIR)"
	
# Creates and runs the test program using the static library.
TEST_STATIC: STATIC COMP_MAIN
	@echo "Linking \"$(TST_DIR)main.cpp\" using the static library."
	@$(CC) -o $(BLD_DIR)static_test $(BLD_DIR)*.o -L$(BLD_DIR) -lTile2d -lglfw -lGL -lGLU -lpng -lGLEW
	@echo "Adding execute permission."
	chmod +x $(BLD_DIR)static_test
	@echo "Done creating test program. Run with command ./static_test from $(BLD_DIR)"
	
# Creates and runs the test program using the dynamic library.
TEST_DYNAMIC: DYNAMIC COMP_MAIN
	@echo "Linking \"$(TST_DIR)main.cpp\" using the dynamic library."
	@$(CC) -o $(BLD_DIR)dynamic_test $(BLD_DIR)*.o -L$(BLD_DIR) -lTile2d -lglfw -lGL -lGLU -lpng -lGLEW
	@echo "Adding execute permission."
	chmod +x $(BLD_DIR)dynamic_test
	@echo "Done creating test program. Run with command ./dynamic_test from $(BLD_DIR)"
	@echo "NOTE: By default the OS will not see the .so file in \"$(BLD_DIR)\". You need to make it see it."
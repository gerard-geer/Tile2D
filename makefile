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

# Cleans the build directory and deletes it.
clean:
	@echo "Cleaning out and deleting build directory \"$(BLD_DIR)\"..."
	@rm -r -f $(BLD_DIR)
	@echo "...Done."
	
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
	@$(CC) $(CFLAGS) $(SRC_DIR)Asset.cpp -o $(BLD_DIR)Asset.o
	@echo "  -AssetManager.cpp"
	@$(CC) $(CFLAGS) $(SRC_DIR)AssetManager.cpp -o $(BLD_DIR)AssetManager.o
	@echo "  -Texture.cpp"
	@$(CC) $(CFLAGS) $(SRC_DIR)Texture.cpp -o $(BLD_DIR)Texture.o
	@echo "  -Camera.cpp"
	@$(CC) $(CFLAGS) $(SRC_DIR)Camera.cpp -o $(BLD_DIR)Camera.o
	@echo "  -ShaderUniform.cpp"
	@$(CC) $(CFLAGS) $(SRC_DIR)ShaderUniform.cpp -o $(BLD_DIR)ShaderUniform.o
	@echo "  -Shader.cpp"
	@$(CC) $(CFLAGS) $(SRC_DIR)Shader.cpp -o $(BLD_DIR)Shader.o
	@echo "  -BasicMatrix.cpp"
	@$(CC) $(CFLAGS) $(SRC_DIR)BasicMatrix.cpp -o $(BLD_DIR)BasicMatrix.o
	@echo "  -Tile.cpp"
	@$(CC) $(CFLAGS) $(SRC_DIR)Tile.cpp -o $(BLD_DIR)Tile.o
	@echo "  -BGTile.cpp"
	@$(CC) $(CFLAGS) $(SRC_DIR)BGTile.cpp -o $(BLD_DIR)BGTile.o
	@echo "  -SceneTile.cpp"
	@$(CC) $(CFLAGS) $(SRC_DIR)SceneTile.cpp -o $(BLD_DIR)SceneTile.o
	@echo "  -AnimTile.cpp"
	@$(CC) $(CFLAGS) $(SRC_DIR)AnimTile.cpp -o $(BLD_DIR)AnimTile.o
	@echo "  -PostTile.cpp"
	@$(CC) $(CFLAGS) $(SRC_DIR)PostTile.cpp -o $(BLD_DIR)PostTile.o
	@echo "  -Framebuffer.cpp"
	@$(CC) $(CFLAGS) $(SRC_DIR)Framebuffer.cpp -o $(BLD_DIR)Framebuffer.o
	@echo "  -Renderer.cpp"
	@$(CC) $(CFLAGS) $(SRC_DIR)Renderer.cpp -o $(BLD_DIR)Renderer.o
	@echo "  -Window.cpp"
	@$(CC) $(CFLAGS) $(SRC_DIR)Window.cpp -o $(BLD_DIR)Window.o
	@echo "Done creating object files."
	
# Compiles all of Tile2D into object files then archives them into a
# static library.
STATIC: OBJ_FILES
	@echo "Using ar to create static library in \"$(BLD_DIR)\" preserving original timestamps."
	@ar -rcs $(BLD_DIR)$(ST_NAME) $(BLD_DIR)Asset.o $(BLD_DIR)AssetManager.o \
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

# Compiles the library and runs the test application. No libraries involved.
TEST: OBJ_FILES
	@echo "Compiling test program into PIC object file."
	@$(CC) $(CFLAGS) $(TST_DIR)main.cpp -o $(BLD_DIR)main.o
	@echo "Linking object files."
	$(CC) -o $(BLD_DIR)test $(BLD_DIR)*.o -L. -lglfw -lGL -lGLU -lpng -lGLEW
	@echo "Adding execute permission."
	@chmod +x $(BLD_DIR)test
	@echo "Done creating test program. Run with command ./test from $(BLD_DIR)"
	
# Creates and runs the test program using the static library.
TEST_STATIC: STATIC
	#echo "Building \"$(TST_DIR)main.cpp\" using the static library..."
	$(CC) $(CFLAGS) -static $(TST_DIR)main.cpp -L. -lTile2d $(L_FLAGS) -o $(BLD_DIR)static_test
	#echo "...Done. Making executable..."
	chmod +x $(BLD_DIR)static_test
	#echo "...Done. Running..."
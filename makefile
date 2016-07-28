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

# The shader source directory.
SDR_DIR=shaders/

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
	
# Compiles the shader source code files.
SHADERS:
	@echo "Consolidating shaders into header file named \"shader_source.h\""
	@rm -f $(HDR_DIR)shader_source.h
	@python glsl-to-header.py $(HDR_DIR)shader_source.h \
							  $(SDR_DIR)bg_tile_shader.vert    $(SDR_DIR)bg_tile_shader.frag       \
							  $(SDR_DIR)scene_tile_shader.vert $(SDR_DIR)scene_tile_shader.frag \
							  $(SDR_DIR)anim_tile_shader.vert  $(SDR_DIR)anim_tile_shader.frag   \
							  $(SDR_DIR)final_pass_shader.vert $(SDR_DIR)final_pass_shader.frag

# Compiles all of Tile2D's source into .o files.
OBJ_FILES_MESSAGE: setup_dir 
	@echo "Compiling PIC object files and placing them in the $(BLD_DIR) directory."
	
# An implicit (suffix) rule to build any file in the SRC and HDR directories, into the BLD directory.
$(BLD_DIR)%.o: $(SRC_DIR)%.cpp $(HDR_DIR)%.h
	@echo "  -$<"
	@$(CC) $(CFLAGS) $< -o $@ -fPIC
	
# A shortcut rule for the above.
%.o: $(BLD_DIR)%.o

	
OBJ_FILES: SHADERS OBJ_FILES_MESSAGE $(BLD_DIR)Asset.o $(BLD_DIR)AssetManager.o $(BLD_DIR)Texture.o	\
		   $(BLD_DIR)Camera.o $(BLD_DIR)ShaderUniform.o $(BLD_DIR)Shader.o $(BLD_DIR)BasicMatrix.o	\
		   $(BLD_DIR)Tile.o $(BLD_DIR)BGTile.o $(BLD_DIR)SceneTile.o $(BLD_DIR)AnimTile.o	\
		   $(BLD_DIR)PostTile.o $(BLD_DIR)Framebuffer.o $(BLD_DIR)Renderer.o $(BLD_DIR)Window.o
	@echo "Done creating object files. Note: some may not have been recompiled."
	
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
COMP_MAIN:
	# Asset.o had to have been created if OBJ_FILES was run.
	@test -s $(BLD_DIR)Asset.o || { echo "Object files not created! Run \"make OBJ_FILES\" first."; exit 1; }
	@echo "Compiling test program into PIC object file."
	@$(CC) $(CFLAGS) $(TST_DIR)main.cpp -o $(BLD_DIR)main.o

COMP_TEST_SHADER_TOOLCHAIN:
	# Asset.o had to have been created if OBJ_FILES was run.
	@test -s $(BLD_DIR)Asset.o || { echo "Object files not created! Run \"make OBJ_FILES\" first."; exit 1; }
	@echo "Compiling test program into PIC object file."
	@$(CC) $(CFLAGS) $(TST_DIR)test_shader_toolchain.cpp -o $(BLD_DIR)test_shader_toolchain.o

# Compiles the library and runs the test application. No libraries involved.
TEST: OBJ_FILES COMP_MAIN
	@echo "Linking object files."
	$(CC) -o $(BLD_DIR)test $(BLD_DIR)*.o -lglfw -lGL -lGLU -lpng -lGLEW
	@echo "Adding execute permission."
	@chmod +x $(BLD_DIR)test
	@echo "Done creating test program. Run with command ./test from $(BLD_DIR)"
	
# Creates and runs the test program using the static library.
TEST_STATIC: COMP_MAIN
	@test -s $(BLD_DIR)$(ST_NAME) || { echo "Static library not created! Run \"make STATIC\" first."; exit 1; }
	@echo "Linking \"$(TST_DIR)main.cpp\" using the static library."
	@$(CC) -o $(BLD_DIR)static_test $(BLD_DIR)*.o -L$(BLD_DIR) -lTile2d -lglfw -lGL -lGLU -lpng -lGLEW
	@echo "Adding execute permission."
	chmod +x $(BLD_DIR)static_test
	@echo "Done creating test program. Run with command ./static_test from $(BLD_DIR)"
	
# Creates and runs the test program using the dynamic library.
TEST_DYNAMIC: COMP_MAIN
	@echo "Have you run make DYNAMIC yet, and made the library visible?"
	@echo "Linking \"$(TST_DIR)main.cpp\" using the dynamic library."
	@$(CC) -o $(BLD_DIR)dynamic_test $(BLD_DIR)*.o -L$(BLD_DIR) -lTile2d -lglfw -lGL -lGLU -lpng -lGLEW
	@echo "Adding execute permission."
	chmod +x $(BLD_DIR)dynamic_test
	@echo "Done creating test program. Run with command ./dynamic_test from $(BLD_DIR)"
	@echo "NOTE: By default the OS will not see the .so file in \"$(BLD_DIR)\". You need to make it see it."
	
TEST_SHADER_TOOLCHAIN: COMP_TEST_SHADER_TOOLCHAIN
	@test -s $(BLD_DIR)Asset.o || { echo "Object files not created! Run \"make OBJ_FILES\" first."; exit 1; }
	@echo "Linking object files."
	$(CC) -o $(BLD_DIR)test_shader_toolchain $(BLD_DIR)*.o -lglfw -lGL -lGLU -lpng -lGLEW
	@echo "Adding execute permission."
	@chmod +x $(BLD_DIR)test_shader_toolchain
	@echo "Done creating test program. Run with command ./test_shader_toolchain from $(BLD_DIR)"
	

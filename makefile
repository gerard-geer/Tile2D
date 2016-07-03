# Set the compiler to Clang.
CC=clang++

# The source directory.
SRC_DIR=lib/

# The header directory.
HDR_DIR=include/

# The build directory.
BLD_DIR=bin/

# Compilation flags. Specifies to only compile (and not to link), as well as
# a custom include directory of HDR_DIR.
CFLAGS= -c -I $(HDR_DIR)

# Creates the build directory if it doesn't exist, then navigates to
# it.
setup_dir:
	@echo "Creating build directory \"$(BLD_DIR)\""
	@mkdir -p $(BLD_DIR)

# Compiles the Asset abstract class.
Asset.o:
	@echo "$@..."
	@$(CC) $(CFLAGS) $(SRC_DIR)Asset.cpp -o $(BLD_DIR)Asset.o

# Compiles the AssetManager class.
AssetManager.o:
	@echo "$@..." 
	@$(CC) $(CFLAGS) $(SRC_DIR)AssetManager.cpp -o $(BLD_DIR)AssetManager.o

# Compiles the Texture class.
Texture.o: Asset.o
	@echo "$@..."
	@$(CC) $(CFLAGS) $(SRC_DIR)Texture.cpp -o $(BLD_DIR)Texture.o

# Comptiles the Camera class.
Camera.o:
	@echo "$@..."
	@$(CC) $(CFLAGS) $(SRC_DIR)Camera.cpp -o $(BLD_DIR)Camera.o

# Compiles the ShaderUniform class.
ShaderUniform.o:
	@echo "$@..."
	@$(CC) $(CFLAGS) $(SRC_DIR)ShaderUniform.cpp -o $(BLD_DIR)ShaderUniform.o
	
# Compiles the Shader class.
Shader.o: 
	@echo "$@..."
	@$(CC) $(CFLAGS) $(SRC_DIR)Shader.cpp -o $(BLD_DIR)Shader.o

# Compiles the BasicMatrix class.
BasicMatrix.o:
	@echo "$@..."
	@$(CC) $(CFLAGS) $(SRC_DIR)BasicMatrix.cpp -o $(BLD_DIR)BasicMatrix.o

# Compiles the Tile class.
Tile.o:
	@echo "$@..."
	@$(CC) $(CFLAGS) $(SRC_DIR)Tile.cpp -o $(BLD_DIR)Tile.o

# Compiles the BGTile class.
BGTile.o:
	@echo "$@..."
	@$(CC) $(CFLAGS) $(SRC_DIR)BGTile.cpp -o $(BLD_DIR)BGTile.o

# Compiles the SceneTile class.
SceneTile.o:
	@echo "$@..."
	@$(CC) $(CFLAGS) $(SRC_DIR)SceneTile.cpp -o $(BLD_DIR)SceneTile.o

# Compiles the AnimTile class.
AnimTile.o:
	@echo "$@..."
	@$(CC) $(CFLAGS) $(SRC_DIR)AnimTile.cpp -o $(BLD_DIR)AnimTile.o
	
# Compiles the PostTile class.
PostTile.o:
	@echo "$@..."
	@$(CC) $(CFLAGS) $(SRC_DIR)PostTile.cpp -o $(BLD_DIR)PostTile.o

# Compiles the Framebuffer class.
Framebuffer.o:
	@echo "$@..."
	@$(CC) $(CFLAGS) $(SRC_DIR)Framebuffer.cpp -o $(BLD_DIR)Framebuffer.o
	
# Compiles the Renderer class.
Renderer.o:
	@echo "$@..."
	@$(CC) $(CFLAGS) $(SRC_DIR)Renderer.cpp -o $(BLD_DIR)Renderer.o
	
# Compiles the Window class.
Window.o:
	@echo "$@..."
	@$(CC) $(CFLAGS) $(SRC_DIR)Window.cpp -o $(BLD_DIR)Window.o

# A message to preceed creating the object files.
OBJ_MESSAGE:
	@echo "Creating object files and placing them in the $(BLD_DIR) directory..."

# Compiles all of Tile2D's source into .o files.
OBJ_ONLY: OBJ_MESSAGE setup_dir Asset.o AssetManager.o Texture.o ShaderUniform.o \
		  Shader.o Camera.o Framebuffer.o Renderer.o Window.o BasicMatrix.o \
		  Tile.o BGTile.o SceneTile.o AnimTile.o PostTile.o
	@echo "...Done creating object files."
	
	

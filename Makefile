TARGET = app.exe
SRC_DIR = src
MODULE_DIR = src/modules
INTERFACE_DIR = src/interfaces
BUILD_DIR = bin
OBJ_DIR = $(BUILD_DIR)/src
INCLUDE_DIR = sfml/include
LIB_DIR = sfml/lib

CXX = g++
CXXFLAGS = -std=c++17 -g3 -Wall -Wextra
LDFLAGS = -L$(LIB_DIR) -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio 
INC = -I$(INCLUDE_DIR) -I$(SRC_DIR) -I$(INTERFACE_DIR) -I$(MODULE_DIR)/animation/src -I$(MODULE_DIR)/texture_manager/src

# Gets all .cpp files and transform them into .o files
SOURCES = $(wildcard $(SRC_DIR)/*.cpp) $(MODULE_DIR)/animation/src/Animation.cpp $(MODULE_DIR)/texture_manager/src/ResourceManager.cpp 
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SOURCES)) 

# Instructions to compile any .cpp file into a .o file
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	if not exist $(subst /,\,$(dir $@)) mkdir $(subst /,\,$(dir $@))
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

# Creates the .exe file
compile: $(OBJECTS)
	g++ -o $(BUILD_DIR)/$(TARGET) $^ $(LDFLAGS) 

# Delete the .exe file and the generated .o files
clean:
	if exist $(subst /,\,$(BUILD_DIR)\$(TARGET)) del /q /f $(subst /,\,$(BUILD_DIR)\$(TARGET))
	if exist $(subst /,\,$(OBJ_DIR)) rmdir /q /s $(subst /,\,$(OBJ_DIR))	

print-%:
	@echo $* = $($*)

.PHONY: clean compile
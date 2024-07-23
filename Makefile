TARGET = app.exe
SRC_DIR = src
BUILD_DIR = bin
OBJ_DIR = $(BUILD_DIR)/src
INCLUDE_DIR = sfml/include
LIB_DIR = sfml/lib

CXX = g++
CXXFLAGS = -std=c++14 -Wall -Wextra -g
LDFLAGS = -L$(LIB_DIR) -lsfml-graphics -lsfml-window -lsfml-system
INC = -I$(INCLUDE_DIR) -I$(SRC_DIR) -I$(SRC_DIR)/domain -I$(SRC_DIR)/domain/interfaces

# Gets all .cpp files and transform them into .o files
SOURCES = $(wildcard $(SRC_DIR)/**/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SOURCES))
OBJECTS += $(OBJ_DIR)/main.o

# Instructions to compile any .cpp file into a .o file
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	if not exist $(subst /,\,$(dir $@)) mkdir $(subst /,\,$(dir $@))
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

# Creates the .exe file
compile: $(OBJECTS)
	g++ -o $(BUILD_DIR)/$(TARGET) $^ $(LDFLAGS)

# Delete the .exe file and the generated .o files
clean:
	del /q /f $(subst /,\,$(BUILD_DIR)\$(TARGET))
	if exist $(subst /,\,$(OBJ_DIR)) rmdir /q /s $(subst /,\,$(OBJ_DIR))	

print-%:
	@echo $* = $($*)

.PHONY: clean compile
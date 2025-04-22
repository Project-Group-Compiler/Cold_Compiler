# Variables
SRC_DIR = src
BIN_DIR = bin
BUILD_DIR = build

LEXER = $(SRC_DIR)/lexer.l
PARSER = $(SRC_DIR)/parser.y
LEXER_CPP = $(BUILD_DIR)/lexer.cpp
PARSER_CPP = $(BUILD_DIR)/parser.cpp
PARSER_HPP = $(BUILD_DIR)/parser.hpp
PARSER_HEADER_HPP = $(SRC_DIR)/parserHeader.hpp
AST_HPP = $(SRC_DIR)/AST.hpp
AST_CPP = $(SRC_DIR)/AST.cpp
DS_HPP = $(SRC_DIR)/data_structures.hpp
DS_CPP = $(SRC_DIR)/data_structures.cpp
TYPES_HPP = $(SRC_DIR)/types.hpp
TAC_HPP = $(SRC_DIR)/tac.hpp
TAC_GEN_CPP = $(SRC_DIR)/tac_gen.cpp
TAC_OPT_CPP = $(SRC_DIR)/tac_opt.cpp
X86_32_LIB_HPP = $(SRC_DIR)/x86_32_lib.hpp
ASM_HPP = $(SRC_DIR)/asm.hpp
ASM_GEN_CPP = $(SRC_DIR)/asm_gen.cpp
DRIVER = $(SRC_DIR)/driver.cpp


# Final binary output
OUTPUT = $(BIN_DIR)/compiler

# Object files including new ones
OBJS = \
	$(BUILD_DIR)/lexer.o \
	$(BUILD_DIR)/parser.o \
	$(BUILD_DIR)/AST.o \
	$(BUILD_DIR)/data_structures.o \
	$(BUILD_DIR)/tac_gen.o \
	$(BUILD_DIR)/tac_opt.o \
	$(BUILD_DIR)/asm_gen.o \
	$(BUILD_DIR)/driver.o

# Compiler and tools
FLEX = flex
BISON = bison
CXX = g++ 
# CXXFLAGS = -I$(SRC_DIR)
CXXFLAGS = -std=c++17 -I$(SRC_DIR) -g -O0 # For debug

# Default target
all: $(OUTPUT)

# Create build directory if it doesn't exist
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

# Generate lexer C++ source from lexer.l
$(LEXER_CPP): $(LEXER) | $(BUILD_DIR)
	@$(FLEX) -o $(LEXER_CPP) $(LEXER)

# Generate parser C++ source and header from parser.y
$(PARSER_CPP) $(PARSER_HPP): $(PARSER) | $(BUILD_DIR)
	@$(BISON) -d -o $(PARSER_CPP) $(PARSER) -Wno-conflicts-rr -Wno-conflicts-sr

# Compile lexer object file
$(BUILD_DIR)/lexer.o: $(LEXER_CPP) $(PARSER_HPP) $(AST_HPP) $(DS_HPP) $(TYPES_HPP)
	@$(CXX) $(CXXFLAGS) -c -o $@ $(LEXER_CPP)

# Compile parser object file
$(BUILD_DIR)/parser.o: $(PARSER_CPP) $(PARSER_HPP) $(PARSER_HEADER_HPP) $(AST_HPP) $(DS_HPP) $(TYPES_HPP) $(TAC_HPP)
	@$(CXX) $(CXXFLAGS) -c -o $@ $(PARSER_CPP)

# Compile AST object file
$(BUILD_DIR)/AST.o: $(AST_CPP) $(AST_HPP)
	@$(CXX) $(CXXFLAGS) -c -o $@ $(AST_CPP)

# Compile data_structures object file
$(BUILD_DIR)/data_structures.o: $(DS_CPP) $(DS_HPP)
	@$(CXX) $(CXXFLAGS) -c -o $@ $(DS_CPP)

# Compile the TAC generator object file
$(BUILD_DIR)/tac_gen.o: $(TAC_GEN_CPP) $(TAC_HPP) $(DS_HPP) $(TYPES_HPP)
	@$(CXX) $(CXXFLAGS) -c -o $@ $(TAC_GEN_CPP)

# Compile the TAC optimizer object file
$(BUILD_DIR)/tac_opt.o: $(TAC_OPT_CPP) $(TAC_HPP)
	@$(CXX) $(CXXFLAGS) -c -o $@ $(TAC_OPT_CPP)

# Compile assembly generator object file
$(BUILD_DIR)/asm_gen.o: $(ASM_GEN_CPP) $(ASM_HPP) $(TAC_HPP) $(DS_HPP) $(X86_32_LIB_HPP)
	@$(CXX) $(CXXFLAGS) -c -o $@ $(ASM_GEN_CPP)

# Compile driver object file
$(BUILD_DIR)/driver.o: $(DRIVER) $(DS_HPP) $(TAC_HPP) $(ASM_HPP)
	@$(CXX) $(CXXFLAGS) -c -o $@ $(DRIVER)

# Link all object files into the final binary
$(OUTPUT): $(OBJS)
	@mkdir -p $(BIN_DIR)
	@$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

files_clean:
	rm -f *.csv *_debug_file.txt *_lexfile.txt *_IR.txt *.asm *_AST.dot Descriptor.txt

.PHONY: all clean files_clean

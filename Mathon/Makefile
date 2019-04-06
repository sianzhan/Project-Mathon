#
# Makefile
#
# Final Project
#

#run alias make='make' to remove useless always-make of makefile
CC = g++-6
CFLAG = -std=c++11 -Wall -O -g -MMD -fmax-errors=5#-ftime-report 
#LDFLAG = -std=c++11
INC =-Iinclude -Iinclude/Sugaku -Iinclude/Sugaku/Integer
SRC_FILES = $(shell find src -name '*.cpp')
#SRC_FILES = $(wildcard src/*.cpp)

OBJ_DIR = obj
OBJ_FILES = $(addprefix $(OBJ_DIR)/,$(notdir $(SRC_FILES:.cpp=.o)))
DEPS = $(OBJ_FILES:.o=.d)

calc: $(OBJ_FILES) Makefile
	$(CC) $(LDFLAG) $(OBJ_FILES) -o calc
	
-include $(DEPS)

$(OBJ_DIR)/%.o: src/%.cpp
	$(CC) $(CFLAG) $(INC) -c -o $@ $<
	
$(OBJ_DIR)/%.o: src/Sugaku/%.cpp
	$(CC) $(CFLAG) $(INC) -c -o $@ $<

#Tools.h
$(OBJ_DIR)/%.o: src/Sugaku/Tools/%.cpp
	$(CC) $(CFLAG) $(INC) -c -o $@ $<
clean:
	rm -f calc
	rm -f obj/*.o
	rm -f obj/*.d
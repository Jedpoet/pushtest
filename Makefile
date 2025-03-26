#
# Usage:
# 'make'        build executable file 'main'
# 'make clean'  removes all .o and executable files
# 'make run'    runs the executable file 'main'
#

# the name of this package. 
# The library and tar file, if any, will be named after it.
PACKAGE  = BJ

# define library paths in addition to /usr/lib
#   if I wanted to include libraries not in /usr/lib I'd specify
#   their path using -Lpath, something like:
LFLAGS =

# define output directory
OUTPUT	:= output
# define source directory
SRC		:= src
# define object directory
OBJ_DIR	:= obj

# You may not need to modify anything below

# define the Cpp compiler to use
CXX = g++
# define any compile-time flags
CXXFLAGS	:= -std=c++17 -Wall -Wno-unused-variable -g

ifeq ($(OS),Windows_NT)
MAIN	:= $(PACKAGE).exe
SOURCEDIRS	:= $(SRC)
FIXPATH = $(subst /,\,$1)
RM			:= del /q /f
MD	:= mkdir
RD	:= rmdir
else
MAIN	:= $(PACKAGE)
SOURCEDIRS	:= $(shell find $(SRC) -type d)
FIXPATH = $1
RM = rm -f
MD	:= mkdir -p
RD	:= rm -rf
endif

# define the C source files
SOURCES		:= $(wildcard $(patsubst %,%/*.cpp, $(SOURCEDIRS)))
# define the C object files
OBJECTS		:= $(SOURCES:$(SRC)/%.cpp=$(OBJ_DIR)/%.o)
# define the dependency output files
DEPS		:= $(SOURCES:$(SRC)/%.cpp=$(OBJ_DIR)/%.d)

#
# The following part of the makefile is generic; it can be used to
# build any executable just by changing the definitions above and by
# deleting dependencies appended to the file from 'make depend'
#

OUTPUTMAIN	:= $(call FIXPATH,$(OUTPUT)/$(MAIN))

all:  $(OBJ_DIR) $(OUTPUT) $(MAIN)
	@echo Executing 'all' complete!

OBJ_SUBDIRS := $(patsubst $(SRC)/%,$(OBJ_DIR)/%,$(SOURCEDIRS))

$(OBJ_DIR): $(OBJ_SUBDIRS)
	$(MD) $(OBJ_DIR) $(OBJ_SUBDIRS)

$(OBJ_SUBDIRS):
	$(MD) $@

$(OUTPUT):
	$(MD) $(OUTPUT)

$(MAIN): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(OUTPUTMAIN) $(OBJECTS) $(LFLAGS)

# include all .d files
-include $(DEPS)

# this is a suffix replacement rule for building .o's and .d's from .c's
# it uses automatic variables $<: the name of the prerequisite of
# the rule(a .c file) and $@: the name of the target of the rule (a .o file)
# -MMD generates dependency output files same name as the .o file
# (see the gnu make manual section about automatic variables)
.suffix: .o .cpp
$(OBJ_DIR)/%.o: $(SRC)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -MMD $<  -o $@

.PHONY: clean
clean:
	$(RM) $(OUTPUTMAIN)
	$(RM) $(call FIXPATH,$(OBJECTS))
	$(RM) $(call FIXPATH,$(DEPS))
	$(RD) $(OBJ_DIR)
	$(RD) $(OUTPUT)
	@echo Cleanup complete!

run: all
	./$(OUTPUTMAIN)
	@echo Executing 'run: all' complete!

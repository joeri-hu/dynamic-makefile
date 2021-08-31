# Dynamic makefile
#
# File:     makefile
# Version:  0.7
#
# Author:   Joeri Kok
# Date:     May 2021
#
# Description:
#   Generates makefile recipes by resolving file-header inclusions recursively.
#
# Known limitations:
#   Basically any project environment that isn't considered trivial. The search
#   mechanisms are implemented quite naively and can be exploited with relative
#   ease.
#
# To-do:
#   - Add support for separation of inc/src directories
#   - Improve search functionality robustness
#   - Write applicable unit-tests
#

OUT_FILE := program.exe

# target directories
SRC_DIR   := ./src
BUILD_DIR := ./build

# target file extensions
SRC_EXT := cpp
OBJ_EXT := o

# external libraries
TOOL_DIR := d:/arduino
SFML_DIR := $(TOOL_DIR)/sfml-2.5.1
SFML_INC := $(SFML_DIR)/include
SFML_LIB := $(SFML_DIR)/lib/x64

# general compiler flags
# note: the -Wno-parentheses is added due to compiler bug #94505
#       and can be omitted from GCC version 9.4 and onwards, refer to:
#       https://gcc.gnu.org/bugzilla/show_bug.cgi?id=94505
CPPFLAGS := -std=c++17 -pedantic -Wall -Wextra -Werror -Wno-parentheses
CPP := g++

# additional compiler flags
FIN_FLAGS   := -Ofast
INC_FLAGS   := -I $(SRC_DIR) -I $(SFML_INC)
LIB_FLAGS   := -L $(SFML_LIB)
STL_FLAGS   :=
MACRO_FLAGS :=

# platform specific configuration, assumes Linux if Windows was not detected
ifeq ($(OS),Windows_NT)
	# static linkage with SFML
	# note: to hide the console window, add '-mwindows' to the compiler flags
	SFML_STL   := -lsfml-main -lsfml-graphics-s -lsfml-window-s -lsfml-system-s
	WIN_STL    := -lgdi32 -lopengl32 -lws2_32 -lwinmm -lfreetype
	SFML_MACRO := SFML_STATIC

	MACRO_FLAGS += -D $(SFML_MACRO)
	STL_FLAGS   += $(SFML_STL) $(WIN_STL)

	# shell commands
	makedir = if not exist "$1" mkdir "$1"
	remove  = del $(patsubst %,"%",$(subst /,\,$1)) & (call;)
else
	# shell commands
	makedir = mkdir -p $1
	remove  = rm -f $1
endif

# generate the prerequisites by finding all file-header dependencies recursively,
# create the corresponding build directory if necessary and invoke the compiler
define createrecipe
$(BUILD_DIR)/$1.$(OBJ_EXT): $(call rfinddeps,$(SRC_DIR)/$1.$(SRC_EXT))
	$$(call makedir,$$(dir $$@))
	$(CPP) $$(CPPFLAGS) $(MACRO_FLAGS) -c $$< $(INC_FLAGS) -o $$@
endef

# find all file-header dependencies of source file $1 recursively, prefix
# the header files with either the directory of their includer or the source
# directory, discard any header file that was already included within the
# current recursion path, and remove unnecessary spaces
# note: the final result might contain duplicates (which will be ignored)
define rfinddeps
$1 $(strip $(foreach header,
	$(filter-out $2,$(call searchpath,$(dir $1),$(call findincs,$1))),
	$(call $0,$(header),$2 $(header))))
endef

# prefix files $2 with directory $1 (requires trailing forward slash)
# if that path exists, otherwise, use the source directory as prefix
searchpath = $(foreach f,$2,$(firstword $(wildcard $1$f) $(SRC_DIR)/$f))

# read file $1, remove newlines, collapse #include directives with the word following up,
# throw away the rest, remove the #include directives, and strip any remaining quotes
s = \#include
findincs = $(subst ",,$(subst $s,,$(filter $s%",$(subst $s ,$s,$(strip $(file < $1))))))
# "))") # fixes a markup issue with some editors

# slight alteration of recursive wildcard function: https://stackoverflow.com/a/12959764
rwildcard = $(wildcard $1$2) $(strip $(foreach d,$(wildcard $1*),$(call $0,$d/,$2)))

# find all source files within the source directory recursively,
# and remove the leading source directory and trailing file extension
targets := $(basename $(subst $(SRC_DIR)/,,$(call rwildcard,$(SRC_DIR)/,*.$(SRC_EXT))))

# add the build directory and object file extension to the targets
objects := $(addprefix $(BUILD_DIR)/,$(addsuffix .$(OBJ_EXT),$(targets)))

clean:
	$(call remove,$(objects))

build: $(objects)
	$(CPP) $(CPPFLAGS) $(MACRO_FLAGS) $(LIB_FLAGS) $(STL_FLAGS) $^ -o $(OUT_FILE)

final: CPPFLAGS += $(FIN_FLAG)
final: clean build

run:
	./$(basename $(OUT_FILE))

# create a recipe for every target
$(foreach target,$(targets),$(eval $(call createrecipe,$(target))))

.PHONY: clean build final run

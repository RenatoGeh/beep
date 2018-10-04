# Compiler
CXX ?= g++

# Paths
SRC_PATH = src
BUILD_PATH = build
BIN_PATH = $(BUILD_PATH)/bin

# Executable
BIN_NAME = beep

# Extensions
SRC_EXT = cc

# Finds sources
SRCS = $(shell find $(SRC_PATH) -name '*.$(SRC_EXT)' -printf '%T@ %p\n' | sort -k 1nr | cut -d ' ' -f 2)

# Objects
OBJS = $(SRCS:$(SRC_PATH)/%.$(SRC_EXT)=$(BUILD_PATH)/%.o)

# Dependencies
DEPS = $(OBJS:.o=.d)

# Flags
WARN_FLAGS = -Wall -Wextra -Wshadow -Wfloat-equal -Wwrite-strings -Wswitch-default \
						 -Wunreachable-code -Wno-unused-parameter
COMP_FLAGS = $(WARN_FLAGS) -std=c++11 -g
INCLUDES = -I include/ -I /usr/include/libircclient
LDLIBS = -lm -lircclient -lcrypto -lssl

# Cleaning command
RM = rm -f

.PHONY: def_target
def_target: release

.PHONY: release
release: export CXXFLAGS := $(CXXFLAGS) $(COMP_FLAGS)
release: dirs
	@$(MAKE) all

.PHONY: dirs
dirs:
	@echo "Creating directories"
	@mkdir -p $(dir $(OBJS))
	@mkdir -p $(BIN_PATH)

.PHONY: all
all: $(BIN_PATH)/$(BIN_NAME)
	@echo "Symlinking: $(BIN_NAME) -> $<"
	@$(RM) $(BIN_NAME)
	@ln -s $(BIN_PATH)/$(BIN_NAME) $(BIN_NAME)

$(BIN_PATH)/$(BIN_NAME): $(OBJS)
	@echo "Linking: $@"
	$(CXX) $(OBJS) -o $@ $(LDLIBS)

-include $(DEPS)

$(BUILD_PATH)/%.o: $(SRC_PATH)/%.$(SRC_EXT)
	@echo "Compiling: $< -> $@"
	$(CXX) $(CXXFLAGS) $(INCLUDES) -MP -MMD -c $< -o $@

.PHONY: clean
clean:
	$(RM) -r $(BUILD_PATH)/*

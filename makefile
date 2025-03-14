# Build directory
BUILD_DIR = cmake-build-debug

# Default build type
BUILD_TYPE ?= Debug

# Project name
PROJECT_NAME = "plotter2d"

# Targets
.PHONY: all clean rebuild configure build run

all: configure build

configure:
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake -DCMAKE_BUILD_TYPE=$(BUILD_TYPE) ..

build:
	@cmake --build $(BUILD_DIR)

clean:
	@rm -rf $(BUILD_DIR)

rebuild: clean all

run:
	@$(BUILD_DIR)/$(PROJECT_NAME)
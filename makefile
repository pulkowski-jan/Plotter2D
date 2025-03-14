# Build directory
BUILD_DIR = cmake-build-debug

# Default build type
BUILD_TYPE ?= Debug

# Targets
.PHONY: all clean rebuild configure build

all: configure build

configure:
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake -DCMAKE_BUILD_TYPE=$(BUILD_TYPE) ..

build:
	@cmake --build $(BUILD_DIR)

clean:
	@rm -rf $(BUILD_DIR)

rebuild: clean all

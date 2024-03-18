BUILDS = compile build clean

.PHONY := $(BUILDS)

.DEFAULT_GOAL := compile

compile :
	@echo "Compiling Project"
	cmake --build build
	@echo "Done Compiling Project."

build :
	@echo "Building CMake Project"
	cmake -S . -B build
	@echo "Done Building CMake Project"

clean :
	@echo "Cleaning CMake Project"
	rm -rf build
	@echo "Done Cleaning CMake Project"
	@echo "You need to run 'make build' to build the project again."

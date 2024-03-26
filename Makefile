BUILDS = release build clean

.PHONY := $(BUILDS)

.DEFAULT_GOAL := release

release :
	@echo "Compiling Project"
	cmake --build build/release
	@echo "Done Compiling Release."

debug :
	@echo "Compiling Debug"
	cmake --build build/debug
	@echo "Done Compiling Debug."

build :
	@echo "Building CMake Projects"
	cmake -S . -B build/debug -DCMAKE_BUILD_TYPE=Debug
	cmake -S . -B build/release -DCMAKE_BUILD_TYPE=Release
	@echo "Done Building CMake Projects"

docu :
	@echo "Create documentation"
	cmake --build build/release -t doxygen
	@echo "Documentation created"

tex_docu :
	@echo "Create documentation"
	cd docs/latex && make
	@echo "Documentation created"

clean :
	@echo "Cleaning CMake Project"
	rm -rf build
	@echo "Done Cleaning CMake Project"
	@echo "You need to run 'make build' to build the project again."

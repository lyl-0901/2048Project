all: build/core_2048.o build/cli_2048 build/gui_2048

.PHONY: create_dir clean

create_dir:
	mkdir -p build

build/core_2048.o: create_dir core-2048/core_2048.hpp core-2048/core_2048.cpp
	g++ -c core-2048/core_2048.cpp -o build/core_2048.o

build/cli_2048: create_dir build/core_2048.o cli-2048/cli_2048.cpp
	g++ cli-2048/cli_2048.cpp build/core_2048.o -o build/cli_2048 


build/gui_2048: create_dir build/core_2048.o $(wildcard gui_2048/*.cpp) $(wildcard gui_2048/*.hpp) gui-2048/gui_2048.pro
	cd gui-2048 && qmake gui_2048.pro && make

clean:
	rm -f build/core_2048.o build/cli_2048 build/gui_2048
	@if [ -f "gui-2048/Makefile" ]; then \
        cd gui-2048 && make clean; \
    fi
	cd gui-2048 && rm -f Makefile

CXXFLAGS = -I/include/nlohmann
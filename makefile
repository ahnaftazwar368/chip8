CXX=g++
SDL2CFLAGS=-I ./src/include -L ./src/lib -l SDL2-2.0.0
FILES=./src/*.cpp

CXXFLAGS=--std=c++20 $(SDL2CFLAGS)

exec: $(FILES)
	$(CXX) $(FILES) $(CXXFLAGS) -o chip8
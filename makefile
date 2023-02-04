CXX=g++
SDL2CFLAGS=-I include -L lib -l SDL2-2.0.0

CXXFLAGS=-O2 -c --std=c++14 -Wall $(SDL2CFLAGS)
LDFLAGS=-I include -L lib -l SDL2-2.0.0

exec: main.o
	$(CXX) $(LDFLAGS) -o chip8 main.o

main.o: main.cpp
	$(CXX) $(CXXFLAGS) main.cpp
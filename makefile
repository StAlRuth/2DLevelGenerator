all:
	g++ src/*.cpp -lsfml-system -lsfml-graphics -lsfml-window -o bin/exec -Wall -g -std=c++0x

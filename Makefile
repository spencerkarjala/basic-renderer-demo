CC = gcc
CCFLAGS = -g -Wall
FNAME = -o ./bin/FruitTetris
LINKS = -lglut -lGLU -lGL -lm -lstdc++
TDIR = ./src/Tetrominoes/Tetromino
TETROMINOES = $(TDIR).cpp $(TDIR)I.cpp $(TDIR)L.cpp $(TDIR)S.cpp $(TDIR)T.cpp

all: ./src/FruitTetris.cpp
	$(CC) $(CCFLAGS) $(FNAME) ./src/FruitTetris.cpp ./src/BoardMediator.cpp ./src/Board.cpp ./src/Fruit.cpp ./src/Renderer.cpp $(TETROMINOES) ./src/GameInstance.cpp $(LINKS)

clean:
	rm ./bin/FruitTetris

run:
	./bin/FruitTetris

valgrind:
	valgrind --leak-check=full ./bin/FruitTetris
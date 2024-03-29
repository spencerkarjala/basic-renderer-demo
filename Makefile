CC = gcc
CCFLAGS = -g -Wall
FNAME = -o ./FruitTetris3D
LINKS = -lglut -lGLU -lGL -lGLEW -lm -lstdc++ -I./lib/
TDIR = ./src/Tetrominoes/Tetromino
TETROMINOES = $(TDIR).cpp $(TDIR)I.cpp $(TDIR)L.cpp $(TDIR)S.cpp $(TDIR)T.cpp

all: ./src/FruitTetris.cpp
	$(CC) $(CCFLAGS) $(FNAME) ./src/FruitTetris.cpp ./src/BoardMediator.cpp ./src/Board.cpp ./src/Fruit.cpp ./src/Renderer.cpp $(TETROMINOES) ./src/GameInstance.cpp ./src/SimpleObject.cpp ./src/Grid.cpp ./src/Shader.cpp ./src/RobotArm.cpp ./src/Camera.cpp ./src/ObjReader.cpp ./src/Scene.cpp ./src/CompoundObject.cpp $(LINKS)

clean:
	rm ./FruitTetris3D

run:
	./FruitTetris3D

valgrind:
	valgrind --leak-check=full --track-origins=yes ./FruitTetris3D
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>
#include <stdio.h>
#include "GameInstance.h"
#include "Board.h"
#include "BoardMediator.h"
#include "Renderer.h"
#include "Grid.h"
#include "Shader.h"

GameInstance gameState;
Renderer renderer;
BoardMediator boardMediator;
Grid* grid;

bool rotateL = false;
bool rotateR = false;

// Initialize settings used for GLUT drawing
void initGlutSettings() {

    const float W_WIDTH  = 480.0;
    const float W_HEIGHT = 640.0;
    const float FOV_Y    = 60.0;
    const float Z_NEAR   = 0.1;
    const float Z_FAR    = 100.0;

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(W_WIDTH, W_HEIGHT);
    glutInitWindowPosition(50, 100);
    glutInitContextVersion( 2, 1 );
    glutInitContextProfile( GLUT_CORE_PROFILE );
    glutCreateWindow("Fruity Tetris Studio");

    glewInit();

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthFunc(GL_LESS);
    glMatrixMode(GL_PROJECTION);
    glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);

    glLoadIdentity();
    gluPerspective(FOV_Y, W_WIDTH / W_HEIGHT, Z_NEAR, Z_FAR);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(2.0f, 1.0f, 3.0f,
              0.0f, 0.0f, 0.0f,
              0.0f, 1.0f, 0.0f);
    gluLookAt(0, 23, 25, 0, 10, 0, 0, 1, 0);
}

// Ends the game in the case where the player has run out of room to place blocks
void endGame() {

    // Set the game mode to "game over"
    gameState.toggleGameOver();

    // Set each fruit available on the game board to be grey
    Board* gameBoard = gameState.getBoard();
    gameBoard->setCellsToGray();

    // Draw the newly greyed-out fruits on the game board
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    renderer.draw(gameBoard, grid);
    glutSwapBuffers();
}

// Start a new tetromino from the top after one has been dropped
void restartTetromino() {
    
    Board* gameBoard = gameState.getBoard();
    try {
        boardMediator.startTetrominoFromTop(gameBoard);
    }

    // If placing a new tetromino caused a collision, end the game
    catch (std::domain_error collision) {
        endGame();
    }
}

// Move the current tetromino down by one cell on the board
void moveTetrominoDown() {

    Board* gameBoard = gameState.getBoard();
    try {
        boardMediator.moveTetromino(Vector(270, 1), gameBoard->getTetromino(),gameBoard);
    }

    // If moving the tetromino caused a collision, end the game
    catch (std::domain_error e) {
        endGame();
    }
}

// Main game loop
void playGame(void) {

    // If the game isn't paused and hasn't ended, redraw the board
    if (!gameState.isPaused() && !gameState.gameIsOver()) {

        double currentTime     = gameState.getCurrentTime();
        double nextRefreshTime = gameState.getNextRefreshTime();

        // Limits the frame rate to limit the necessary operations
        if (currentTime >= nextRefreshTime) {

            double refreshRate = gameState.getRefreshRate();
            double currentTime = gameState.getCurrentTime();
            Board* gameBoard   = gameState.getBoard();

            // Lock player key input during critical tetromino calculations
            gameState.toggleInputLock();
            gameState.setNextRefreshTime(currentTime + refreshRate);

            if (!gameBoard->tetrominoIsOnBoard()) {
                restartTetromino();
            }

            // Drop the tetromino down one block if enough time has elapsed
            if (currentTime >= gameState.getNextTickTime()) {
                gameState.setNextTickTime(currentTime + gameState.getTickLength());
                moveTetrominoDown();
            }

            if (rotateL) {
                renderer.rotateWorldLeft(gameState.getBoard(), grid);
            }
            if (rotateR) {
                renderer.rotateWorldRight(gameState.getBoard(), grid);
            }

            // Allow for player key input
            gameState.toggleInputLock();
            
            // Draw the resulting game board
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            renderer.draw(gameBoard, grid);
            glutSwapBuffers();
        }
    }
}

// Drop tetromino on the board to the lowest position it can reach vertically
void dropCurrentTetromino() {
    Board* gameBoard = gameState.getBoard();
    if (!gameState.isPaused() && !gameState.gameIsOver()) {

        try {
            boardMediator.dropTetromino(gameBoard->getTetromino(), gameBoard);
        }

        // If dropping the tetromino caused a collision, end the game
        catch (std::domain_error e) {
            endGame();
        }
    }
}

// In the case of a down-arrow press, speed up the drop speed of the tetromino
void speedUpTetrominoDrop() {
    double newTickLength = 75.0;
    gameState.setTickLength(newTickLength);
    gameState.setNextTickTime(gameState.getCurrentTime());
}

// In case of a left-arrow press, move the tetromino left
void moveTetrominoLeft() {
    Board* gameBoard = gameState.getBoard();
    if (gameBoard->tetrominoIsOnBoard()) {
        boardMediator.moveTetromino(Vector(180, 1), gameBoard->getTetromino(), gameBoard);
    }
}

// In case of a right-arrow press, move the tetromino right
void moveTetrominoRight() {
    Board* gameBoard = gameState.getBoard();
    if (gameBoard->tetrominoIsOnBoard()) {
        boardMediator.moveTetromino(Vector(0, 1), gameBoard->getTetromino(), gameBoard);
    }
}

// In case of an up-arrow press, rotate the tetromino counterclockwise
void rotateTetrominoCCW() {
    Board* gameBoard = gameState.getBoard();
    boardMediator.rotateTetromino(gameBoard->getTetromino(), gameBoard);
}

// In case the down-arrow has been released, slow tetromino drop speed back down
void slowDownTetrominoDrop() {
    double newTickLength = 1000.0;
    gameState.setTickLength(newTickLength);
    double currentTime = gameState.getCurrentTime();
    gameState.setNextTickTime(currentTime + newTickLength);
}

// Provides the callback for any regular ASCII key press
void regularKeyCallback(unsigned char keyPressed, int xMouse, int yMouse) {

    // Only allow for key presses if the game's input is not locked
    if (!gameState.inputIsLocked()) {

        // Lock player key input to prevent multiple key presses
        gameState.toggleInputLock();
        switch (keyPressed) {

            case ' ':                   // Spacebar => Drop tetromino
                dropCurrentTetromino();
                break;
            case 'q':
            case 'Q':
                exit(0);                // Q => Quit the game
                break;
            case 'p':
            case 'P':
                gameState.flipPause();  // P => Pause the game
                break;
            case 'r':
            case 'R':
                gameState.restart();    // R => Restart the game
        }

        // Unlock player key input
        gameState.toggleInputLock();
    }
}

// Provides the callback for the release of the arrow keys
void specialKeyCallbackUp(int keyPressed, int xMouse, int yMouse) {

    // Allow for key releases if the game isn't paused, locked, or hasn't ended
    if (!gameState.isPaused() && !gameState.inputIsLocked() && !gameState.gameIsOver()) {

        // Lock player key input to prevent multiple key presses
        gameState.toggleInputLock();
        switch (keyPressed) {

            case GLUT_KEY_DOWN:         // Down arrow released => slow tetromino speed
                slowDownTetrominoDrop();
                break;
            case GLUT_KEY_LEFT:
                rotateL = false;
                break;
            case GLUT_KEY_RIGHT:
                rotateR = false;
                break;
        }

        // Unlock player key input
        gameState.toggleInputLock();
    }
}

// Provides the callback for any arrow key being pressed
void specialKeyCallbackDown(int keyPressed, int xMouse, int yMouse) {

    // Allow for arrow key presses if the game isn't paused, locked, or hasn't ended
    if (!gameState.isPaused() && !gameState.inputIsLocked() && !gameState.gameIsOver()) {

        // Lock player key input to prevent multiple key presses
        gameState.toggleInputLock();
        switch (keyPressed) {

            case GLUT_KEY_DOWN:         // Down arrow press => fast tetromino speed
                speedUpTetrominoDrop();
                break;
            case GLUT_KEY_LEFT:         // Left arrow press => move tetromino left
                if (glutGetModifiers() == GLUT_ACTIVE_CTRL) {
                    rotateL = true;
                }
                else {
                    moveTetrominoLeft();
                }
                break;
            case GLUT_KEY_RIGHT:        // Right arrow press => move tetromino right
                if (glutGetModifiers() == GLUT_ACTIVE_CTRL) {
                    rotateR = true;
                }
                else {
                    moveTetrominoRight();
                }
                break;
            case GLUT_KEY_UP:           // Up arrow press => rotate tetromino CCW
                rotateTetrominoCCW();
                break;
        }

        // Unlock player key input
        gameState.toggleInputLock();
    }
}

int main(int argc, char** argv) {

    Coordinate origin;
    origin.x = 0;
    origin.y = 0;

    Dimension gridSize;
    gridSize.width  = 10;
    gridSize.height = 20;
    gridSize.depth  = 1;

    GLfloat vertices[] = {
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f,  0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f
    };

    GLuint indices[] = {
        0, 1, 3,
        0, 2, 3,
        0, 1, 5,
        0, 4, 5,
        0, 2, 6,
        0, 4, 6,
        7, 3, 2,
        7, 6, 2,
        7, 3, 1,
        7, 5, 1,
        7, 5, 4,
        7, 6, 4
    };

    // Seed the random number generator with system clock
    srand(time(NULL));

    // Initialize GLUT and init locally-defined GLUT settings
    glutInit(&argc, argv);
    initGlutSettings();

    // Provide the callback for window redrawing
    glutDisplayFunc(playGame);
    glutIdleFunc(playGame);

    // Provide the callback for ASCII key presses
    glutKeyboardFunc(regularKeyCallback);

    // Provide callbacks for key up/down arrow presses
    glutSpecialFunc(specialKeyCallbackDown);
    glutSpecialUpFunc(specialKeyCallbackUp);

    // Compatibility thing for GLEW v2.0
    glewExperimental = GL_TRUE;


	if (glewInit() == GLEW_OK) {
        printf("Using GLEW version %s.\n", glewGetString(GLEW_VERSION));
    }
    else {
        printf("glew not ok\n");
        exit(1);
    }

    // todo: add error handling stuff for glew shader calls

    // const GLchar* abc = "test.vert";
    // const GLchar* def = "test.frag";

    Shader shader("./src/test.vert", "./src/test.frag");

    // // init shader programs
    // GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // glShaderSource(vertexShader, 1, &abc, NULL);
    // glCompileShader(vertexShader);
    // int success;
    // char infoLog[512];
    // glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    // if (!success) {
    //     glGetProgramInfoLog(vertexShader, 512, NULL, infoLog);
    //     std::cout << "err\n" << infoLog << std::endl;
    // }

    // GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    // glShaderSource(fragmentShader, 1, &def, NULL);
    // glCompileShader(fragmentShader);
    // glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    // if (!success) {
    //     glGetProgramInfoLog(fragmentShader, 512, NULL, infoLog);
    //     std::cout << "err\n" << infoLog << std::endl;
    // }

    // GLuint shaderProgram = glCreateProgram();
    // glAttachShader(shaderProgram, vertexShader);
    // glAttachShader(shaderProgram, fragmentShader);
    // glLinkProgram(shaderProgram);
    // glUseProgram(shaderProgram);
    // glDeleteShader(vertexShader);
    // glDeleteShader(fragmentShader);
    
    // Initialize grid object shaders
    grid = new Grid(20, 10, gridSize, origin);

    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    grid->setVAO(VAO);
    grid->setShader(shader.getID());


    // Call the main loop for GLUT
    glutMainLoop();
    
    return 0;
}
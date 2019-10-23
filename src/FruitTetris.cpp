#include <GL/glut.h>
#include <stdio.h>
#include "GameInstance.h"
#include "Board.h"
#include "BoardMediator.h"
#include "Renderer.h"

GameInstance gameState;
Renderer renderer;
BoardMediator boardMediator;

// Initialize settings used for GLUT drawing
void initGlutSettings() {

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowPosition(50, 100);
    glutInitWindowSize(400, 800);
    glutCreateWindow("Fruity Tetris Studio");
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, 200.0, 0.0, 400.0);
    glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
}

// Ends the game in the case where the player has run out of room to place blocks
void endGame() {

    // Set the game mode to "game over"
    gameState.toggleGameOver();

    // Set each fruit available on the game board to be grey
    Board* gameBoard = gameState.getBoard();
    gameBoard->setCellsToGray();

    // Draw the newly greyed-out fruits on the game board
    glClear(GL_COLOR_BUFFER_BIT);
    renderer.draw(gameBoard);
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

            // Allow for player key input
            gameState.toggleInputLock();
            
            // Draw the resulting game board
            glClear(GL_COLOR_BUFFER_BIT);
            renderer.draw(gameBoard);
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
                moveTetrominoLeft();
                break;
            case GLUT_KEY_RIGHT:        // Right arrow press => move tetromino right
                moveTetrominoRight();
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

    // Call the main loop for GLUT
    glutMainLoop();
    
    return 0;
}
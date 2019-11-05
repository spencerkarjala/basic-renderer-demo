#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>
#include <../lib/glm/gtc/type_ptr.hpp>
#include <stdio.h>
#include "GameInstance.h"
#include "Board.h"
#include "BoardMediator.h"
#include "Renderer.h"
#include "Grid.h"
#include "Shader.h"
#include "string.h"
#include "Camera.h"
#include "ObjReader.h"
#include "RobotArm.h"
#include "Cube.h"
#include "Number.h"
#include "Scene.h"

#define BASE_HEIGHT 5
#define BASE_WIDTH  10

#define WINDOW_WIDTH   480
#define WINDOW_HEIGHT  640
#define VIEW_FOV       60.0f
#define VIEW_NEAR_CLIP 0.1f
#define VIEW_FAR_CLIP  100.0f

Grid* grid;
RobotArm* arm;
Cube* cube;
Camera* camera;
ObjReader reader;
GameInstance gameState;
Renderer* renderer;
BoardMediator boardMediator;
Number* num;
Scene scene;

int  rotateCamera    = 0;
int  rotateTopArm    = 0;
int  rotateBottomArm = 0;

bool timeExpired = false;

glm::mat4 mProjection;
glm::mat4 mModelView;

// Closes the game, freeing any necessary data
void closeGame() {
    delete grid;
    delete arm;
    delete cube;
    delete num;
    delete camera;
    exit(0);
}

// Ends the game in the case where the player has run out of room to place blocks
void endGame() {

    gameState.toggleGameOver();

    // Set each fruit available on the game board to be grey
    Board* gameBoard = gameState.getBoard();
    gameBoard->setCellsToGray();
}

// Drop tetromino on the board to the lowest position it can reach vertically
void dropCurrentTetromino() {
    
    if (arm->isHolding()) {

        // Safely retrieve the current game board
        Board*      board      = gameState.getBoard();
        if (board == nullptr) {
            return;
        }

        // Safely retrieve the tetromino currently placed on the board
        Tetromino*  tetromino  = board->getTetromino();
        if (tetromino == nullptr) {
            return;
        }

        // Safely retrieve the fruit belonging to the tetromino
        Fruit**     tetFruits  = tetromino->getFruits();
        if (tetFruits == nullptr) {
            return;
        }

        // Run through and check collision on each fruit; if collision, end game
        for (int index = 0; index < tetromino->NUM_FRUIT_PER_TETROMINO; index++) {
            Coordinate fruitPos = tetFruits[index]->getPosition();
            if (board->isOutOfBoundsAt(fruitPos) || board->isCollisionAt(fruitPos)) {
                if (gameState.timerHasExpired()) {
                    endGame();
                }
                return;
            }
        }

        // Drop the fruit, swap the timer model, and reset the timer
        arm->toggleHold();
        num->swapModel("./src/3dno5.obj");
        gameState.resetTimer();
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
    Board*      gameBoard  = gameState.getBoard();
    Tetromino*  tetromino  = gameBoard->getTetromino();

    if (tetromino != nullptr && arm->isHolding() 
        && !gameState.gameIsOver() && !gameState.isPaused()) {
        tetromino->rotateCCW();
    }
}

// In case the down-arrow has been released, slow tetromino drop speed back down
void slowDownTetrominoDrop() {
    double newTickLength = 1000.0;
    gameState.setTickLength(newTickLength);
    double currentTime = gameState.getCurrentTime();
    gameState.setNextTickTime(currentTime + newTickLength);
}

// Move the current tetromino down by  cell on the board
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

void updateTimer() {

    double currentTime = gameState.getCurrentTime();

    // Set the next timer tick and decrement the timer
    gameState.setNextHoldTime(currentTime + gameState.getTickLength());
    gameState.decrementTimer();

    // Update the timer's current model
    int curCount = gameState.getTimer();
    char* path = new char;
    sprintf(path, "./src/3dno%d.obj", curCount);
    num->swapModel(path);
    delete path;

    // If the timer's expired, drop the tetromino and mark it expired
    if (gameState.timerHasExpired()) {
        dropCurrentTetromino();
        gameState.toggleTimerExpired();
    }
}

void updateGame() {

    if (!gameState.isPaused() && !gameState.gameIsOver()) {

        double currentTime     = gameState.getCurrentTime();
        double nextRefreshTime = gameState.getNextRefreshTime();

        // Limits the frame rate to limit the necessary operations
        if (currentTime >= nextRefreshTime) {

            double refreshRate = gameState.getRefreshRate();
            double currentTime = gameState.getCurrentTime();

            // Lock player key input during critical tetromino calculations
            gameState.setNextRefreshTime(currentTime + refreshRate);

            // Drop the tetromino down one block if enough time has elapsed
            if (currentTime >= gameState.getNextTickTime() && !arm->isHolding()) {
                gameState.setNextTickTime(currentTime + gameState.getTickLength());
                moveTetrominoDown();
            }

            // If it's time for the timer to tick and the arm is holding a shape
            if (currentTime >= gameState.getNextHoldTime() && arm->isHolding()) {
                updateTimer();
            }

            glm::vec3 tetPosition = arm->getTipLocation(mModelView);

            if (!gameState.getBoard()->tetrominoIsOnBoard()) {
                arm->toggleHold();
            }
            if (arm->isHolding()) {
                boardMediator.updateTetrominoPosition(gameState.getBoard(), tetPosition);
            }
        }
    }
}

std::vector<float> generateCubeColors(Color color) {

    int NUM_CUBE_VERTICES = 8;
    std::vector<float> colors;
    for (int index = 0; index < NUM_CUBE_VERTICES; index++) {
        colors.push_back(color.red);  colors.push_back(color.green);
        colors.push_back(color.blue); colors.push_back(color.alpha);
    }
    return colors;
}

// Draws a given fruit on the game board
void drawFruit(Fruit* fruit) {
    
    Color       color     = fruit->getColor();
    Coordinate  position  = fruit->getPosition();
    Board*      board     = gameState.getBoard();
    Tetromino*  tetromino = board->getTetromino();

    unsigned int ModelView  = scene.getSceneData().modelView;

    if (tetromino != NULL && tetromino->fruitBelongsToTetromino(fruit)) { 
        if (board->isOutOfBoundsAt(position) || board->isCollisionAt(position)) {
            color.red  = 0.3f; color.green = 0.3f;
            color.blue = 0.3f; color.alpha = 1.0f;
        }
    }

    std::vector<float> cubeColors = generateCubeColors(color);

    if (fruit != nullptr) {

        cube->swapColor(cubeColors);
        cube->translate(glm::vec3(position.x - 4.5f, position.y + 0.5f, 0.f));

        if (!tetromino->fruitBelongsToTetromino(fruit)) {
            cube->scale(glm::vec3(0.99f, 0.99f, 0.99f));
        }

        glm::mat4 fruitModel = mModelView * cube->getModel();
        glUniformMatrix4fv(ModelView, 1, false, glm::value_ptr(fruitModel));
        cube->draw(mModelView);
        if (!tetromino->fruitBelongsToTetromino(fruit)) {
            cube->scale(glm::vec3(1.f / 0.99f, 1.f / 0.99f, 1.f / 0.99f));
        }
        cube->translate(glm::vec3(4.5f - position.x, -position.y - 0.5f, 0.f));
    }
}

// Draws each fruit on the game board that isn't NULL
void drawBoard() {

    // Get fruit matrix and dimensions for traversal
    Board*    gameBoard  = gameState.getBoard();
    Fruit***  fruits     = gameBoard->getFruits();
    int       numRows    = gameBoard->getNumRows();
    int       numCols    = gameBoard->getNumCols();

    // For each cell in each row, if a fruit exists there, draw it
    for (int row = 0; row < numRows; row++) {
        for (int col = 0; col < numCols; col++) {
            Fruit* currentFruit = fruits[row][col];
            if (currentFruit != NULL) {
                drawFruit(currentFruit);
            }
        }
    }
}

void drawTetromino() {

    Board*      gameBoard  = gameState.getBoard();
    Tetromino*  tetromino  = gameBoard->getTetromino();

    if (tetromino == NULL) {
        return;
    }

    Fruit** tetFruits = tetromino->getFruits();

    for (int index = 0; index < tetromino->NUM_FRUIT_PER_TETROMINO; index++) {
        Fruit* curr = tetFruits[index];
        if (curr != NULL) {
            drawFruit(curr);
        }
    }
}

void updateBackground() {
    if (!gameState.gameIsOver()) {
        glClearColor(0.7355f, 0.8391f, 0.9482f, 1.0f);
    }
    else {
        glClearColor(0.9804f, 0.5020f, 0.4471f, 1.0f);
    }
}

// Used to redraw the scene on each screen refresh
void refresh(void) {

    updateGame();

    updateBackground();

    if (rotateCamera != 0) {
        scene.rotateCamera(0.04f * rotateCamera);
        camera->rotate(0.04f * rotateCamera, glm::vec3(0.f, 1.f, 0.f));
    }
    if (rotateBottomArm != 0) {
        arm->rotateBottom(0.04f * rotateBottomArm);
    }
    if (rotateTopArm != 0) {
        arm->rotateTop(0.04f * rotateTopArm);
    }
    
    scene.refresh();

    // Calculate the model-view matrix
    mModelView = camera->getView();

    unsigned int ModelView  = scene.getSceneData().modelView;

    arm->draw(ModelView, mModelView);

    drawBoard();

    drawTetromino();

    glBindVertexArray(0);
 
    glutSwapBuffers();
}

void reshape(int width, int height) {

    scene.reshape(width, height);
}

void restartGame() {
    gameState.restart(); // R <=> restart game
    num->swapModel("./src/3dno5.obj");
}

void pauseGame() {
    gameState.flipPause();
    rotateTopArm = 0;
    rotateBottomArm = 0;
}

// Listens for ASCII keyboard input
void normalKeyDownListener(unsigned char key, int x, int y) {
    
    switch (key) {

        case 'q':
        case 'Q':
            closeGame();                   // Q <=> quit game
            break; 
        case 's':
        case 'S':
            if (!gameState.isPaused()) {
                rotateTopArm = 1;          // S <=> rotate arm top
            }
            break;
        case 'w':
        case 'W':
            if (!gameState.isPaused()) {
                rotateTopArm = -1;         // W <=> rotate arm top
            }
            break;
        case 'a':
        case 'A':
            if (!gameState.isPaused()) {
                rotateBottomArm = 1;       // A <=> rotate arm bottom
            }
            break;
        case 'd':
        case 'D':
            if (!gameState.isPaused()) {
                rotateBottomArm = -1;      // D <=> rotate arm bottom
            }
            break;
        case 'p':
        case 'P':
            pauseGame();         // P <=> pause game
            break;
        case 'r':
        case 'R':
            restartGame();                 // R <=> restart game
            break;
        case ' ':
            dropCurrentTetromino();        // Space <=> drop tetromino
            break;
    }
}

// Listens for ASCII keyboard key release
void normalKeyUpListener(unsigned char key, int x, int y) {

    switch(key) {

        case 'w':
        case 'W':
        case 's':
        case 'S':
            rotateTopArm = 0;       break;
        case 'a':
        case 'A':
        case 'd':
        case 'D':
            rotateBottomArm = 0;    break;
    }
}

// Provides the callback for any arrow key being pressed
void specialKeyDownListener(int key, int x, int y) {

    switch (key) {

        case GLUT_KEY_LEFT:         // CTRL-Left arrow press => spin camera left
            rotateCamera = glutGetModifiers() == GLUT_ACTIVE_CTRL;
            break;
        case GLUT_KEY_RIGHT:        // CTRL-Right arrow press => spin camera right
            rotateCamera = -(glutGetModifiers() == GLUT_ACTIVE_CTRL);
            break;
        case GLUT_KEY_UP:           // Up arrow press => rotate tetromino CCW
            rotateTetrominoCCW();
            break;
    }
}

// Provides the callback for the release of the arrow keys
void specialKeyUpListener(int key, int x, int y) {

    switch (key) {

        case GLUT_KEY_LEFT:
        case GLUT_KEY_RIGHT:
            rotateCamera = 0;
            break;
    }
}

void initGlutSettings() {

    // Set window settings like size, title, and context version
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitContextVersion(3, 2);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutCreateWindow("Fruity Tetris Studio");

    // Set up GLUT callback functions
    glutDisplayFunc(refresh);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(normalKeyDownListener);
    glutKeyboardUpFunc(normalKeyUpListener);
    glutSpecialFunc(specialKeyDownListener);
    glutSpecialUpFunc(specialKeyUpListener);
    glutIdleFunc(refresh);
}

void setVertexBuffers() {

    // Initalize the vertex array object
    scene.setupBuffers();
    unsigned int shader = scene.getSceneData().shader;

    // Set up buffers for game grid
    grid->setupBuffers(
        "./src/grid.obj",
        "vPosition",
        "vColor",
        shader,
        glm::vec4(0.f, 0.f, 0.f, 1.f));

    arm->init(shader);

    Color cubeColor;
    cubeColor.red  = 0.3f; cubeColor.green = 0.3f;
    cubeColor.blue = 0.3f; cubeColor.alpha = 1.0f;

    std::vector<float> cubeColors = generateCubeColors(cubeColor);

    cube->setupBuffers(
        "./src/cube.obj",
        "vPosition",
        "vColor",
        shader,
        glm::vec4(0.f, 0.f, 0.f, 1.f));

    num->setupBuffers(
        "./src/3dno2.obj",
        "vPosition",
        "vColor",
        shader,
        glm::vec4(1.f, 1.f, 1.f, 1.f));
    num->swapModel("./src/3dno5.obj");

    glBindVertexArray(0);
}

// Creates a camera with default parameters
void initScene() {

    // Retrieve window dimensions for aspect ratio
    float width  = glutGet(GLUT_WINDOW_WIDTH);
    float height = glutGet(GLUT_WINDOW_HEIGHT);

    float aspect = width / height;
    float vectors[9] = {
        20.f, 26.f, 20.f,
         0.f, 10.f,  0.f,
         0.f,  1.f,  0.f
    };

    scene.setupCamera(vectors, VIEW_FOV, aspect, VIEW_NEAR_CLIP, VIEW_FAR_CLIP);

    // Create a new camera
    camera = new Camera(
        glm::vec3(20.f, 26.f, 20.f),  // Position vector
        glm::vec3( 0.f, 10.f,  0.f),  // Target vector
        glm::vec3( 0.f,  1.f,  0.f),  // Up vector
        VIEW_FOV,                     // Field of view
        width / height,               // Aspect ratio
        VIEW_NEAR_CLIP,               // Near clip plane z
        VIEW_FAR_CLIP                 // Far clip plane z
    );
}

int main(int argc, char** argv) {

    // Initialize GLUT settings and GLEW
    glutInit(&argc, argv);
    initGlutSettings();
    glewInit();

    // Set basic 3D data for basic OpenGL
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.7355f, 0.8391f, 0.9482f, 1.0f);

    // Initialize a shader, given the desired paths
    Shader newShader("./src/test.vert", "./src/test.frag");

    initScene();
    scene.setShader(newShader.getID());

    // Create a new grid object
    Dimension gridSize;
    gridSize.width  = 10;
    gridSize.height = 20;
    gridSize.depth  = 1;
    grid = new Grid(20, 10, gridSize, SimpleObject::OBJ_VERTEX | SimpleObject::OBJ_LINE, GL_LINES);


    arm   = new RobotArm(SimpleObject::OBJ_VERTEX | SimpleObject::OBJ_LINE, GL_TRIANGLES);
    cube  = new Cube(SimpleObject::OBJ_VERTEX | SimpleObject::OBJ_FACE, GL_TRIANGLES);
    num   = new Number(SimpleObject::OBJ_VERTEX | SimpleObject::OBJ_FACE, GL_TRIANGLES);

    scene.insertObject("Grid", grid);
    scene.insertObject("Num", num);

    // Set up vertex buffers and vertex attribute arrays
    setVertexBuffers();

    // Initialize grid to correct location and dimensions
    grid->translate(glm::vec3(0.f, 10.f, 0.f));
    grid->scale(glm::vec3(10.f, 10.f, 10.f));

    num->translate(glm::vec3(0.f, 21.f, 0.f));
    num->rotate(M_PI / 2, glm::vec3(1.f, 0.f, 0.f));
    num->scale(glm::vec3(0.2f, 0.2f, 0.2f));

    // Go to GLUT main loop
    glutMainLoop();
    
    return 0;
}
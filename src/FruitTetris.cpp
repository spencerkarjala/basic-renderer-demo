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
#include "Axes.h"
#include "RobotArm.h"
#include "Cube.h"

#define BASE_HEIGHT 5
#define BASE_WIDTH  10

#define WINDOW_WIDTH   480
#define WINDOW_HEIGHT  640
#define VIEW_FOV       60.0f
#define VIEW_NEAR_CLIP 0.1f
#define VIEW_FAR_CLIP  100.0f

GLuint vPosition, vColor, Projection, ModelView;
Grid* grid;
Axes* axes;
RobotArm* arm;
Cube* cube;
Camera* camera;
ObjReader reader;
GameInstance gameState;
Renderer* renderer;
BoardMediator boardMediator;

int  rotateCamera    = 0;
int  rotateTopArm    = 0;
int  rotateBottomArm = 0;

glm::mat4 mProjection;
glm::mat4 mModelView;

GLuint shader, VAO;
 
// Vertex colors for the 3D axis
std::vector<float> axisColors = {   
    1.0f, 0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f
};

std::vector<float> gridColors = {
    1.0f, 1.0f, 1.0f, 1.0f
};

// Closes the game, freeing any necessary data`
void closeGame() {
    glDeleteVertexArrays(1, &VAO);
    delete grid;
    delete camera;
    delete axes;
    exit(0);
}

// Drop tetromino on the board to the lowest position it can reach vertically
void dropCurrentTetromino() {
    
    Board*      board      = gameState.getBoard();
    Tetromino*  tetromino  = board->getTetromino();
    Fruit**     tetFruits  = tetromino->getFruits();

    for (int index = 0; index < tetromino->NUM_FRUIT_PER_TETROMINO; index++) {
        Coordinate fruitPos = tetFruits[index]->getPosition();
        if (board->isOutOfBoundsAt(fruitPos) || board->isCollisionAt(fruitPos)) {
            return;
        }
    }

    arm->toggleHold();
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

    if (tetromino != nullptr) {
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

// Ends the game in the case where the player has run out of room to place blocks
void endGame() {

    // // Set the game mode to "game over"
    // gameState.toggleGameOver();

    // // Set each fruit available on the game board to be grey
    // Board* gameBoard = gameState.getBoard();
    // gameBoard->setCellsToGray();

    // // Draw the newly greyed-out fruits on the game board
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // renderer.draw(gameBoard, grid);
    // glutSwapBuffers();
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

void updateGame() {

    if (!gameState.isPaused() && !gameState.gameIsOver()) {

        double currentTime     = gameState.getCurrentTime();
        double nextRefreshTime = gameState.getNextRefreshTime();

        // Limits the frame rate to limit the necessary operations
        if (currentTime >= nextRefreshTime) {

            double refreshRate = gameState.getRefreshRate();
            double currentTime = gameState.getCurrentTime();

            // Lock player key input during critical tetromino calculations
            gameState.toggleInputLock();
            gameState.setNextRefreshTime(currentTime + refreshRate);

            // Drop the tetromino down one block if enough time has elapsed
            if (currentTime >= gameState.getNextTickTime() && !arm->isHolding()) {
                gameState.setNextTickTime(currentTime + gameState.getTickLength());
                moveTetrominoDown();
            }

            glm::vec3 tetPosition = arm->getTipLocation(mModelView);

            if (!gameState.getBoard()->tetrominoIsOnBoard()) {
                arm->toggleHold();
            }
            if (arm->isHolding()) {
                boardMediator.updateTetrominoPosition(gameState.getBoard(), tetPosition);
            }

            // Allow for player key input
            gameState.toggleInputLock();
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

    if (tetromino != NULL && tetromino->fruitBelongsToTetromino(fruit)) { 
        if (board->isOutOfBoundsAt(position) || board->isCollisionAt(position)) {
            color.red  = 0.3f; color.green = 0.3f;
            color.blue = 0.3f; color.alpha = 1.0f;
        }
    }

    std::vector<float> cubeColors = generateCubeColors(color);

    if (fruit != nullptr) {


        cube->changeColor(cubeColors);
        cube->translate(glm::vec3(position.x - 4.5f, position.y + 0.5f, 0.f));

        if (!tetromino->fruitBelongsToTetromino(fruit)) {
            cube->scale(glm::vec3(0.99f, 0.99f, 0.99f));
        }

        glm::mat4 fruitModel = mModelView * cube->getModel();
        glUniformMatrix4fv(ModelView, 1, false, glm::value_ptr(fruitModel));
        cube->draw(GL_TRIANGLES);
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

// Used to redraw the scene on each screen refresh
void refresh(void) {

    updateGame();

    if (rotateCamera != 0) {
        camera->rotate(0.02f * rotateCamera, glm::vec3(0.f, 1.f, 0.f));
    }
    if (rotateBottomArm != 0) {
        arm->rotateBottom(0.005f * rotateBottomArm);
    }
    if (rotateTopArm != 0) {
        arm->rotateTop(0.005f * rotateTopArm);
    }
    
    // Clear color and z-buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Calculate the model-view matrix
    mModelView = camera->getView();
    mModelView = mModelView * glm::mat4x4(1.f);

    // Set up the shader and the MVP matrices
    glUseProgram(shader);
    glUniformMatrix4fv(Projection, 1, false, glm::value_ptr(mProjection));
    glUniformMatrix4fv(ModelView,  1, false, glm::value_ptr(mModelView));
    glBindVertexArray(VAO);

    // arm->draw(ModelView, mModelView);

    // glm::mat4 axesModel = mModelView * axes->getModel();
    // glUniformMatrix4fv(ModelView, 1, false, glm::value_ptr(axesModel));
    // axes->draw(GL_LINES);

    // glm::mat4 gridModel = mModelView * grid->getModel();
    // glUniformMatrix4fv(ModelView, 1, false, glm::value_ptr(gridModel));
    // grid->draw(GL_LINES);

    // drawBoard();

    // drawTetromino();

    glutBitmapCharacter(GLUT_BITMAP_8_BY_13, 'O');

    glBindVertexArray(0);
 
    glutSwapBuffers();
}

void reshape(int width, int height) {
 
    // Set the viewport to be the entire window
    glViewport(0, 0, width, height);

    // Update the aspect ratio for the frustum
    camera->setFOV((float) width / (float) height);

    // Set up perspective matrix for the frustum
    mProjection = camera->getProjection();
    
    // Set model, view, and projection matrices
    glUniformMatrix4fv(Projection, 1, GL_TRUE, glm::value_ptr(mProjection));
    glUniformMatrix4fv(ModelView,  1, GL_TRUE, glm::value_ptr(mModelView));
}

// Listens for ASCII keyboard input
void normalKeyDownListener(unsigned char key, int x, int y) {
    
    switch (key) {

        case 'q':
        case 'Q':
            closeGame();            break; // Q <=> quit game
        case 's':
        case 'S':
            rotateTopArm = 1;       break; // S <=> rotate arm top
        case 'w':
        case 'W':
            rotateTopArm = -1;      break; // W <=> rotate arm top
        case 'a':
        case 'A':
            rotateBottomArm = 1;    break; // A <=> rotate arm bottom
        case 'd':
        case 'D':
            rotateBottomArm = -1;   break; // D <=> rotate arm bottom
        case 'p':
        case 'P':
            gameState.flipPause();  break; // P <=> pause game
        case 'r':
        case 'R':
            gameState.restart();    break; // R <=> restart game
        case ' ':                  
            dropCurrentTetromino(); break; // Space <=> drop tetromino
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
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Set up buffers for coordinate axes
    axes->setupBuffers("./src/axes.obj", "vPosition", "vColor", shader, axisColors);

    // Set up buffers for game grid
    grid->setupBuffers("./src/grid.obj", "vPosition","vColor", shader, grid->getColors());

    arm->init(shader);

    Color cubeColor;
    cubeColor.red  = 0.3f; cubeColor.green = 0.3f;
    cubeColor.blue = 0.3f; cubeColor.alpha = 1.0f;

    std::vector<float> cubeColors = generateCubeColors(cubeColor);

    cube->setupBuffers("./src/cube.obj", "vPosition", "vColor", shader, cubeColors);

    glBindVertexArray(0);
}

// Sets attribute variables for later manipulation
void setAttributes() {

    Projection = glGetUniformLocation(shader, "Projection");
    ModelView  = glGetUniformLocation(shader, "ModelView");
}

// Creates a camera with default parameters
void initCamera() {

    // Retrieve window dimensions for aspect ratio
    float width  = glutGet(GLUT_WINDOW_WIDTH);
    float height = glutGet(GLUT_WINDOW_HEIGHT);

    // Create a new camera
    camera = new Camera(
        glm::vec3(20.f, 25.f, 20.f),  // Position vector
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
    initCamera();
    Shader newShader("./src/test.vert", "./src/test.frag");
    shader = newShader.getID();

    // Create a new grid object
    Dimension gridSize;
    gridSize.width  = 10;
    gridSize.height = 20;
    gridSize.depth  = 1;
    grid = new Grid(20, 10, gridSize, WorldObject::OBJ_VERTEX | WorldObject::OBJ_LINE);

    axes   = new Axes(WorldObject::OBJ_VERTEX | WorldObject::OBJ_LINE);
    arm    = new RobotArm();
    cube   = new Cube(WorldObject::OBJ_VERTEX | WorldObject::OBJ_FACE);

    // Set up vertex buffers and vertex attribute arrays
    setAttributes();
    setVertexBuffers();

    // Initialize axes to the correct dimensions
    axes->scale(glm::vec3(100.f, 100.f, 100.f));
    
    // Initialize grid to correct location and dimensions
    grid->translate(glm::vec3(0.f, 10.f, 0.f));
    grid->scale(glm::vec3(10.f, 10.f, 10.f));

    // Go to GLUT main loop
    glutMainLoop();
    
    return 0;
}
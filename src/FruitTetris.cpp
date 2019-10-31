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

#define BASE_HEIGHT 5
#define BASE_WIDTH  10

#define WINDOW_WIDTH   480
#define WINDOW_HEIGHT  640
#define VIEW_FOV       60.0f
#define VIEW_NEAR_CLIP 0.1f
#define VIEW_FAR_CLIP  100.0f

GLuint vPosition, vColor, Projection, ModelView;
Grid* grid;
Camera* camera;

glm::mat4 mProjection;
glm::mat4 mModelView;

GLuint program;

// Vertices for the 3D axis
float vertices[] = {
    -1.0f,  0.0f,  0.0f,  1.0f,
     99.0f, 0.0f,  0.0f,  1.0f,
     0.0f, -1.0f, 0.0f,   1.0f,
     0.0f,  99.0f, 0.0f,  1.0f,
     0.0f,  0.0f, -1.0f,  1.0f,
     0.0f,  0.0f,  99.0f, 1.0f
};
 
// Vertex colors for the 3D axis
float colors[] = {   
    1.0f, 0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f
};

// GameInstance gameState;
// Renderer renderer;
// BoardMediator boardMediator;
// Grid* grid;

// GLuint ModelView;
// GLuint Projection;

// mat4 model_view;

// bool rotateL = false;
// bool rotateR = false;

// // Initialize settings used for GLUT drawing
// void initGlutSettings() {

//     const float W_WIDTH  = 480.0;
//     const float W_HEIGHT = 640.0;
//     const float FOV_Y    = 60.0;
//     const float Z_NEAR   = 0.1;
//     const float Z_FAR    = 100.0;

//     glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
//     glutInitWindowSize(W_WIDTH, W_HEIGHT);
//     glutInitWindowPosition(50, 100);
//     glutInitContextVersion( 3, 2 );
//     glutInitContextProfile( GLUT_CORE_PROFILE );
//     glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
//     glutCreateWindow("Fruity Tetris Studio");
// }

// // Ends the game in the case where the player has run out of room to place blocks
// void endGame() {

//     // Set the game mode to "game over"
//     gameState.toggleGameOver();

//     // Set each fruit available on the game board to be grey
//     Board* gameBoard = gameState.getBoard();
//     gameBoard->setCellsToGray();

//     // Draw the newly greyed-out fruits on the game board
//     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//     renderer.draw(gameBoard, grid);
//     glutSwapBuffers();
// }

// // Start a new tetromino from the top after one has been dropped
// void restartTetromino() {
    
//     Board* gameBoard = gameState.getBoard();
//     try {
//         boardMediator.startTetrominoFromTop(gameBoard);
//     }

//     // If placing a new tetromino caused a collision, end the game
//     catch (std::domain_error collision) {
//         endGame();
//     }
// }

// // Move the current tetromino down by one cell on the board
// void moveTetrominoDown() {

//     Board* gameBoard = gameState.getBoard();
//     try {
//         boardMediator.moveTetromino(Vector(270, 1), gameBoard->getTetromino(),gameBoard);
//     }

//     // If moving the tetromino caused a collision, end the game
//     catch (std::domain_error e) {
//         endGame();
//     }
// }

// float index = 0;

// // Main game loop
// void playGame(void) {

//     // glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
//     // glClear(GL_COLOR_BUFFER_BIT);
//     // glUseProgram(grid->getShader());
//     // glBindVertexArray(grid->getVAO());
//     // glDrawArrays(GL_TRIANGLES, 0, 3);
//     // glutSwapBuffers();
//     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//     mat4 instance = ( RotateZ(1.0 * index) * Translate( 0.0, 0.5 * BASE_HEIGHT, 0.0 ) *
// 		 Scale( BASE_WIDTH,
// 			BASE_HEIGHT,
// 			BASE_WIDTH ) );

//             index++;

//     for (int i = 0; i < 4; i++) {
//         printf("%f\t%f\t%f\t%f\n", instance._m[i].x, instance._m[i].y, instance._m[i].z, instance._m[i].w);
//     }
//     printf("\n");

//     glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view * instance);
//     glDrawArrays(GL_TRIANGLES, 0, 3);
//     glutSwapBuffers();

//     // // If the game isn't paused and hasn't ended, redraw the board
//     // if (!gameState.isPaused() && !gameState.gameIsOver()) {

//     //     double currentTime     = gameState.getCurrentTime();
//     //     double nextRefreshTime = gameState.getNextRefreshTime();

//     //     // Limits the frame rate to limit the necessary operations
//     //     if (currentTime >= nextRefreshTime) {

//     //         double refreshRate = gameState.getRefreshRate();
//     //         double currentTime = gameState.getCurrentTime();
//     //         Board* gameBoard   = gameState.getBoard();

//     //         // Lock player key input during critical tetromino calculations
//     //         gameState.toggleInputLock();
//     //         gameState.setNextRefreshTime(currentTime + refreshRate);

//     //         if (!gameBoard->tetrominoIsOnBoard()) {
//     //             restartTetromino();
//     //         }

//     //         // Drop the tetromino down one block if enough time has elapsed
//     //         if (currentTime >= gameState.getNextTickTime()) {
//     //             gameState.setNextTickTime(currentTime + gameState.getTickLength());
//     //             moveTetrominoDown();
//     //         }

//     //         if (rotateL) {
//     //             renderer.rotateWorldLeft(gameState.getBoard(), grid);
//     //         }
//     //         if (rotateR) {
//     //             renderer.rotateWorldRight(gameState.getBoard(), grid);
//     //         }

//     //         // Allow for player key input
//     //         gameState.toggleInputLock();
            
//     //         // Draw the resulting game board
//     //         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//     //         renderer.draw(gameBoard, grid);
//     //         glutSwapBuffers();
//     //     }
//     // }
// }

// // Drop tetromino on the board to the lowest position it can reach vertically
// void dropCurrentTetromino() {
//     Board* gameBoard = gameState.getBoard();
//     if (!gameState.isPaused() && !gameState.gameIsOver()) {

//         try {
//             boardMediator.dropTetromino(gameBoard->getTetromino(), gameBoard);
//         }

//         // If dropping the tetromino caused a collision, end the game
//         catch (std::domain_error e) {
//             endGame();
//         }
//     }
// }

// // In the case of a down-arrow press, speed up the drop speed of the tetromino
// void speedUpTetrominoDrop() {
//     double newTickLength = 75.0;
//     gameState.setTickLength(newTickLength);
//     gameState.setNextTickTime(gameState.getCurrentTime());
// }

// // In case of a left-arrow press, move the tetromino left
// void moveTetrominoLeft() {
//     Board* gameBoard = gameState.getBoard();
//     if (gameBoard->tetrominoIsOnBoard()) {
//         boardMediator.moveTetromino(Vector(180, 1), gameBoard->getTetromino(), gameBoard);
//     }
// }

// // In case of a right-arrow press, move the tetromino right
// void moveTetrominoRight() {
//     Board* gameBoard = gameState.getBoard();
//     if (gameBoard->tetrominoIsOnBoard()) {
//         boardMediator.moveTetromino(Vector(0, 1), gameBoard->getTetromino(), gameBoard);
//     }
// }

// // In case of an up-arrow press, rotate the tetromino counterclockwise
// void rotateTetrominoCCW() {
//     Board* gameBoard = gameState.getBoard();
//     boardMediator.rotateTetromino(gameBoard->getTetromino(), gameBoard);
// }

// // In case the down-arrow has been released, slow tetromino drop speed back down
// void slowDownTetrominoDrop() {
//     double newTickLength = 1000.0;
//     gameState.setTickLength(newTickLength);
//     double currentTime = gameState.getCurrentTime();
//     gameState.setNextTickTime(currentTime + newTickLength);
// }

// // Provides the callback for any regular ASCII key press
// void regularKeyCallback(unsigned char keyPressed, int xMouse, int yMouse) {

//     // Only allow for key presses if the game's input is not locked
//     if (!gameState.inputIsLocked()) {

//         // Lock player key input to prevent multiple key presses
//         gameState.toggleInputLock();
//         switch (keyPressed) {

//             case ' ':                   // Spacebar => Drop tetromino
//                 dropCurrentTetromino();
//                 break;
//             case 'q':
//             case 'Q':
//                 exit(0);                // Q => Quit the game
//                 break;
//             case 'p':
//             case 'P':
//                 gameState.flipPause();  // P => Pause the game
//                 break;
//             case 'r':
//             case 'R':
//                 gameState.restart();    // R => Restart the game
//         }

//         // Unlock player key input
//         gameState.toggleInputLock();
//     }
// }

// // Provides the callback for the release of the arrow keys
// void specialKeyCallbackUp(int keyPressed, int xMouse, int yMouse) {

//     // Allow for key releases if the game isn't paused, locked, or hasn't ended
//     if (!gameState.isPaused() && !gameState.inputIsLocked() && !gameState.gameIsOver()) {

//         // Lock player key input to prevent multiple key presses
//         gameState.toggleInputLock();
//         switch (keyPressed) {

//             case GLUT_KEY_DOWN:         // Down arrow released => slow tetromino speed
//                 slowDownTetrominoDrop();
//                 break;
//             case GLUT_KEY_LEFT:
//                 rotateL = false;
//                 break;
//             case GLUT_KEY_RIGHT:
//                 rotateR = false;
//                 break;
//         }

//         // Unlock player key input
//         gameState.toggleInputLock();
//     }
// }

// // Provides the callback for any arrow key being pressed
// void specialKeyCallbackDown(int keyPressed, int xMouse, int yMouse) {

//     // Allow for arrow key presses if the game isn't paused, locked, or hasn't ended
//     if (!gameState.isPaused() && !gameState.inputIsLocked() && !gameState.gameIsOver()) {

//         // Lock player key input to prevent multiple key presses
//         gameState.toggleInputLock();
//         switch (keyPressed) {

//             case GLUT_KEY_DOWN:         // Down arrow press => fast tetromino speed
//                 speedUpTetrominoDrop();
//                 break;
//             case GLUT_KEY_LEFT:         // Left arrow press => move tetromino left
//                 if (glutGetModifiers() == GLUT_ACTIVE_CTRL) {
//                     rotateL = true;
//                 }
//                 else {
//                     moveTetrominoLeft();
//                 }
//                 break;
//             case GLUT_KEY_RIGHT:        // Right arrow press => move tetromino right
//                 if (glutGetModifiers() == GLUT_ACTIVE_CTRL) {
//                     rotateR = true;
//                 }
//                 else {
//                     moveTetrominoRight();
//                 }
//                 break;
//             case GLUT_KEY_UP:           // Up arrow press => rotate tetromino CCW
//                 rotateTetrominoCCW();
//                 break;
//         }

//         // Unlock player key input
//         gameState.toggleInputLock();
//     }
// }

// Used to redraw the scene on each screen refresh
void refresh(void) {
    
    // Clear color and z-buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Calculate the model-view matrix
    mModelView = camera->getView();
    mModelView = mModelView * glm::mat4x4(1.0f);

    // Set up the shader and the MVP matrices
    glUseProgram(grid->getShader());
    glUniformMatrix4fv(Projection, 1, false, glm::value_ptr(mProjection));
    glUniformMatrix4fv(ModelView,  1, false, glm::value_ptr(mModelView));
 
    glBindVertexArray(grid->getVAO());
    glDrawArrays(GL_LINES, 0, 6);
 
    glutSwapBuffers();
}

void reshape(int width, int height) {
 
    // Set the viewport to be the entire window
    glViewport(0, 0, width, height);

    // Update the aspect ratio for the frustum
    camera->setFOV((float) width / (float) height);

    // Set up perspective matrix for the frustum
    mProjection = camera->getProjection();
}

// Closes the game, freeing any necessary data`
void closeGame() {

    const GLuint vaoToDelete = grid->getVAO();

    glDeleteVertexArrays(1, &vaoToDelete);
    delete grid;
    delete camera;
    exit(0);
}

// Listens for ASCII keyboard input
void normalKeyListener(unsigned char key, int x, int y) {
    
    switch (key) {

        // If 'q' pressed, quit the game
        case 'q':
        case 'Q':
            closeGame();
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
    glutKeyboardFunc(normalKeyListener);
    glutIdleFunc(refresh);
}

void setVertexBuffers() {

    // Initalize the vertex array object
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Initialize the vertex buffer object for vertices and colors
    GLuint vertexVBO, colorVBO;
    glGenBuffers(1, &vertexVBO);
    glGenBuffers(1, &colorVBO);

    // Bind buffers for both vertices and colors
    glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
    glBindBuffer(GL_ARRAY_BUFFER, colorVBO);

    // Bind the buffer used for vertex data
    glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Reference the vertex buffer using a saved global attribute
    vPosition = glGetAttribLocation( program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, 0, 0, 0);
 
    // Bind the buffer used for vertex color data
    glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

    // Reference the vertex color buffer using a saved global attribute
    vColor     = glGetAttribLocation( program, "vColor"); 
    glEnableVertexAttribArray(vColor);
    glVertexAttribPointer(vColor, 4, GL_FLOAT, 0, 0, 0);

    // Save the VAO for later manipulation
    grid->setVAO(VAO);
}

// Sets attribute variables for later manipulation
void setAttributes() {
 
    Projection = glGetUniformLocation(program, "Projection");
    ModelView  = glGetUniformLocation(program, "ModelView");
}

// Creates a camera with default parameters
void initCamera() {

    // Retrieve window dimensions for aspect ratio
    float width  = glutGet(GLUT_WINDOW_WIDTH);
    float height = glutGet(GLUT_WINDOW_HEIGHT);

    // Create a new camera
    camera = new Camera(
        glm::vec3(2.0f, 1.0f, 3.0f),    // Position vector
        glm::vec3(0.0f, 0.0f, 0.0f),    // Target vector
        glm::vec3(0.0f, 1.0f, 0.0f),    // Up vector
        VIEW_FOV,                       // Field of view
        width / height,                 // Aspect ratio
        VIEW_NEAR_CLIP,                 // Near clip plane z
        VIEW_FAR_CLIP                   // Far clip plane z
        );
}

int main(int argc, char** argv) {

    // Initialize GLUT settings and GLEW
    glutInit(&argc, argv);
    initGlutSettings();
    glewInit();

    // Set basic 3D data for basic OpenGL
    glEnable(GL_DEPTH_TEST);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    // Initialize a shader, given the desired paths
    initCamera();
    Shader shader("./src/test.vert", "./src/test.frag");
    program = shader.getID();

    // Create a new grid object
    Coordinate origin; 
    origin.y = 0; 
    origin.x = 0;
    Dimension gridSize;
    gridSize.width  = 10;
    gridSize.height = 20;
    gridSize.depth  = 1;
    grid = new Grid(20, 10, gridSize, origin);
    grid->setShader(shader.getID());

    // Set up vertex buffers and vertex attribute arrays
    setAttributes();
    setVertexBuffers();

    ObjReader reader;

    std::vector<glm::vec4> vert = reader.readVertices("./src/grid.obj");

    // Go to GLUT main loop
    glutMainLoop();
    
    return 0;
}
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>
#include <stdio.h>
#include "GameInstance.h"
#include "Board.h"
#include "BoardMediator.h"
#include "Renderer.h"
#include "Grid.h"
#include "Shader.h"
#include "mat.h"

#define BASE_HEIGHT 5
#define BASE_WIDTH  10

GameInstance gameState;
Renderer renderer;
BoardMediator boardMediator;
Grid* grid;

GLuint ModelView;
GLuint Projection;

mat4 model_view;

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
    glutInitContextVersion( 3, 2 );
    glutInitContextProfile( GLUT_CORE_PROFILE );
    glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
    glutCreateWindow("Fruity Tetris Studio");

    // glDepthFunc(GL_LESS);
    // glMatrixMode(GL_PROJECTION);

    // glLoadIdentity();
    // gluPerspective(FOV_Y, W_WIDTH / W_HEIGHT, Z_NEAR, Z_FAR);
    
    // glMatrixMode(GL_MODELVIEW);
    // glLoadIdentity();
    // gluLookAt(0.0f, 0.0f, -3.0f,
    //           0.0f, 0.0f, 0.0f,
    //           0.0f, 1.0f, 0.0f);
    // gluLookAt(0, 23, 25, 0, 10, 0, 0, 1, 0);
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

float index = 0;

// Main game loop
void playGame(void) {

    // glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    // glClear(GL_COLOR_BUFFER_BIT);
    // glUseProgram(grid->getShader());
    // glBindVertexArray(grid->getVAO());
    // glDrawArrays(GL_TRIANGLES, 0, 3);
    // glutSwapBuffers();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mat4 instance = ( RotateZ(1.0 * index) * Translate( 0.0, 0.5 * BASE_HEIGHT, 0.0 ) *
		 Scale( BASE_WIDTH,
			BASE_HEIGHT,
			BASE_WIDTH ) );

            index++;

    for (int i = 0; i < 4; i++) {
        printf("%f\t%f\t%f\t%f\n", instance._m[i].x, instance._m[i].y, instance._m[i].z, instance._m[i].w);
    }
    printf("\n");

    glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view * instance);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glutSwapBuffers();

    // // If the game isn't paused and hasn't ended, redraw the board
    // if (!gameState.isPaused() && !gameState.gameIsOver()) {

    //     double currentTime     = gameState.getCurrentTime();
    //     double nextRefreshTime = gameState.getNextRefreshTime();

    //     // Limits the frame rate to limit the necessary operations
    //     if (currentTime >= nextRefreshTime) {

    //         double refreshRate = gameState.getRefreshRate();
    //         double currentTime = gameState.getCurrentTime();
    //         Board* gameBoard   = gameState.getBoard();

    //         // Lock player key input during critical tetromino calculations
    //         gameState.toggleInputLock();
    //         gameState.setNextRefreshTime(currentTime + refreshRate);

    //         if (!gameBoard->tetrominoIsOnBoard()) {
    //             restartTetromino();
    //         }

    //         // Drop the tetromino down one block if enough time has elapsed
    //         if (currentTime >= gameState.getNextTickTime()) {
    //             gameState.setNextTickTime(currentTime + gameState.getTickLength());
    //             moveTetrominoDown();
    //         }

    //         if (rotateL) {
    //             renderer.rotateWorldLeft(gameState.getBoard(), grid);
    //         }
    //         if (rotateR) {
    //             renderer.rotateWorldRight(gameState.getBoard(), grid);
    //         }

    //         // Allow for player key input
    //         gameState.toggleInputLock();
            
    //         // Draw the resulting game board
    //         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //         renderer.draw(gameBoard, grid);
    //         glutSwapBuffers();
    //     }
    // }
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

void redraw(int width, int height) {

    //test this
    glViewport( 0, 0, width, height );

    GLfloat  left = -10.0, right = 10.0;
    GLfloat  bottom = -5.0, top = 15.0;
    GLfloat  zNear = -10.0, zFar = 10.0;

    GLfloat aspect = GLfloat(width)/height;

    if ( aspect > 1.0 ) {
	left *= aspect;
	right *= aspect;
    }
    else {
	bottom /= aspect;
	top /= aspect;
    }

    mat4 projection = Ortho( left, right, bottom, top, zNear, zFar );

    // for (int i = 0; i < 4; i++) {
    //     printf("%f\t%f\t%f\t%f\n", projection._m[i].x, projection._m[i].y, projection._m[i].z, projection._m[i].w);
    // }
    // printf("\n");

    glUniformMatrix4fv( Projection, 1, GL_TRUE, projection );

    model_view = mat4( 1.0 );  // An Identity matrix
}

int main(int argc, char** argv) {

    Coordinate origin;
    origin.x = 0;
    origin.y = 0;

    Dimension gridSize;
    gridSize.width  = 10;
    gridSize.height = 20;
    gridSize.depth  = 1;

    // Seed the random number generator with system clock
    srand(time(NULL));

    // // Initialize GLUT and init locally-defined GLUT settings
    // glutInit(&argc, argv);
    // initGlutSettings();

    // // Compatibility thing for GLEW v2.0
    // // glewExperimental = GL_TRUE;

	// if (glewInit() == GLEW_OK) {
    //     printf("Using GLEW version %s.\n", glewGetString(GLEW_VERSION));
    // }
    // else {
    //     printf("glew not ok\n");
    //     exit(1);
    // }

    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
    glutInitWindowSize( 512, 512 );
    const GLubyte* glString = glGetString(GL_VERSION);
    glutInitContextVersion( 3, 2 );
    glutInitContextProfile( GLUT_CORE_PROFILE );
    glutCreateWindow( "robot" );

    // glewExperimental = GL_TRUE; 
    glewInit();

    
    // Initialize grid object shaders
    grid = new Grid(20, 10, gridSize, origin);
    Shader shader("./src/test.vert", "./src/test.frag");
    grid->setShader(shader.getID());

    glClearColor(1.0, 1.0, 1.0, 1.0);
    // glEnable(GL_BLEND);
    // glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Provide the callback for window redrawing
    glutDisplayFunc(playGame);
    glutReshapeFunc(redraw);

    // Provide the callback for ASCII key presses
    glutKeyboardFunc(regularKeyCallback);

    // Provide callbacks for key up/down arrow presses
    glutSpecialFunc(specialKeyCallbackDown);
    glutSpecialUpFunc(specialKeyCallbackUp);

    // ---------------init stuff----------------------------

    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
    }; 

    // Create a vertex array object
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Create a vertex buffer object
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), NULL, GL_STATIC_DRAW);
    glUseProgram(shader.getID());

    // GLuint vPosition = glGetAttribLocation(shader.getID(), "vPosition");
    // glEnableVertexAttribArray(vPosition);
    // glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    // GLuint vColor = glGetAttribLocation(shader.getID(), "vColor");
    // glEnableVertexAttribArray(vColor);
    // glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    
    ModelView  = glGetUniformLocation(shader.getID(), "ModelView");
    Projection = glGetUniformLocation(shader.getID(), "Projection");

    glEnable(GL_DEPTH);
    glClearColor(1.0, 1.0, 1.0, 1.0);
    // -----------------------------------------------------


    // // Create a vertex array object
    // GLuint VAO;
    // glGenVertexArrays( 1, &VAO );
    // glBindVertexArray( VAO );



    // // Create and initialize a buffer object
    // GLuint buffer;
    // glGenBuffers( 1, &buffer );
    // glBindBuffer( GL_ARRAY_BUFFER, buffer );
    // glBufferData( GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(colors),
	// 	  NULL, GL_DYNAMIC_DRAW );
    // glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices );
    // glBufferSubData( GL_ARRAY_BUFFER, sizeof(vertices), sizeof(colors), colors );

    // glUseProgram(shader.getID());
    // #define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))
    // GLuint vPosition = glGetAttribLocation( shader.getID(), "vPosition" );
    // glEnableVertexAttribArray( vPosition );
    // glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0,
	// 		   BUFFER_OFFSET(0) );

    // GLuint vColor = glGetAttribLocation( shader.getID(), "vColor" );
    // glEnableVertexAttribArray( vColor );
    // glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0,
	// 		   BUFFER_OFFSET(sizeof(vertices)) );

    // glEnable( GL_DEPTH );
    // glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );


    /*
     *
     * try # 2 failed */
    // GLuint VBO, VAO;
    // glGenVertexArrays(1, &VAO);
    // glGenBuffers(1, &VBO);
    // glBindVertexArray(VAO);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    // glEnableVertexAttribArray(0);
    


    


    /*
     *
     * try # 1 failed */
    // GLuint VBO, VAO, EBO;
    // glGenVertexArrays(1, &VAO);
    // glGenBuffers(1, &VBO);
    // glGenBuffers(1, &EBO);

    // glBindVertexArray(VAO);

    // glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    // glEnableVertexAttribArray(0);

    // glBindBuffer(GL_ARRAY_BUFFER, 0);
    // glBindVertexArray(0);

    // grid->setVAO(VAO);
    // grid->setShader(shader.getID());

    // Call the main loop for GLUT
    glutMainLoop();
    
    return 0;
}
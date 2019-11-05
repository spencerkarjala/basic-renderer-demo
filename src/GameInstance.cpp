#include "GameInstance.h"
#include <stdexcept>
#include <chrono>

#define GAME_WIDTH  200
#define GAME_HEIGHT 400
#define NUM_COLUMNS 10
#define NUM_ROWS    20

GameInstance::GameInstance() {
    this->tickLength      = 1000.0;
    this->refreshRate     = 1000.0 / 30;
    this->nextTickTime    = this->getCurrentTime() + this->tickLength;
    this->nextRefreshTime = this->getCurrentTime() + this->refreshRate;
    this->nextHoldTime    = this->getCurrentTime() + this->tickLength;
    this->paused          = false;
    this->gameOver        = false;
    this->timerExpired    = false;
    this->timerMax        = 5;
    this->timer           = 5;
    this->board = new Board(GAME_WIDTH-20, GAME_HEIGHT-20, NUM_COLUMNS, NUM_ROWS, 10, 10);
}
GameInstance::~GameInstance() {
    delete this->board;
}

double GameInstance::getTickLength() {
    return this->tickLength;
}
void GameInstance::setTickLength(double tickLength) {
    this->tickLength = tickLength;
}

double GameInstance::getRefreshRate() {
    return this->refreshRate;
}
void GameInstance::setRefreshRate(double refreshRate) {
    this->refreshRate = refreshRate;
}

void GameInstance::flipPause() {
    this->paused = this->paused ^ 1;
}
bool GameInstance::isPaused() {
    return this->paused;
}

void GameInstance::toggleTimerExpired() {
    this->timerExpired = this->timerExpired ^ 1;
}
bool GameInstance::timerHasExpired() {
    return this->timerExpired;
}

void GameInstance::toggleGameOver() {
    this->gameOver = this->gameOver ^ 1;
}

bool GameInstance::gameIsOver() {
    return this->gameOver;
}

double GameInstance::getNextTickTime() {
    return this->nextTickTime;
}
void GameInstance::setNextTickTime(double nextTickTime) {
    this->nextTickTime = nextTickTime;
}

double GameInstance::getNextRefreshTime() {
    return this->nextRefreshTime;
}
void GameInstance::setNextRefreshTime(double nextRefreshTime) {
    this->nextRefreshTime = nextRefreshTime;
}

double GameInstance::getNextHoldTime() {
    return this->nextHoldTime;
}
void GameInstance::setNextHoldTime(double nextHoldTime) {
    this->nextHoldTime = nextHoldTime;
}

int GameInstance::getTimer() {
    return this->timer;
}

void GameInstance::decrementTimer() {
    
    if (this->timer == 0) {
        this->timerExpired = true;
    }

    int   a        = this->timer;
    int   b        = this->timerMax + 1;
    
    this->timer = (a % b + b - 1) % b;
}

void GameInstance::resetTimer() {
    this->timer = this->timerMax;
}

Board* GameInstance::getBoard() {
    return this->board;
}

// Retrieves the current system time as a double
double GameInstance::getCurrentTime() {

    using namespace std::chrono;

    // Obtain the current amount of time since the epoch
    system_clock::time_point currentTime  = system_clock::now();
    system_clock::duration timeSinceEpoch = currentTime.time_since_epoch();
    milliseconds timeInMs = duration_cast<milliseconds>(timeSinceEpoch);

    // Convert the result to a double and return it
    return timeInMs.count();
}

// Completely overwrites all game data and starts a new game
void GameInstance::restart() {

    delete this->board;

    this->tickLength      = 1000.0;
    this->refreshRate     = 1000.0 / 30;
    this->nextTickTime    = this->getCurrentTime() + this->tickLength;
    this->nextRefreshTime = this->getCurrentTime() + this->refreshRate;
    this->nextHoldTime    = this->getCurrentTime() + this->tickLength;
    this->paused          = false;
    this->gameOver        = false;
    this->timerExpired    = false;
    this->timer           = 5;
    this->board = new Board(GAME_WIDTH-20, GAME_HEIGHT-20, NUM_COLUMNS, NUM_ROWS, 10, 10);
}
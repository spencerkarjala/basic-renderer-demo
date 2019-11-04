#ifndef GAME_INSTANCE_H
#define GAME_INSTANCE_H

/*  Stores the game state information and provides a simple API to track game
    information using the system's clock.                                       */

#include "Board.h"

class GameInstance {

    public:
    GameInstance();
    ~GameInstance();

    double getTickLength();
    void setTickLength(double tickLength);

    double getRefreshRate();
    void setRefreshRate(double refreshRate);

    void flipPause();
    bool isPaused();

    void toggleGameOver();
    bool gameIsOver();

    void toggleTimerExpired();
    bool timerHasExpired();
    
    double getNextTickTime();
    void setNextTickTime(double nextTickTime);

    double getNextRefreshTime();
    void setNextRefreshTime(double nextTickTime);

    double getNextHoldTime();
    void setNextHoldTime(double nextHoldTime);

    int getTimer();
    bool decrementTimer();
    void resetTimer();

    Board* getBoard();

    double getCurrentTime();

    void restart();

    private:
    double tickLength;
    double refreshRate;
    double nextTickTime;
    double nextRefreshTime;
    double nextHoldTime;
    int timerMax;
    int timer;
    bool paused;
    bool gameOver;
    bool timerExpired;
    Board* board;
};

#endif
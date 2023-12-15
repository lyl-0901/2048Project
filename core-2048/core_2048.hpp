#ifndef CORE2048_HPP
#define CORE2048_HPP

#include <iostream>
#include <vector>

using namespace std;



class Core2048{
public:
    Core2048(); 
    void printScore();
    int getScore();
    void printGameOverMessage();
    int generateRandomNumber(double probability);
    void initializeBoard();
    void printBoard();
    void generateNewNumber();
    //check if the game is over
    bool isGameOver();
    //get the move direction from the user
    //W: up, S: down, A: left, D: right
    //Press Q to quit the game
    char getMoveDirection();
    //move the numbers on the board
    void moveNumbers(char moveDirection);

    bool isValidMoveUp();
    bool isValidMoveDown();
    bool isValidMoveLeft();
    bool isValidMoveRight();


    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();

    //for the tempBoard
    void moveUp(vector<vector<int>> &b);
    void moveDown(vector<vector<int>>& b);
    void moveLeft(vector<vector<int>>& b);
    void moveRight(vector<vector<int>>& b);

    //check if is valid to merge when moving up
    bool isValidUpMerge(int row, int col,const vector<vector<int>>& b);
    //check if is valid to merge when moving down
    bool isValidDownMerge(int row, int col,const vector<vector<int>>& b);
    //check if is valid to merge when moving left
    bool isValidLeftMerge(int row, int col,const vector<vector<int>>& b);
    //check if is valid to merge when moving right
    bool isValidRightMerge(int row, int col,const vector<vector<int>>& b);

    bool isValidUpMerge(int row, int col);
    bool isValidDownMerge(int row, int col);
    bool isValidLeftMerge(int row, int col);
    bool isValidRightMerge(int row, int col);

    //for the board
    void mergeUp();
    void mergeDown();
    void mergeLeft();
    void mergeRight();

    //overload the function for checking if is valid to move
    void mergeUp(vector<vector<int>>& b);
    //overload the function mergeDown for checking if is valid to move
    void mergeDown(vector<vector<int>>& b);
    //overload the function mergeLeft for checking if is valid to move
    void mergeLeft(vector<vector<int>>& b);
    //overload the function mergeRight for checking if is valid to move
    void mergeRight(vector<vector<int>>& b);

    int getTile(int row, int col);

    void setTargetTile(int value);
    void setFourTileProbability(double probability);
    void setBestScore(int score);

    int getTargetTile();
    double getFourTileProbability();
    int getBestScore();
    void updateBestScore();
    void resetScore(); 
    bool hasAchievedTargetTile();

    void updateConfigFile();
    
private:
    vector<vector<int> > board;
    int score = 0;
    int targetTile=2048;
    double fourTileProbability=0.1;
    int bestScore;
};

#endif // CORE2048_HPP

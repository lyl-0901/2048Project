#ifndef CORE_2048_CPP
#define CORE_2048_CPP

// the core code of 2048 game
#include"core_2048.hpp"
#include"../single_include/nlohmann/json.hpp"
#include<fstream>
using namespace std;

Core2048::Core2048(){
    ifstream configFile("config.json");
    nlohmann::json config;
    configFile >> config;
    configFile.close();
    bestScore = config["bestScore"];
    targetTile = config["targetTile"];
    fourTileProbability = config["fourTileProbability"];
    score = 0;
}

void Core2048::printScore(){
    cout<<"Your Score: "<<score;
}

int Core2048::getScore(){
    return score;
}

void Core2048::printGameOverMessage(){
    cout<<"Game Over!"<<" You got "<<score<<" scores!"<<endl;
}

//pass a double number to the function which is the probability of generating 4
int Core2048::generateRandomNumber(double probability){
    int randomNumber = rand() % 10;
    if(randomNumber < probability * 10){
        return 4;
    }
    else{
        return 2;
    }
}

//initialize the game board, randomly generate two 2s on the board
void Core2048::initializeBoard(){
    board = vector<vector<int> >(4, vector<int>(4, 0));
    int randomRow1 = rand() % 4;
    int randomCol1 = rand() % 4;
    int randomRow2 = rand() % 4;
    int randomCol2 = rand() % 4;
    while(randomRow1 == randomRow2 && randomCol1 == randomCol2){
        randomRow2 = rand() % 4;
        randomCol2 = rand() % 4;
    }
    board[randomRow1][randomCol1] = 2;
    board[randomRow2][randomCol2] = 2;
}

//print the game board
void Core2048::printBoard(){
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            cout<<board[i][j]<<"\t";
        }
        cout<<endl;
    }
}

//generate a new number on the board
void Core2048::generateNewNumber(){
    int randomNumber = generateRandomNumber(fourTileProbability);
    int randomRow = rand() % 4;
    int randomCol = rand() % 4;
    while(board[randomRow][randomCol] != 0){
        randomRow = rand() % 4;
        randomCol = rand() % 4;
    }
    board[randomRow][randomCol] = randomNumber;
}

//check if the game is over
bool Core2048::isGameOver(){
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            if(board[i][j] == 0){
                return false;
            }
        }
    }
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            if(isValidMoveUp() || isValidMoveDown() || isValidMoveLeft() || isValidMoveRight()){
                return false;
            }
            if(isValidUpMerge(i, j) || isValidDownMerge(i, j) || isValidLeftMerge(i, j) || isValidRightMerge(i, j)){
                return false;
            }
        }
    }
    return true;
}

//get the move direction from the user
//W: up, S: down, A: left, D: right
//Press Q to quit the game
char Core2048::getMoveDirection(){
    char moveDirection;
    cout<<"Next move:";
    cin>>moveDirection;
    return moveDirection;
}

//move the numbers on the board
void Core2048::moveNumbers(char moveDirection){
    if(moveDirection == 'W'){
        if(!isValidMoveUp()){
            cout<<"Invalid move!"<<endl;
            return;
        }
        moveUp();
        mergeUp();
        //move after merge
        moveUp();
    }
    else if(moveDirection == 'S'){
        if(!isValidMoveDown()){
            cout<<"Invalid move!"<<endl;
            return;
        }
        moveDown();
        mergeDown();
        moveDown();
    }
    else if(moveDirection == 'A'){
        if(!isValidMoveLeft()){
            cout<<"Invalid move!"<<endl;
            return;
        }
        moveLeft();
        mergeLeft();
        moveLeft();
    }
    else if(moveDirection == 'D'){
        if(!isValidMoveRight()){
            cout<<"Invalid move!"<<endl;
            return;
        }
        moveRight();
        mergeRight();
        moveRight();
    }
    else if(moveDirection=='Q'){
        cout<<"You quit the game!"<<endl;
        exit(0);
    }
    else {
        cout<<"Please enter A (Left) or D (Right) or W (Up) or Q (Quit)!"<<endl;
        return;
    }
    generateNewNumber();
    return;
}

//check if is valid to move up
//if the board doesn't change after moving, then it is not valid
bool Core2048::isValidMoveUp(){
    vector<vector<int> > tempBoard(4, vector<int>(4, 0));
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            tempBoard[i][j] = board[i][j];
        }
    }
    moveUp(tempBoard);
    mergeUp(tempBoard); 
    moveUp(tempBoard);
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            if(tempBoard[i][j] != board[i][j]){
                return true;
            }
        }
    }
    return false;
}

//check if is valid to move down
bool Core2048::isValidMoveDown(){
    vector<vector<int> > tempBoard(4, vector<int>(4, 0));
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            tempBoard[i][j] = board[i][j];
        }
    }
    moveDown(tempBoard);
    mergeDown(tempBoard); 
    moveDown(tempBoard);
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            if(tempBoard[i][j] != board[i][j]){
                return true;
            }
        }
    }
    return false;
}

//check if is valid to move left
bool Core2048::isValidMoveLeft(){
    vector<vector<int> > tempBoard(4, vector<int>(4, 0));
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            tempBoard[i][j] = board[i][j];
        }
    }
  
    moveLeft(tempBoard);
    mergeLeft(tempBoard); 
    moveLeft(tempBoard);
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            if(tempBoard[i][j] != board[i][j]){
                return true;
            }
        }
    }
    return false;
}

//check if is valid to move right
bool Core2048::isValidMoveRight(){
    vector<vector<int> > tempBoard(4, vector<int>(4, 0));
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            tempBoard[i][j] = board[i][j];
        }
    }
    moveRight(tempBoard);
    mergeRight(tempBoard); 
    moveRight(tempBoard);
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            if(tempBoard[i][j] != board[i][j]){
                return true;
            }
        }
    }
    return false;
}


//move up
void Core2048::moveUp(vector<vector<int>>& b){
    for(int col=0; col<4; col++){
    //create a vector to store non-zero numbers in each column
    vector<int> nonZeroNumbers;
        for(int row=0;row<4;row++){
            if(b[row][col]!=0){
                nonZeroNumbers.push_back(b[row][col]);
            }
        }
        //move the numbers up
        for(int row=0; row<4; row++){
            if(row < nonZeroNumbers.size()){
                b[row][col] = nonZeroNumbers[row];
            }
            else{
                b[row][col] = 0;
            }
        }
    }
}

void Core2048::moveUp(){
    for(int col=0; col<4; col++){
    //create a vector to store non-zero numbers in each column
    vector<int> nonZeroNumbers;
        for(int row=0;row<4;row++){
            if(board[row][col]!=0){
                nonZeroNumbers.push_back(board[row][col]);
            }
        }
        //move the numbers up
        for(int row=0; row<4; row++){
            if(row < nonZeroNumbers.size()){
                board[row][col] = nonZeroNumbers[row];
            }
            else{
                board[row][col] = 0;
            }
        }
    }
}

//move down
void Core2048::moveDown(vector<vector<int>>& b){
    for(int col=0; col<4; col++){
    //create a vector to store non-zero numbers in each column
    vector<int> nonZeroNumbers;
        for(int row=3;row>=0;row--){
            if(b[row][col]!=0){
                nonZeroNumbers.push_back(b[row][col]);
            }
        }
        //move the numbers down
        for(int row=3; row>=0; row--){
            if(3-row < nonZeroNumbers.size()){
                b[row][col] = nonZeroNumbers[3-row];
            }
            else{
                b[row][col] = 0;
            }
        }
    }
}

void Core2048::moveDown(){
    for(int col=0; col<4; col++){
    //create a vector to store non-zero numbers in each column
    vector<int> nonZeroNumbers;
        for(int row=3;row>=0;row--){
            if(board[row][col]!=0){
                nonZeroNumbers.push_back(board[row][col]);
            }
        }
        //move the numbers down
        for(int row=3; row>=0; row--){
            if(3-row < nonZeroNumbers.size()){
                board[row][col] = nonZeroNumbers[3-row];
            }
            else{
                board[row][col] = 0;
            }
        }
    }
}

//move left
void Core2048::moveLeft(vector<vector<int>>& b){
    for(int row=0; row<4; row++){
    //create a vector to store non-zero numbers in each row
    vector<int> nonZeroNumbers;
        for(int col=0;col<4;col++){
            if(b[row][col]!=0){
                nonZeroNumbers.push_back(b[row][col]);
            }
        }
        //move the numbers left
        for(int col=0; col<4; col++){
            if(col < nonZeroNumbers.size()){
                b[row][col] = nonZeroNumbers[col];
            }
            else{
                b[row][col] = 0;
            }
        }
    }
}

void Core2048::moveLeft(){
    for(int row=0; row<4; row++){
    //create a vector to store non-zero numbers in each row
    vector<int> nonZeroNumbers;
        for(int col=0;col<4;col++){
            if(board[row][col]!=0){
                nonZeroNumbers.push_back(board[row][col]);
            }
        }
        //move the numbers left
        for(int col=0; col<4; col++){
            if(col < nonZeroNumbers.size()){
                board[row][col] = nonZeroNumbers[col];
            }
            else{
                board[row][col] = 0;
            }
        }
    }
}

//move right
void Core2048::moveRight(vector<vector<int>>& b){
    for(int row=0; row<4; row++){
    //create a vector to store non-zero numbers in each row
    vector<int> nonZeroNumbers;
        for(int col=3;col>=0;col--){
            if(b[row][col]!=0){
                nonZeroNumbers.push_back(b[row][col]);
            }
        }
        //move the numbers right
        for(int col=3; col>=0; col--){
            if(3-col < nonZeroNumbers.size()){
                b[row][col] = nonZeroNumbers[3-col];
            }
            else{
                b[row][col] = 0;
            }
        }
    }
}

void Core2048::moveRight(){
    for(int row=0; row<4; row++){
    //create a vector to store non-zero numbers in each row
    vector<int> nonZeroNumbers;
        for(int col=3;col>=0;col--){
            if(board[row][col]!=0){
                nonZeroNumbers.push_back(board[row][col]);
            }
        }
        //move the numbers right
        for(int col=3; col>=0; col--){
            if(3-col < nonZeroNumbers.size()){
                board[row][col] = nonZeroNumbers[3-col];
            }
            else{
                board[row][col] = 0;
            }
        }
    }
}

//check if is valid to merge when moving up
bool Core2048::isValidUpMerge(int row, int col, const vector<vector<int>>& b){
    if(row == 0){
        return false;
    }
    else if(b[row-1][col] == 0){
        return false;
    }
    else if(b[row-1][col] != b[row][col]){
        return false;
    }
    else{
        return true;
    }
}

bool Core2048::isValidUpMerge(int row, int col){
    if(row == 0){
        return false;
    }
    else if(board[row-1][col] == 0){
        return false;
    }
    else if(board[row-1][col] != board[row][col]){
        return false;
    }
    else{
        return true;
    }
}

//check if is valid to merge when moving down
bool Core2048::isValidDownMerge(int row, int col, const vector<vector<int>>& b){
    if(row == 3){
        return false;
    }
    else if(b[row+1][col] == 0){
        return false;
    }
    else if(b[row+1][col] != b[row][col]){
        return false;
    }
    else{
        return true;
    }
}

bool Core2048::isValidDownMerge(int row, int col){
    if(row == 3){
        return false;
    }
    else if(board[row+1][col] == 0){
        return false;
    }
    else if(board[row+1][col] != board[row][col]){
        return false;
    }
    else{
        return true;
    }
}

//check if is valid to merge when moving left
bool Core2048::isValidLeftMerge(int row, int col, const vector<vector<int>>& b){
    if(col == 0){
        return false;
    }
    else if(b[row][col-1] == 0){
        return false;
    }
    else if(b[row][col-1] != b[row][col]){
        return false;
    }
    else{
        return true;
    }
}

bool Core2048::isValidLeftMerge(int row, int col){
    if(col == 0){
        return false;
    }
    else if(board[row][col-1] == 0){
        return false;
    }
    else if(board[row][col-1] != board[row][col]){
        return false;
    }
    else{
        return true;
    }
}



//check if is valid to merge when moving right
bool Core2048::isValidRightMerge(int row, int col){
    if(col == 3){
        return false;
    }
    else if(board[row][col+1] == 0){
        return false;
    }
    else if(board[row][col+1] != board[row][col]){
        return false;
    }
    else{
        return true;
    }
}

bool Core2048::isValidRightMerge(int row, int col, const vector<vector<int>>& b){
    if(col == 3){
        return false;
    }
    else if(b[row][col+1] == 0){
        return false;
    }
    else if(b[row][col+1] != b[row][col]){
        return false;
    }
    else{
        return true;
    }
}

//merge the numbers on the board when moving up
//for the overloaded function, it will not change the score
void Core2048::mergeUp(vector<vector<int>>& b){
    for(int col=0; col<4; col++){
        for(int row=0; row<4; row++){
            if(isValidUpMerge(row, col, b)){
                b[row-1][col] = b[row-1][col] * 2;
                b[row][col] = 0;
            }
        }
    }
}


void Core2048::mergeUp(){
    for(int col=0; col<4; col++){
        for(int row=0; row<4; row++){
            if(isValidUpMerge(row, col, board)){
                board[row-1][col] = board[row-1][col] * 2;
                board[row][col] = 0;
                score += board[row-1][col];
            }
        }
    }
}

//merge the numbers on the board when moving down
void Core2048::mergeDown(vector<vector<int>>& b){
    for(int col=0; col<4; col++){
        for(int row=3; row>=0; row--){
            if(isValidDownMerge(row, col, b)){
                b[row+1][col] = b[row+1][col] * 2;
                b[row][col] = 0;
            }
        }
    }
}

void Core2048::mergeDown(){
    for(int col=0; col<4; col++){
        for(int row=3; row>=0; row--){
            if(isValidDownMerge(row, col)){
                board[row+1][col] = board[row+1][col] * 2;
                board[row][col] = 0;
                score += board[row+1][col];
            }
        }
    }
}


//merge the numbers on the board when moving left
//for the temple board, it will not change the score
void Core2048::mergeLeft(vector<vector<int>>& b){
    for(int row=0; row<4; row++){
        for(int col=0; col<4; col++){
            if(isValidLeftMerge(row, col, b)){
                b[row][col-1] = b[row][col-1] * 2;
                b[row][col] = 0;
            }
        }
    }
}

void Core2048::mergeLeft(){
    for(int row=0; row<4; row++){
        for(int col=0; col<4; col++){
            if(isValidLeftMerge(row, col)){
                board[row][col-1] = board[row][col-1] * 2;
                board[row][col] = 0;
                score+=board[row][col-1];
            }
        }
    }
}

//merge the numbers on the board when moving right
void Core2048::mergeRight(vector<vector<int>>& b){
    for(int row=0; row<4; row++){
        for(int col=3; col>=0; col--){
            if(isValidRightMerge(row, col,b)){
                b[row][col+1] = b[row][col+1] * 2;
                b[row][col] = 0;
            }
        }
    }
}

void Core2048::mergeRight(){
    for(int row=0; row<4; row++){
        for(int col=3; col>=0; col--){
            if(isValidRightMerge(row, col)){
                board[row][col+1] = board[row][col+1] * 2;
                board[row][col] = 0;
                score += board[row][col+1];
            }
        }
    }
}




int Core2048::getTile(int row, int col){
    if (row >= 0 && row < board.size() && col >= 0 && col < board[0].size()) {
        return board[row][col];
    } else {
        std::cerr << "Error: Invalid row or column index\n";
        return -1; // or some other error value
    }
}

void Core2048::setBestScore(int score){
    bestScore = score;
}

void Core2048::setTargetTile(int value){
    targetTile = value;
}

void Core2048::setFourTileProbability(double probability){
    fourTileProbability = probability;
}

int Core2048::getBestScore(){
    return bestScore;
}

int Core2048::getTargetTile(){
    return targetTile;
}

double Core2048::getFourTileProbability(){
    return fourTileProbability;
}

void Core2048::updateBestScore(){
    if(score > bestScore){
        bestScore = score;
    }
}

void Core2048::resetScore(){
    score = 0;
}   

bool Core2048::hasAchievedTargetTile(){
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            if(board[i][j] == targetTile){
                return true;
            }
        }
    }
    return false;
    
}

void Core2048::updateConfigFile(){
    nlohmann::json config; 
    config["bestScore"] = bestScore;
    config["targetTile"] = targetTile;
    config["fourTileProbability"] = fourTileProbability;
    std::ofstream configFile("config.json");
    configFile << config.dump(4);
    configFile.close();
}

#endif

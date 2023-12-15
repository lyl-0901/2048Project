#include "../core-2048/core_2048.hpp"
#include<fstream>
#include"../single_include/nlohmann/json.hpp"

using namespace std;
using json = nlohmann::json;

int main(){
    Core2048 game;

    // Display current settings
    cout << "Current settings:" << endl;
    cout << "Target tile: " << game.getTargetTile() << endl;
    cout << "Probability of 4: " << game.getFourTileProbability() << endl;

    // Ask user if they want to change settings
    char changeSettings;
    cout << "Do you want to change these settings? (Y/N): ";
    cin >> changeSettings;

    if(toupper(changeSettings) == 'Y'){
        // Change target tile
        int newTargetTile;
        cout << "Enter new target tile: ";
        cin >> newTargetTile;
        game.setTargetTile(newTargetTile);

        //Change probability of 4
        double newFourTileProbability;
        cout << "Enter new probability of 4: ";
        cin >> newFourTileProbability;
        game.setFourTileProbability(newFourTileProbability);

        // Write new settings to config file
        game.updateConfigFile();

    }

    cout << "Best score: " << game.getBestScore() << endl;
    cout<<"Do you want to reset the best score? (Y/N): ";
    char resetBestScore;
    cin>>resetBestScore;
    if(toupper(resetBestScore) == 'Y'){
        game.resetScore();
        game.updateConfigFile();
    }


    cout<<"Welcome to 2048!"<<endl;
    cout<<"Please enter A (Left) or D (Right) or W (Up) or Q (Quit)!"<<endl;
    game.initializeBoard();
    while(!game.isGameOver()){
        game.printBoard();
        cout<<game.getTargetTile()<<endl;
        if(game.hasAchievedTargetTile()){
            cout<<"You have achieved the target tile!"<<endl;
            cout<<"Would you like to continue to play? (Y/N)"<<endl;
            char continuePlay;
            cin>>continuePlay;
            if(toupper(continuePlay) == 'Y'){
                game.setTargetTile(game.getTargetTile() * 2);
            }
            else{
                break;
            }
        }
        game.printScore();
        cout<<" "<<"Best Score:"<<game.getBestScore()<<endl;

        game.updateConfigFile();

        char moveDirection = game.getMoveDirection();
        game.moveNumbers(moveDirection); // Pass the 'board' as an argument
        game.updateBestScore();
        
    }
    //write the best score to the config file
   game.updateConfigFile();

    game.printBoard();
    game.printGameOverMessage();
    
    return 0;
}



       

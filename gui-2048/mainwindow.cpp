#include "mainwindow.h"
#include <QMainWindow>
#include <QPushButton>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QIODevice>
#include <QMenuBar>
#include <QInputDialog>
#include <QComboBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), game() // Instantiate the game here
{
    // Initialize the grid layout and tiles
    gridLayout = new QGridLayout;
    QWidget *centralW = new QWidget; // Rename this variable
    centralW->setLayout(gridLayout); // Use the new name here

    this->resize(800, 800); // Set the size of the window
    this->setWindowTitle("2048"); // Set the title of the window

    // Create the menu bar
    menuBar = new QMenuBar;
    setMenuBar(menuBar);

    // Create the "Settings" menu
    settingsMenu = new QMenu(tr("&Settings"), this);
    menuBar->addMenu(settingsMenu);

    // Create the "Set Target Tile" action
    setTargetTileAction = new QAction(tr("Set Target Tile"), this);
    settingsMenu->addAction(setTargetTileAction);

    // Create the "Set Four Tile Probability" action
    setFourTileProbabilityAction = new QAction(tr("Set Four Tile Probability"), this);
    settingsMenu->addAction(setFourTileProbabilityAction);

    //Create the "Clear Best Score" action
    clearBestScoreAction = new QAction(tr("Clear Best Score"), this);
    settingsMenu->addAction(clearBestScoreAction);

    connect(setTargetTileAction, &QAction::triggered, this, &MainWindow::showSetTargetTileDialog);
    connect(setFourTileProbabilityAction, &QAction::triggered, this, &MainWindow::showSetFourTileProbabilityDialog);
    connect(clearBestScoreAction, &QAction::triggered, this, [this](){
        game.setBestScore(0);
        updateGUI();
    });

    // Initialize the score label
    scoreLabel = new QLabel;
    scoreLabel->setAlignment(Qt::AlignCenter);
    scoreLabel->setFixedHeight(50);
    gridLayout->addWidget(scoreLabel, 0, 0, 1, 1); // Add the score label to the top of the grid

    // Initialize the tiles
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j <4; ++j) {
            tiles[i][j] = new QLabel;
            tiles[i][j]->setAlignment(Qt::AlignCenter);
            tiles[i][j]->setFrameStyle(QFrame::Panel | QFrame::Sunken);
            tiles[i][j]->setLineWidth(0);
            tiles[i][j]->setFixedSize(150,150);
            QFont font = tiles[i][j]->font();
            font.setPointSize(30); 
            tiles[i][j]->setFont(font);
            gridLayout->addWidget(tiles[i][j], i+1, j); // Add the tiles to the grid layout
        }
    }
    QPushButton *endGameButton = new QPushButton("End Game");
    gridLayout->addWidget(endGameButton, 0, 3, 1, 1); // Add the button below the grid
    connect(endGameButton, &QPushButton::clicked, this, &MainWindow::close);

    readConfigFile();
    //show the best score and target tile
    bestScoreLabel = new QLabel;
    bestScoreLabel->setAlignment(Qt::AlignCenter);
    bestScoreLabel->setFixedHeight(50);
    bestScoreLabel->setText("Best Score: " + QString::number(game.getBestScore()));
    gridLayout->addWidget(bestScoreLabel, 0, 1, 1, 1); // Add the score label to the top of the grid
    
    targetTileLabel = new QLabel;
    targetTileLabel->setAlignment(Qt::AlignCenter);
    targetTileLabel->setFixedHeight(50);
    targetTileLabel->setText("Target Tile: " + QString::number(game.getTargetTile()));
    gridLayout->addWidget(targetTileLabel, 0, 2, 1, 1); // Add the score label to the top of the grid


    setCentralWidget(centralW); // And here
    // Initialize the game board and update the GUI
    game.initializeBoard();
    updateGUI();
}

MainWindow::~MainWindow()
{
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            delete tiles[i][j];
        }
    }
    delete gridLayout;
    delete scoreLabel;

}

void MainWindow::keyPressEvent(QKeyEvent *event)
{

    // Handle keyboard input here
    char moveDirection;
    // Handle key presses
    switch (event->key()) {
    case Qt::Key_W:
        moveDirection = 'W';
        break;
    case Qt::Key_S:
        moveDirection = 'S';
        break;
    case Qt::Key_A:
        moveDirection = 'A';
        break;
    case Qt::Key_D:
        moveDirection = 'D';
        break;
    case Qt::Key_Q:
        moveDirection = 'Q';
        break;
    default:
        QMainWindow::keyPressEvent(event);
        return;
    }
    game.moveNumbers(moveDirection);
    //update the best score
    game.updateBestScore();
    writeConfigFile();
    // Update the tiles
    updateGUI();
    if(game.hasAchievedTargetTile()){
        QMessageBox msgBox;
        msgBox.setText("You have achieved the target tile! Would you like to continue to play?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);  
        msgBox.setDefaultButton(QMessageBox::Yes);
        int ret = msgBox.exec();
        switch (ret) {
        case QMessageBox::Yes:
            game.setTargetTile(game.getTargetTile() * 2);
            writeConfigFile();
            updateGUI();
            break;
        case QMessageBox::No:
            close();
            break;
        }
    }
    // Check if the game is over
    if (game.isGameOver()) {
        writeConfigFile();
        //tell the player the score and ask if they want to play again
        QMessageBox msgBox;
        msgBox.setText("Game Over! Your score was " + QString::number(game.getScore()) + ". Would you like to play again?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);  
        msgBox.setDefaultButton(QMessageBox::Yes);
        int ret = msgBox.exec();
        switch (ret) {
        case QMessageBox::Yes:
            game.initializeBoard();
            updateGUI();
            break;
        case QMessageBox::No:
            close();
            break;
        }
        
    }

}

void MainWindow::updateGUI()
{
    readConfigFile();
    scoreLabel->setText("Score: " + QString::number(game.getScore()));
    //show the best score and target tile
    bestScoreLabel->setText("Best Score: " + QString::number(game.getBestScore()));
    targetTileLabel->setText("Target Tile: " + QString::number(game.getTargetTile())); 
    // Update the text of each QLabel to reflect the current state of the game board
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            tiles[i][j]->setText(QString::number(game.getTile(i, j)));
            int number = game.getTile(i, j); // Get the number on the tile
            QString color;
            switch (number) {
            case 2: color = "lightgray"; break;
            case 4: color = "lightblue"; break;
            case 8: color = "orange"; break;
            case 16: color = "yellow"; break;
            case 32: color = "green"; break;
            case 64: color = "cyan"; break;
            case 128: color = "blue"; break;
            case 256: color = "red"; break;
            case 512: color = "magenta"; break;
            case 1024: color = "darkred"; break;
            case 2048: color = "darkblue"; break;
            default: color = "white"; break;
            }
            tiles[i][j]->setStyleSheet("QLabel { background-color : " + color + "; color : black; border: 1px solid black;}");
        }
    }
}


void MainWindow::readConfigFile() {
    QFile file("config.json");
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open config file.");
        return;
    }

    QByteArray data = file.readAll();
    QJsonDocument doc(QJsonDocument::fromJson(data));
    QJsonObject json = doc.object();

    int targetTile = json["targetTile"].toInt();
    double fourTileProbability = json["fourTileProbability"].toDouble();
    int bestScore = json["bestScore"].toInt();

    game.setTargetTile(targetTile);
    game.setFourTileProbability(fourTileProbability);
    game.setBestScore(bestScore);

}

void MainWindow::writeConfigFile() {
    QFile file("config.json");
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open config file.");
        return;
    }

    QJsonObject json;
    json["targetTile"] = game.getTargetTile();
    json["fourTileProbability"] = game.getFourTileProbability();
    json["bestScore"] = game.getBestScore();

    QJsonDocument doc(json);
    file.write(doc.toJson());
}

void MainWindow::showSetTargetTileDialog()
{
    bool ok;
    int newTargetTile = QInputDialog::getInt(this, tr("Set Target Tile"), tr("Target Tile:"), game.getTargetTile(), 2, 2048, 1, &ok);
    if (ok)
        game.setTargetTile(newTargetTile);
        writeConfigFile();
        updateGUI();
}


void MainWindow::showSetFourTileProbabilityDialog() {
    bool ok;
    double fourTileProbability = QInputDialog::getDouble(this, tr("Set Four Tile Probability"), tr("Four Tile Probability:"), game.getFourTileProbability(), 0.0, 1.0, 2, &ok);
    if (ok) {
        game.setFourTileProbability(fourTileProbability);
        writeConfigFile();
        updateGUI();
    }
}

/*
void MainWindow::on_actionQuit_triggered()
{
    close();
}
*/
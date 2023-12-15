#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QGridLayout>
#include <QLabel>
#include <QKeyEvent>
#include <QMessageBox>
#include <QComboBox>
#include "../core-2048/core_2048.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    //void on_actionQuit_triggered();
    void showSetTargetTileDialog();
    void showSetFourTileProbabilityDialog();


private:
    Ui::MainWindow *ui;
    QGridLayout *gridLayout;
    QLabel *tiles[4][4];
    QLabel *scoreLabel;
    QLabel *targetTileLabel;
    QLabel *bestScoreLabel;
    QMenuBar *menuBar;
    QMenu *settingsMenu;
    QAction *setTargetTileAction;
    QAction *setFourTileProbabilityAction;
    QAction *clearBestScoreAction;
    //QComboBox *targetTileComboBox;
    Core2048 game; // Instance of your game logic class
    int score = 0;
    void readConfigFile();
    void writeConfigFile();
    void updateGUI();
    
};
#endif // MAINWINDOW_H

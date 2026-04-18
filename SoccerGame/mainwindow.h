#pragma once

#include <QtWidgets/QMainWindow>

class QStackedWidget;
class MenuScreen;
class PlayerSelectScreen;
class SoccerGame;
class EndGameScreen;

class MainWindow : public QMainWindow
{
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override = default;

private:
    void showPlayerSelectScreen();
    void showMenuScreen();
    void startGameWithPlayers(int p1Type, int p2Type);
    void showEndGameScreen(int score1, int score2);
    void replayLastMatch();

    QStackedWidget* stackedWidget;
    MenuScreen* menuScreen;
    PlayerSelectScreen* playerSelectScreen;
    SoccerGame* gameScreen;
    EndGameScreen* endGameScreen = nullptr;
};

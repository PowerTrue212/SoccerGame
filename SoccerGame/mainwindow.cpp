#include "mainwindow.h"

#include "constants.h"
#include "endgamescreen.h"
#include "menuscreen.h"
#include "playerselectscreen.h"
#include "soccergame.h"

#include <QPushButton>
#include <QStackedWidget>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      stackedWidget(new QStackedWidget(this)),
      menuScreen(new MenuScreen(this)),
      playerSelectScreen(new PlayerSelectScreen(this)),
      gameScreen(new SoccerGame(this))
{
    setWindowTitle("SoccerGame");
    setFixedSize(Constants::WindowWidth, Constants::WindowHeight);

    setCentralWidget(stackedWidget);
    gameScreen->setFocusPolicy(Qt::StrongFocus);
    stackedWidget->addWidget(menuScreen);
    stackedWidget->addWidget(playerSelectScreen);
    stackedWidget->addWidget(gameScreen);
    stackedWidget->setCurrentWidget(menuScreen);

    connect(menuScreen->startGameButton(), &QPushButton::clicked, this, &MainWindow::showPlayerSelectScreen);
    connect(playerSelectScreen, &PlayerSelectScreen::backToMenu, this, &MainWindow::showMenuScreen);
    connect(playerSelectScreen, &PlayerSelectScreen::playersSelected, this, &MainWindow::startGameWithPlayers);
    connect(gameScreen, &SoccerGame::gameFinished, this, &MainWindow::showEndGameScreen);
}

void MainWindow::showPlayerSelectScreen()
{
    stackedWidget->setCurrentWidget(playerSelectScreen);
}

void MainWindow::showMenuScreen()
{
    stackedWidget->setCurrentWidget(menuScreen);
}

void MainWindow::startGameWithPlayers(int p1Type, int p2Type)
{
    gameScreen->setPlayers(p1Type, p2Type);
    gameScreen->restartMatch();
    stackedWidget->setCurrentWidget(gameScreen);
    gameScreen->setFocus();
}

void MainWindow::showEndGameScreen(int score1, int score2)
{
    if (!endGameScreen) {
        endGameScreen = new EndGameScreen(score1, score2, this);
        stackedWidget->addWidget(endGameScreen);
        connect(endGameScreen, &EndGameScreen::playAgain, this, &MainWindow::replayLastMatch);
        connect(endGameScreen, &EndGameScreen::backToMenu, this, &MainWindow::showMenuScreen);
    }
    else {
        endGameScreen->setResult(score1, score2);
    }

    stackedWidget->setCurrentWidget(endGameScreen);
}

void MainWindow::replayLastMatch()
{
    gameScreen->restartMatch();
    stackedWidget->setCurrentWidget(gameScreen);
    gameScreen->setFocus();
}

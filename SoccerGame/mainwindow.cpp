#include "mainwindow.h"

#include "constants.h"
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
    connect(gameScreen, &SoccerGame::gameFinished, this, &MainWindow::showMenuScreen);
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

#include "mainwindow.h"

#include "constants.h"
#include "endgamescreen.h"
#include "menuscreen.h"
#include "playerselectscreen.h"
#include "soccergame.h"

#include <QAudioOutput>
#include <QMediaPlayer>
#include <QPushButton>
#include <QStackedWidget>
#include <QString>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      stackedWidget(new QStackedWidget(this)),
      menuScreen(new MenuScreen(this)),
      playerSelectScreen(new PlayerSelectScreen(this)),
      gameScreen(new SoccerGame(this)),
      backgroundPlayer(new QMediaPlayer(this)),
      backgroundAudio(new QAudioOutput(this))
{
    setWindowTitle("SoccerGame");
    setFixedSize(Constants::WindowWidth, Constants::WindowHeight);

    setCentralWidget(stackedWidget);
    gameScreen->setFocusPolicy(Qt::StrongFocus);
    backgroundPlayer->setAudioOutput(backgroundAudio);
    stackedWidget->addWidget(menuScreen);
    stackedWidget->addWidget(playerSelectScreen);
    stackedWidget->addWidget(gameScreen);
    stackedWidget->setCurrentWidget(menuScreen);

    setBackgroundAudio("qrc:/SoccerGame/music/start.mp4");

    connect(menuScreen->startGameButton(), &QPushButton::clicked, this, &MainWindow::showPlayerSelectScreen);
    connect(playerSelectScreen, &PlayerSelectScreen::backToMenu, this, &MainWindow::showMenuScreen);
    connect(playerSelectScreen, &PlayerSelectScreen::playersSelected, this, &MainWindow::startGameWithPlayers);
    connect(gameScreen, &SoccerGame::gameFinished, this, &MainWindow::showEndGameScreen);
}

void MainWindow::showPlayerSelectScreen()
{
    stackedWidget->setCurrentWidget(playerSelectScreen);
    setBackgroundAudio("qrc:/SoccerGame/music/start.mp4");
}

void MainWindow::showMenuScreen()
{
    stackedWidget->setCurrentWidget(menuScreen);
    setBackgroundAudio("qrc:/SoccerGame/music/start.mp4");
}

void MainWindow::startGameWithPlayers(int p1Type, int p2Type)
{
    gameScreen->setPlayers(p1Type, p2Type);
    gameScreen->restartMatch();
    stackedWidget->setCurrentWidget(gameScreen);
    gameScreen->setFocus();
    setBackgroundAudio("qrc:/SoccerGame/music/game.mp4");
}

void MainWindow::showEndGameScreen(int score1, int score2)
{
    if (!endGameScreen) {
        endGameScreen = new EndGameScreen(score1, score2, this);
        stackedWidget->addWidget(endGameScreen);
        connect(endGameScreen, &EndGameScreen::playAgain, this, &MainWindow::replayLastMatch);
        connect(endGameScreen, &EndGameScreen::backToMenu, this, &MainWindow::showMenuScreen);
        connect(endGameScreen, &EndGameScreen::celebrationFinished, this, [this](const QString& videoPath) {
            if (!videoPath.isEmpty()) {
                setBackgroundAudio(videoPath);
            }
        });
    }
    else {
        endGameScreen->setResult(score1, score2);
    }

    const int winnerType = (score1 > score2) ? gameScreen->getPlayer1Type() : (score2 > score1) ? gameScreen->getPlayer2Type() : -1;
    const QString videoSource = celebrationVideoForPlayer(winnerType);
    endGameScreen->playCelebrationVideo(videoSource);
    stopBackgroundAudio();

    stackedWidget->setCurrentWidget(endGameScreen);
}

void MainWindow::replayLastMatch()
{
    gameScreen->restartMatch();
    stackedWidget->setCurrentWidget(gameScreen);
    gameScreen->setFocus();
    setBackgroundAudio("qrc:/SoccerGame/music/game.mp4");
}

QString MainWindow::celebrationVideoForPlayer(int playerType) const
{
    switch (playerType) {
    case 0:
        return "qrc:/SoccerGame/music/enzo_.mp4";
    case 1:
        return "qrc:/SoccerGame/music/alvarez_.mp4";
    case 2:
        return "qrc:/SoccerGame/music/rice_.mp4";
    case 3:
        return "qrc:/SoccerGame/music/haland_.mp4";
    case 4:
        return "qrc:/SoccerGame/music/kane_.mp4";
    case 5:
        return "qrc:/SoccerGame/music/dembele_.mp4";
    default:
        return QString();
    }
}

void MainWindow::setBackgroundAudio(const QString& source)
{
    if (currentAudioSource == source) {
        return;
    }

    currentAudioSource = source;
    backgroundPlayer->stop();
    if (!source.isEmpty()) {
        backgroundPlayer->setSource(QUrl(source));
        backgroundPlayer->setLoops(QMediaPlayer::Infinite);
        backgroundPlayer->play();
    }
}

void MainWindow::stopBackgroundAudio()
{
    currentAudioSource.clear();
    backgroundPlayer->stop();
}

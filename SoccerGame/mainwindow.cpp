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
#include <QShortcut>
#include <QKeySequence>

// 初始化主窗口、界面栈以及背景音效。
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

    auto* debugShortcut = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_E), this);
    connect(debugShortcut, &QShortcut::activated, this, [this]() {
        gameScreen->setPlayers(0, 1);
        showEndGameScreen(2, 1);
    });
}

// 切换到选人界面。
void MainWindow::showPlayerSelectScreen()
{
    stackedWidget->setCurrentWidget(playerSelectScreen);
    setBackgroundAudio("qrc:/SoccerGame/music/start.mp4");
}

// 切换回菜单界面。
void MainWindow::showMenuScreen()
{
    stackedWidget->setCurrentWidget(menuScreen);
    setBackgroundAudio("qrc:/SoccerGame/music/start.mp4");
}

// 根据玩家选择启动比赛。
void MainWindow::startGameWithPlayers(int p1Type, int p2Type)
{
    gameScreen->setPlayers(p1Type, p2Type);
    gameScreen->restartMatch();
    stackedWidget->setCurrentWidget(gameScreen);
    gameScreen->setFocus();
    setBackgroundAudio("qrc:/SoccerGame/music/game.mp4");
}

// 显示结束界面并触发胜利视频。
void MainWindow::showEndGameScreen(int score1, int score2)
{
    const int player1Type = gameScreen->getPlayer1Type();
    const int player2Type = gameScreen->getPlayer2Type();

    if (!endGameScreen) {
        endGameScreen = new EndGameScreen(score1, score2, player1Type, player2Type, this);
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
        endGameScreen->setResult(score1, score2, player1Type, player2Type);
    }

    const int winnerType = (score1 > score2) ? player1Type : (score2 > score1) ? player2Type : -1;
    const QString videoSource = celebrationVideoForPlayer(winnerType);
    endGameScreen->playCelebrationVideo(videoSource);
    stopBackgroundAudio();

    stackedWidget->setCurrentWidget(endGameScreen);
}

// 重新开始上一场比赛。
void MainWindow::replayLastMatch()
{
    gameScreen->restartMatch();
    stackedWidget->setCurrentWidget(gameScreen);
    gameScreen->setFocus();
    setBackgroundAudio("qrc:/SoccerGame/music/game.mp4");
}

// 根据球员类型选择庆祝视频。
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
        return "qrc:/SoccerGame/music/dembele_.mp4";
    case 5:
        return "qrc:/SoccerGame/music/kane_.mp4";
    default:
        return QString();
    }
}

// 切换背景音效播放源。
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

// 停止背景音效播放。
void MainWindow::stopBackgroundAudio()
{
    currentAudioSource.clear();
    backgroundPlayer->stop();
}

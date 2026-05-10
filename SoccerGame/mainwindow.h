#pragma once

#include <QtWidgets/QMainWindow>

class QAudioOutput;
class QMediaPlayer;

class QStackedWidget;
class MenuScreen;
class PlayerSelectScreen;
class SoccerGame;
class EndGameScreen;

// 主窗口，负责界面切换与音频控制。
class MainWindow : public QMainWindow
{
public:
    // 初始化主窗口与界面栈。
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override = default;

private:
    // 切换到选人界面。
    void showPlayerSelectScreen();
    // 切换回菜单界面。
    void showMenuScreen();
    // 根据玩家选择启动比赛。
    void startGameWithPlayers(int p1Type, int p2Type);
    // 显示结束界面。
    void showEndGameScreen(int score1, int score2);
    // 重新开始上一场比赛。
    void replayLastMatch();
    // 根据球员类型获取庆祝视频路径。
    QString celebrationVideoForPlayer(int playerType) const;
    // 设置背景音频播放源。
    void setBackgroundAudio(const QString& source);
    // 停止背景音频播放。
    void stopBackgroundAudio();

    QStackedWidget* stackedWidget;
    MenuScreen* menuScreen;
    PlayerSelectScreen* playerSelectScreen;
    SoccerGame* gameScreen;
    EndGameScreen* endGameScreen = nullptr;
    QMediaPlayer* backgroundPlayer;
    QAudioOutput* backgroundAudio;
    QString currentAudioSource;
};

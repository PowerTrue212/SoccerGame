#pragma once

#include <QtWidgets/QWidget>
#include <QPainter>
#include <QTimer>
#include <QMouseEvent>
#include "player.h"
#include "ball.h"

// 比赛主界面，负责渲染场景与比赛逻辑更新。
class SoccerGame : public QWidget
{
    Q_OBJECT

public:
    // 初始化比赛界面与比赛数据。
    SoccerGame(QWidget *parent = nullptr);
    ~SoccerGame();
    // 设置双方球员类型。
    void setPlayers(int p1Type, int p2Type);
    // 重置并重新开始比赛。
    void restartMatch();
    // 获取玩家1的球员类型。
    int getPlayer1Type() const;
    // 获取玩家2的球员类型。
    int getPlayer2Type() const;

signals:
    // 比赛结束时触发。
    void gameFinished(int score1, int score2);

protected:
    // 处理球员与足球的碰撞检测。
    void collision();
    // 绘制比赛场景。
    void paintEvent(QPaintEvent* event) override;
    // 定时器驱动的比赛更新逻辑。
    void timerEvent(QTimerEvent* event) override;
    // 处理键盘按下事件。
    void keyPressEvent(QKeyEvent* event) override;
    // 处理键盘松开事件。
    void keyReleaseEvent(QKeyEvent* event) override;
    // 处理鼠标点击事件。
    void mousePressEvent(QMouseEvent* event) override;

private:
	Player *player1;
	Player *player2;
    Ball *ball;
    int gameTimerId;
    int score1 = 0;
    int score2 = 0;
    int remainingMs = 90000;
    bool matchFinished = false;
    int player1Type = 0;
    int player2Type = 0;

    void resetPositions();
    void checkGoalAndScore();
};


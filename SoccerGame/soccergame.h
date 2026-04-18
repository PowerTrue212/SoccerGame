#pragma once

#include <QtWidgets/QWidget>
#include <QPainter>
#include <Qtimer>
#include <QMouseEvent>
#include "player.h"
#include "ball.h"

class SoccerGame : public QWidget
{
    Q_OBJECT

public:
    SoccerGame(QWidget *parent = nullptr);
    ~SoccerGame();
    void setPlayers(int p1Type, int p2Type);

signals:
    void gameFinished(int score1, int score2);

protected:
    void collision();
	void paintEvent(QPaintEvent* event) override;
	void timerEvent(QTimerEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
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
    void restartMatch();
    void checkGoalAndScore();
};


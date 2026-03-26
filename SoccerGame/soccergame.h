#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_soccergame.h"
#include <QPainter>
#include <Qtimer>
#include "player.h"
#include "ball.h"

class SoccerGame : public QMainWindow
{
    Q_OBJECT

public:
    SoccerGame(QWidget *parent = nullptr);
    ~SoccerGame();

protected:
    void collision();
	void paintEvent(QPaintEvent* event) override;
	void timerEvent(QTimerEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;

private:
    Ui::SoccerGameClass ui;
	Player *player1;
	Player *player2;
    Ball *ball;
    int gameTimerId;
};


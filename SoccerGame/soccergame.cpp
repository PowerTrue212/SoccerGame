#include "soccergame.h"
#include "player.h"
#include "ball.h"
#include "constants.h"
#include "physicscollision.h"
#include <qobject.h>
#include <qpainter.h>
#include <QKeyEvent>

SoccerGame::SoccerGame(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	this->resize(Constants::WindowWidth, Constants::WindowHeight);
	this->setWindowTitle("SoccerGame");

	player1 = new Player();
	player2 = new Player();
	ball = new Ball();

	player1->pos = QPointF(100, Constants::GroundLevel - Constants::PlayerHeight);
	player2->pos = QPointF(900, Constants::GroundLevel - Constants::PlayerHeight);
	player1->playerface = 1;
	player2->playerface = -1;
	gameTimerId = startTimer(16);
}

void SoccerGame::paintEvent(QPaintEvent* event) {
	QPainter painter(this);
	
	player1->DrawPlayer(&painter);
	player2->DrawPlayer(&painter);
	ball->DrawBall(&painter);

}

void SoccerGame::collision() {
	Player* players[2] = { player1, player2 };
	PhysicsCollision::checkPlayerCollision(player1, player2);
	for (auto player : players) {
		if (!PhysicsCollision::checkCollideBody(player, ball))
			PhysicsCollision::checkCollideHead(player, ball);
	}
}

void SoccerGame::timerEvent(QTimerEvent* ) {
	player1->update();
	player2->update();
	ball->update();

	collision();

	update();
}

void SoccerGame::keyPressEvent(QKeyEvent *event) {
	if (event->key() == Qt::Key_A) player1->move(-1);
	if (event->key() == Qt::Key_D) player1->move(1);
	if (event->key() == Qt::Key_W) player1->jump();
	if (event->key() == Qt::Key_S) player1->iskick = true;

	if (event->key() == Qt::Key_Left) player2->move(-1);
	if (event->key() == Qt::Key_Right) player2->move(1);
	if (event->key() == Qt::Key_Up) player2->jump();
	if (event->key() == Qt::Key_Down) player2->iskick = true;
}

SoccerGame::~SoccerGame()
{
	delete player1;
	delete player2;
	delete ball;
}



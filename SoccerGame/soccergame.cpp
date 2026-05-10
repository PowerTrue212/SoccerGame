#include "soccergame.h"
#include "player.h"
#include "ball.h"
#include "constants.h"
#include "physicscollision.h"
#include <QObject.h>
#include <qpainter.h>
#include <QKeyEvent>
#include <QString>

SoccerGame::SoccerGame(QWidget *parent)
    : QWidget(parent)
{
	player1 = new Player();
	player2 = new Player();
	ball = new Ball();
	resetPositions();
	gameTimerId = startTimer(16);
}

void SoccerGame::paintEvent(QPaintEvent* event) {
	Q_UNUSED(event);
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);

	// 1. 绘制背景图（拉伸填满整个窗口）
	QPixmap bg(":/images/image/background.png");
	painter.drawPixmap(rect(), bg);

	///painter.fillRect(rect(), QColor(135, 206, 235));
	//painter.fillRect(0, Constants::GroundLevel, width(), height() - Constants::GroundLevel, QColor(80, 180, 80));
	painter.setBrush(Qt::NoBrush);
	painter.setPen(QPen(Qt::white, 3));
	painter.drawRect(0, Constants::GroundLevel - Constants::GoalHeight, Constants::GoalWidth, Constants::GoalHeight);
	painter.drawRect(Constants::WindowWidth - Constants::GoalWidth, Constants::GroundLevel - Constants::GoalHeight, Constants::GoalWidth, Constants::GoalHeight);
	painter.fillRect(0, Constants::GroundLevel - Constants::GoalHeight - Constants::GoalBarThickness, Constants::GoalWidth, Constants::GoalBarThickness, QColor(220, 220, 220));
	painter.fillRect(Constants::WindowWidth - Constants::GoalWidth, Constants::GroundLevel - Constants::GoalHeight - Constants::GoalBarThickness, Constants::GoalWidth, Constants::GoalBarThickness, QColor(220, 220, 220));
	
	player1->DrawPlayer(&painter);
	player2->DrawPlayer(&painter);
	ball->DrawBall(&painter);

   const QString abbreviations[] = {
		QStringLiteral("CHE"),
		QStringLiteral("ATM"),
		QStringLiteral("ARS"),
		QStringLiteral("MCI"),
		QStringLiteral("PSG"),
		QStringLiteral("FCB")
	};

	auto abbreviationForType = [&](int type) -> QString {
		if (type >= 0 && type < 6) {
			return abbreviations[type];
		}
		return QString();
	};

	auto teamColorForType = [&](int type) -> QColor {
		switch (type) {
		case 0:
			return QColor("#034694");
		case 1:
			return QColor("#CB3524");
		case 2:
			return QColor("#EF0107");
		case 3:
			return QColor("#6CABDD");
		case 4:
			return QColor("#004170");
		case 5:
			return QColor("#DC052D");
		default:
			return QColor("#444444");
		}
	};

	const QString p1Abbreviation = abbreviationForType(player1Type);
	const QString p2Abbreviation = abbreviationForType(player2Type);

  const int scoreY = 28;
	const int rectHeight = 42;
	const int rectWidth = 110;
	const int gap = 8;
	const int leftRectX = 24;
	const int rightRectX = leftRectX + rectWidth + gap;

	QFont scoreFont("Microsoft YaHei", 22, QFont::Bold);
	painter.setFont(scoreFont);
	painter.setPen(Qt::NoPen);

	painter.setBrush(teamColorForType(player1Type));
 painter.drawRect(QRect(leftRectX, scoreY, rectWidth, rectHeight));
	painter.setBrush(teamColorForType(player2Type));
    painter.drawRect(QRect(rightRectX, scoreY, rectWidth, rectHeight));

	painter.setPen(Qt::white);
	painter.drawText(QRect(leftRectX, scoreY, rectWidth, rectHeight), Qt::AlignCenter,
		QString("%1 %2").arg(p1Abbreviation).arg(score1));
	painter.drawText(QRect(rightRectX, scoreY, rectWidth, rectHeight), Qt::AlignCenter,
		QString("%1 %2").arg(score2).arg(p2Abbreviation));

	const int totalSeconds = remainingMs / 1000;
	const int mm = totalSeconds / 60;
	const int ss = totalSeconds % 60;

	const QString timeText = QString("%1:%2")
		.arg(mm, 2, 10, QLatin1Char('0'))
		.arg(ss, 2, 10, QLatin1Char('0'));
	QFont font("Arial", 50);        // 字体名称，像素/点大小
	font.setBold(true);             // 可选：加粗
	painter.setFont(font);          // 设置到 painter
	painter.drawText(rect(), Qt::AlignTop | Qt::AlignHCenter, timeText);

	if (matchFinished) {
		painter.drawText(rect(), Qt::AlignCenter, "Game Over - Left Click to Restart");
	}

}

void SoccerGame::collision() {
	Player* players[2] = { player1, player2 };
	PhysicsCollision::checkPlayerCollision(player1, player2);
	for (auto player : players) {
		if (!PhysicsCollision::checkCollideBody(player, ball))
			PhysicsCollision::checkCollideHead(player, ball);
	}
   PhysicsCollision::checkGoalCrossbarCollision(ball);
}

void SoccerGame::timerEvent(QTimerEvent* ) {
	if (matchFinished) {

		update();
		return;
	}

	player1->update();
	player2->update();
	ball->update();

	collision();
	checkGoalAndScore();

	remainingMs -= 16;
	if (remainingMs <= 0) {
		remainingMs = 0;
		matchFinished = true;
       emit gameFinished(score1, score2);
	}

	update();
}

void SoccerGame::keyPressEvent(QKeyEvent *event) {
   if (event->isAutoRepeat()) return;

	if (event->key() == Qt::Key_A) player1->move(-1);
	if (event->key() == Qt::Key_D) player1->move(1);
	if (event->key() == Qt::Key_W) player1->jump();
	if (event->key() == Qt::Key_S) player1->iskick = true;

	if (event->key() == Qt::Key_Left) player2->move(-1);
	if (event->key() == Qt::Key_Right) player2->move(1);
	if (event->key() == Qt::Key_Up) player2->jump();
	if (event->key() == Qt::Key_Down) player2->iskick = true;
}

void SoccerGame::keyReleaseEvent(QKeyEvent* event) {
	if (event->isAutoRepeat()) return;

	if (event->key() == Qt::Key_A || event->key() == Qt::Key_D) player1->move(0);
    if (event->key() == Qt::Key_S) player1->iskick = false;
	if (event->key() == Qt::Key_Left || event->key() == Qt::Key_Right) player2->move(0);
	if (event->key() == Qt::Key_Down) player2->iskick = false;
}

void SoccerGame::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton && matchFinished) {
		restartMatch();
		update();
	}
    QWidget::mousePressEvent(event);
}

void SoccerGame::setPlayers(int p1Type, int p2Type)
{
	player1Type = p1Type;
	player2Type = p2Type;

	const QString headshots[] = {
		":/SoccerGame/image/player_1.png",
		":/SoccerGame/image/player_2.png",
		":/SoccerGame/image/player_3.png",
		":/SoccerGame/image/player_4.png",
		":/SoccerGame/image/player_6.png",
		":/SoccerGame/image/player_5.png"
	};

	const QString bodies[] = {
		":/SoccerGame/image/CHE.png",
		":/SoccerGame/image/ATM.png",
		":/SoccerGame/image/ARS.png",
		":/SoccerGame/image/MCI.png",
		":/SoccerGame/image/PSG.png",
		":/SoccerGame/image/FCB.png"
	};

	if (p1Type >= 0 && p1Type < 6) {
		player1->setHeadshot(QPixmap(headshots[p1Type]));
       player1->setBody(QPixmap(bodies[p1Type]));
	}
	if (p2Type >= 0 && p2Type < 6) {
		player2->setHeadshot(QPixmap(headshots[p2Type]));
       player2->setBody(QPixmap(bodies[p2Type]));
	}
}

void SoccerGame::resetPositions()
{
	player1->pos = QPointF(300, Constants::GroundLevel - Constants::PlayerHeight);
	player2->pos = QPointF(1300, Constants::GroundLevel - Constants::PlayerHeight);
	player1->velocity = QPointF(0, 0);
	player2->velocity = QPointF(0, 0);
	player1->moveInput = 0;
	player2->moveInput = 0;
	player1->iskick = false;
	player2->iskick = false;
	player1->onGround = true;
	player2->onGround = true;
	player1->playerface = 1;
	player2->playerface = -1;

	ball->pos = QPointF(Constants::WindowWidth / 2.0, 100);
	ball->prevPos = ball->pos;
	ball->velocity = QPointF(0, 0);
	ball->onGround = false;
}

int SoccerGame::getPlayer1Type() const
{
	return player1Type;
}

int SoccerGame::getPlayer2Type() const
{
	return player2Type;
}

void SoccerGame::restartMatch()
{
	score1 = 0;
	score2 = 0;
	remainingMs = 90000;
	matchFinished = false;
	resetPositions();
}

void SoccerGame::checkGoalAndScore()
{
	const double goalTop = Constants::GroundLevel - Constants::GoalHeight;
	const bool inGoalHeight = ball->pos.y() >= goalTop && ball->pos.y() <= Constants::GroundLevel;
	if (!inGoalHeight) return;

	if (ball->pos.x() <= Constants::GoalWidth) {
		score2 += 1;
		resetPositions();
	}
	else if (ball->pos.x() >= Constants::WindowWidth - Constants::GoalWidth) {
		score1 += 1;
		resetPositions();
	}
}

SoccerGame::~SoccerGame()
{
	delete player1;
	delete player2;
	delete ball;
}



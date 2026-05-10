#pragma once

#include <QPointF>
#include <QPainter>
#include <QPixmap>

class Player
{
public:
	Player();
	
	void DrawPlayer(QPainter* painter) const;
	void setHeadshot(const QPixmap& pixmap);
	void setBody(const QPixmap& pixmap);

	void move(int orientation); // key press left or right
	void jump();
	void update();

	QPointF pos; // head center position
	QPointF velocity;

	bool onGround = true;
	bool iskick = false;
	int playerface;
	double acceleration;
	int moveInput = 0;
 QPixmap headshot;
	QPixmap body;
};


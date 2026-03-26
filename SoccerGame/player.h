#pragma once

#include <QPointF>
#include <QPainter>

class Player
{
public:
	Player();
	
	void DrawPlayer(QPainter* painter) const;

	void move(int orientation); // key press left or right
	void jump();
	void update();

	QPointF pos; // head center position
	QPointF velocity;

	bool onGround = true;
	bool iskick = false;
	int playerface;
	int acceleration;
	float friction = 0.85f;
};


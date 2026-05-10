#pragma once

#include <QPointF>
#include <QPainter>
#include <QPixmap>

class Ball
{
public:
	Ball();
	void DrawBall(QPainter* painter) const;
	void update();
	void Friction();

	QPointF pos; // 球的中心位置
	QPointF prevPos; // 上一个位置，用于检测连续碰撞
	QPointF velocity;
	QPointF acceleration;
	QPixmap sprite;

	bool onGround = false;
};


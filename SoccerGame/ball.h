#pragma once

#include <QPointF>
#include <QPainter>
#include <QPixmap>

/// 比赛用足球实体，负责球的物理状态与绘制。
class Ball
{
public:
    // 初始化足球位置、速度与贴图资源。
	Ball();
    // 绘制足球贴图或备用圆形。
	void DrawBall(QPainter* painter) const;
    // 更新足球物理状态并处理边界碰撞。
	void update();
   // 对地面接触时的水平速度施加摩擦力。
	void Friction();

	QPointF pos; // 球的中心位置
	QPointF prevPos; // 上一个位置，用于检测连续碰撞
	QPointF velocity;
	QPointF acceleration;
	QPixmap sprite;

	bool onGround = false;
};


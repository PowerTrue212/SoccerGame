#pragma once

#include <QPointF>
#include <QPainter>
#include <QPixmap>

// 球员实体，处理移动逻辑、绘制以及物理状态。
class Player
{
public:
  // 初始化球员物理参数。
	Player();

   // 绘制球员的身体与头像贴图。
	void DrawPlayer(QPainter* painter) const;
   // 设置球员头像贴图。
	void setHeadshot(const QPixmap& pixmap);
   // 设置球员身体贴图。
	void setBody(const QPixmap& pixmap);

   // 处理左右移动输入。
	// -1 左移，1 右移，0 停止。
  void move(int orientation); // 左右移动输入
 // 触发跳跃动作。
	void jump();
  // 更新球员位置与速度，并检测边界碰撞。
	void update();

    QPointF pos; // 头部中心位置
	QPointF velocity;

	bool onGround = true;
	bool iskick = false;
	int playerface;
	double acceleration;
	int moveInput = 0;
 QPixmap headshot;
	QPixmap body;
};


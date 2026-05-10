#include "player.h"
#include "constants.h"
#include "physicscollision.h"
#include <QPainter>
#include <QPainterPath>
#include <algorithm>
#include <cmath>

// 构造球员并初始化速度与重力加速度。
Player::Player()
{
	velocity = QPointF(0, 0);
	acceleration = Constants::Gravity;
}

// 设置头像贴图。
void Player::setHeadshot(const QPixmap& pixmap)
{
	headshot = pixmap;
}

// 设置身体贴图。
void Player::setBody(const QPixmap& pixmap)
{
	body = pixmap;
}

// 绘制球员身体与头像。
void Player::DrawPlayer(QPainter* painter)const {
    const QRectF bodyRect(
		pos.x() - Constants::PlayerBodyWidth * 0.5,
		pos.y() + Constants::PlayerRadius,
		Constants::PlayerBodyWidth,
		Constants::PlayerBodyHeight);

	if (!body.isNull()) {
		painter->drawPixmap(bodyRect.toRect(), body);
	} else {
		painter->setBrush(Qt::blue);
		painter->drawRect(bodyRect);
	}

	const double headScale = 1.25;
	const double headSize = Constants::PlayerRadius * 2.0 * headScale;
	const double headOffsetY = Constants::PlayerRadius * 0.25;
	const QRectF headRect(
		pos.x() - headSize * 0.5,
		pos.y() - headSize * 0.5 + headOffsetY,
		headSize,
		headSize);

	if (!headshot.isNull()) {
		QPainterPath clipPath;
		clipPath.addEllipse(headRect);
		painter->save();
		painter->setClipPath(clipPath);

		QPixmap pixmap = headshot;
		if (playerface == 1) {
			pixmap = pixmap.transformed(QTransform().scale(-1, 1));
		}
		painter->drawPixmap(headRect.toRect(), pixmap);
		painter->restore();
	}

}

// 处理球员水平移动输入。
void Player::move(int orientation) {
    moveInput = std::clamp(orientation, -1, 1);
}

// 触发球员跳跃。
void Player::jump()
{
	if (onGround == false) return;
	onGround = false;
    velocity.ry() = -Constants::JumpSpeed;
}

// 更新球员速度与位置，并处理边界碰撞。
void Player::update()
{
  const double ax = onGround ? Constants::PlayerGroundAccel : Constants::PlayerAirAccel;
	velocity.rx() += moveInput * ax;

	const double vmax = onGround ? Constants::PlayerMaxSpeedGround : Constants::PlayerMaxSpeedAir;
	velocity.setX(std::clamp(velocity.x(), -vmax, vmax));

	if (moveInput == 0) {
		velocity.rx() *= (onGround ? Constants::PlayerGroundDrag : Constants::PlayerAirDrag);
		if (std::abs(velocity.x()) < 0.08) velocity.setX(0.0);
	}

	velocity.ry() += acceleration;
	pos += velocity;

	PhysicsCollision::checkPlayerBoundary(this);
}

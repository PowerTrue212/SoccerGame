#include "player.h"
#include "constants.h"
#include "physicscollision.h"
#include <Qpainter>

Player::Player()
{
	velocity = QPointF(0, 0);
	acceleration = 1;
}

void Player::DrawPlayer(QPainter* painter)const {
	painter->setBrush(Qt::blue);
	painter->drawRect(pos.x() - Constants::PlayerBodyWidth * 0.5f, pos.y() + Constants::PlayerRadius,
		Constants::PlayerBodyWidth, Constants::PlayerBodyHeight);
	painter->drawEllipse(pos, Constants::PlayerRadius, Constants::PlayerRadius);
}

void Player::move(int orientation) {
	velocity.setX(orientation * 10);
}

void Player::jump()
{
	if (onGround == false) return;
	onGround = false;
	velocity.ry() = -20;
}

void Player::update()
{
	velocity.ry() += acceleration;
	pos += velocity;

	PhysicsCollision::checkPlayerBoundary(this);

	if (onGround) {
		velocity.setX(velocity.x() * friction); // 地面摩擦力
		if (std::abs(velocity.x()) < 0.1) velocity.setX(0); // 避免无限小的速度
	}
}

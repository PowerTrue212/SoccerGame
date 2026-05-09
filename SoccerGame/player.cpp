#include "player.h"
#include "constants.h"
#include "physicscollision.h"
#include <Qpainter>
#include <QPainterPath>
#include <algorithm>
#include <cmath>

Player::Player()
{
	velocity = QPointF(0, 0);
   acceleration = Constants::Gravity;
}

void Player::setHeadshot(const QPixmap& pixmap)
{
	headshot = pixmap;
}

void Player::DrawPlayer(QPainter* painter)const {
	painter->setBrush(Qt::blue);
	painter->drawRect(pos.x() - Constants::PlayerBodyWidth * 0.5f, pos.y() + Constants::PlayerRadius,
		Constants::PlayerBodyWidth, Constants::PlayerBodyHeight);

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

void Player::move(int orientation) {
    moveInput = std::clamp(orientation, -1, 1);
}

void Player::jump()
{
	if (onGround == false) return;
	onGround = false;
    velocity.ry() = -Constants::JumpSpeed;
}

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

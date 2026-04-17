#include "ball.h"
#include "constants.h"
#include "physicscollision.h"
#include <cmath>

Ball::Ball()
{
   velocity = QPointF(0, 0);
	pos = QPointF(Constants::WindowWidth/2, 100);
	prevPos = pos;
   acceleration = QPointF(0, Constants::Gravity);
}

void Ball::DrawBall(QPainter* painter) const
{
	painter->setBrush(Qt::red);
	painter->drawEllipse(pos, Constants::BallRadius, Constants::BallRadius);
}

void Ball::Friction() {
	if (!onGround) return;
   velocity.rx() *= Constants::BallGroundFriction;
}

void Ball::update()
{
	prevPos = pos;
	velocity += acceleration;

	if (!onGround) {
		velocity *= Constants::BallAirDrag;
	}
	else {
		velocity.rx() *= Constants::BallGroundFriction;
	}

	const double speed = std::hypot(velocity.x(), velocity.y());
	if (speed > Constants::BallMaxSpeed) {
		velocity *= (Constants::BallMaxSpeed / speed);
	}

	pos += velocity;

	PhysicsCollision::checkBallBoundary(this);

}


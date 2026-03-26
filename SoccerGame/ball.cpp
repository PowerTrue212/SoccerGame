#include "ball.h"
#include "constants.h"
#include "physicscollision.h"

Ball::Ball()
{
	velocity = QPointF(1, 0);
	pos = QPointF(600, 100);
	prevPos = pos;
	acceleration = QPointF(0, 1);
}

void Ball::DrawBall(QPainter* painter) const
{
	painter->setBrush(Qt::red);
	painter->drawEllipse(pos, Constants::BallRadius, Constants::BallRadius);
}

void Ball::Friction() {
	if (!onGround) return;
	velocity.rx() *= 0.9;
}

void Ball::update()
{
	prevPos = pos;
	velocity += acceleration;
	pos += velocity;

	PhysicsCollision::checkBallBoundary(this);

}


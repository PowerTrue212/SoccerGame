#include "ball.h"
#include "constants.h"
#include "physicscollision.h"
#include <cmath>

// 构造足球并载入贴图资源。
Ball::Ball()
{
   velocity = QPointF(0, 0);
	pos = QPointF(Constants::WindowWidth/2, 100);
	prevPos = pos;
   acceleration = QPointF(0, Constants::Gravity);
    sprite = QPixmap(":/SoccerGame/image/soccer.png");
}

// 绘制足球贴图，若贴图不存在则绘制备用圆形。
void Ball::DrawBall(QPainter* painter) const
{
 if (!sprite.isNull()) {
		const int diameter = Constants::BallRadius * 2;
		const QRect targetRect(
			static_cast<int>(pos.x()) - Constants::BallRadius,
			static_cast<int>(pos.y()) - Constants::BallRadius,
			diameter,
			diameter);
		painter->drawPixmap(targetRect, sprite);
		return;
	}

	painter->setBrush(Qt::red);
	painter->drawEllipse(pos, Constants::BallRadius, Constants::BallRadius);
}

// 在足球接触地面时对水平速度应用摩擦。
void Ball::Friction() {
	if (!onGround) return;
   velocity.rx() *= Constants::BallGroundFriction;
}

// 更新足球物理状态、限制速度并处理边界碰撞。
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


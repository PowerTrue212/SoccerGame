#include "physicscollision.h"
#include "ball.h"
#include "constants.h"
#include "player.h"
#include <QDebug>
#include <algorithm>
#include <cmath>
#include <limits>

namespace
{
 // 轴对齐包围盒结构，用于碰撞检测。
	struct AABB
	{
		double left;
		double top;
		double right;
		double bottom;
	};

 // 将数值限制在指定区间内。
	static inline double clampd(double v, double lo, double hi)
	{
		return std::max(lo, std::min(v, hi));
	}

 // 扫掠点与包围盒的连续碰撞检测（p0->p1）。
	// 返回碰撞时间 t ∈ [0,1] 以及碰撞法线。
	bool sweptPointVsAABB(const QPointF& p0, const QPointF& p1, const AABB& box, double& tHit, QPointF& normal)
	{
		constexpr double inf = std::numeric_limits<double>::infinity();
		const double dx = p1.x() - p0.x();
		const double dy = p1.y() - p0.y();
		const double eps = 1e-9;

		double tEnter = -inf;
		double tExit = inf;
		normal = QPointF(0, 0);

       // X 轴区间
		if (std::abs(dx) < eps) {
			if (p0.x() < box.left || p0.x() > box.right) return false;
		}
		else {
			double tx1 = (box.left - p0.x()) / dx;
			double tx2 = (box.right - p0.x()) / dx;
			double txNear = std::min(tx1, tx2);
			double txFar = std::max(tx1, tx2);

			if (txNear > tEnter) {
				tEnter = txNear;
				normal = QPointF(dx > 0 ? -1.0 : 1.0, 0.0);
			}
			tExit = std::min(tExit, txFar);
		}

       // Y 轴区间
		if (std::abs(dy) < eps) {
			if (p0.y() < box.top || p0.y() > box.bottom) return false;
		}
		else {
			double ty1 = (box.top - p0.y()) / dy;
			double ty2 = (box.bottom - p0.y()) / dy;
			double tyNear = std::min(ty1, ty2);
			double tyFar = std::max(ty1, ty2);

			if (tyNear > tEnter) {
				tEnter = tyNear;
				normal = QPointF(0.0, dy > 0 ? -1.0 : 1.0);
			}
			tExit = std::min(tExit, tyFar);
		}

		if (tEnter > tExit) return false;
		if (tExit < 0.0) return false;
		if (tEnter > 1.0) return false;
        if (tEnter < 0.0) return false; // 起点在扩展包围盒内，交给离散分支处理

		tHit = tEnter;
		return true;
	}

  // 扫掠点与圆形的连续碰撞检测。
	bool sweptPointVsCircle(const QPointF& p0, const QPointF& p1, const QPointF& center, double radius, double& tHit, QPointF& normal)
	{
		const QPointF d = p1 - p0;
		const QPointF m = p0 - center;

		const double a = QPointF::dotProduct(d, d);
		const double b = 2.0 * QPointF::dotProduct(m, d);
		const double c = QPointF::dotProduct(m, m) - radius * radius;

		// 起点已在圆内：交给离散修正分支
		if (c <= 0.0) return false;
		if (a <= 1e-12) return false;

		const double disc = b * b - 4.0 * a * c;
		if (disc < 0.0) return false;

		const double sqrtDisc = std::sqrt(disc);
      const double t = (-b - sqrtDisc) / (2.0 * a); // 首次命中

		if (t < 0.0 || t > 1.0) return false;

		QPointF hitPos = p0 + d * t;
		normal = hitPos - center;
		const double nLen = std::sqrt(QPointF::dotProduct(normal, normal));
		if (nLen <= 1e-9) normal = QPointF(1.0, 0.0);
		else normal /= nLen;

		tHit = t;
		return true;
	}

  // 处理足球与包围盒的碰撞修正与反弹。
	bool resolveBallAABBCollision(Ball* ball, const AABB& box, double restitution)
	{
		double cx = clampd(ball->pos.x(), box.left, box.right);
		double cy = clampd(ball->pos.y(), box.top, box.bottom);
		QPointF closest(cx, cy);

		QPointF diff = ball->pos - closest;
		double dist2 = QPointF::dotProduct(diff, diff);
		const double r = Constants::BallRadius;
		if (dist2 >= r * r) return false;

		QPointF normal;
		double dist = std::sqrt(std::max(dist2, 1e-12));
		if (dist > 1e-6) {
			normal = diff / dist;
			ball->pos += normal * (r - dist);
		}
		else {
			double toLeft = std::abs(ball->pos.x() - box.left);
			double toRight = std::abs(box.right - ball->pos.x());
			double toTop = std::abs(ball->pos.y() - box.top);
			double toBottom = std::abs(box.bottom - ball->pos.y());
			double minD = std::min(std::min(toLeft, toRight), std::min(toTop, toBottom));

			if (minD == toLeft) normal = QPointF(-1, 0);
			else if (minD == toRight) normal = QPointF(1, 0);
			else if (minD == toTop) normal = QPointF(0, -1);
			else normal = QPointF(0, 1);

			ball->pos += normal * (r + 0.1);
		}

		double vn = QPointF::dotProduct(ball->velocity, normal);
		if (vn < 0.0) {
			ball->velocity -= (1.0 + restitution) * vn * normal;
		}
		return true;
	}
}

// 检测足球与球门横梁的碰撞。
void PhysicsCollision::checkGoalCrossbarCollision(Ball* ball)
{
	const double goalTop = Constants::GroundLevel - Constants::GoalHeight;

	const AABB leftBar{
		0.0,
		goalTop - Constants::GoalBarThickness,
		double(Constants::GoalWidth),
		goalTop
	};

	const AABB rightBar{
		double(Constants::WindowWidth - Constants::GoalWidth),
		goalTop - Constants::GoalBarThickness,
		double(Constants::WindowWidth),
		goalTop
	};

	const double restitution = 0.55;
	resolveBallAABBCollision(ball, leftBar, restitution);
	resolveBallAABBCollision(ball, rightBar, restitution);
}

// 处理足球与场地边界的碰撞修正。
void PhysicsCollision::checkBallBoundary(Ball* ball)
{
	// floor
	if (ball->pos.y() + Constants::BallRadius >= Constants::GroundLevel) {
		ball->pos.setY(Constants::GroundLevel - Constants::BallRadius);

		// 关键：小速度直接静止，避免地面“抖动反弹”累积能量
       if (std::abs(ball->velocity.y()) < Constants::BallSleepVy) {
			ball->velocity.setY(0.0);
		}
		else {
            ball->velocity.setY(-ball->velocity.y() * Constants::BallRestitutionGround); // 降低地面反弹系数
		}

		// friction on ground
      ball->velocity.setX(ball->velocity.x() * Constants::BallGroundFriction);
		ball->onGround = true;
	}
	else {
		ball->onGround = false;
	}

	// ceiling
	if (ball->pos.y() - Constants::BallRadius <= 0) {
		ball->pos.setY(Constants::BallRadius);
     ball->velocity.setY(-ball->velocity.y() * Constants::BallRestitutionWall);
	}

	// left wall
	if (ball->pos.x() - Constants::BallRadius <= 0) {
		ball->pos.setX(Constants::BallRadius);
     ball->velocity.setX(-ball->velocity.x() * Constants::BallRestitutionWall);
	}

	// right wall
	if (ball->pos.x() + Constants::BallRadius >= Constants::WindowWidth) {
		ball->pos.setX(Constants::WindowWidth - Constants::BallRadius);
     ball->velocity.setX(-ball->velocity.x() * Constants::BallRestitutionWall);
	}
}

// 处理球员与场地边界的碰撞修正。
void PhysicsCollision::checkPlayerBoundary(Player* player)
{
	if (player->pos.y() + Constants::PlayerHeight > Constants::GroundLevel) { // 判断球员是否在地面以下
     player->pos.setY(Constants::GroundLevel - Constants::PlayerHeight);
		player->onGround = true;
		player->velocity.ry() = 0;
	}

	//判断是否与左边界碰撞
	if (player->pos.x() - Constants::PlayerRadius < 0) {
		player->pos.setX(Constants::PlayerRadius);
		player->velocity.setX(0);
	}

	//判断是否与右边界碰撞
	if (player->pos.x() + Constants::PlayerRadius > Constants::WindowWidth) {
		player->pos.setX(Constants::WindowWidth - Constants::PlayerRadius);
		player->velocity.setX(0);
	}
}

// 处理两个球员之间的碰撞。
void PhysicsCollision::checkPlayerCollision(Player* player1, Player* player2)
{
	QPointF orient = player2->pos - player1->pos;
	double dist = std::sqrt(orient.x() * orient.x() + orient.y() * orient.y());
	double combinedR = double(2 * Constants::PlayerRadius);

	if (combinedR - dist <= 0.01) return;

	player1->velocity.setX(0);
	player2->velocity.setX(0);

	player1->pos.rx() -= 0.8 * (combinedR - dist) * orient.x() / dist;
	player2->pos.rx() += 0.8 * (combinedR - dist) * orient.x() / dist;
	
}

// 检测足球与球员头部的碰撞并计算反弹。
bool PhysicsCollision::checkCollideHead(Player* player, Ball* ball)
{
	const QPointF headCenter = player->pos;
	const double combinedR = double(Constants::PlayerRadius + Constants::BallRadius);

	bool hit = false;
	QPointF normal(0.0, 0.0);
	double tHit = 0.0;

	// 1) CCD：防高速穿模（用 ball->prevPos -> ball->pos 线段）
	if (sweptPointVsCircle(ball->prevPos, ball->pos, headCenter, combinedR, tHit, normal)) {
		const QPointF delta = ball->pos - ball->prevPos;
		const double kEps = 0.05;
		ball->pos = ball->prevPos + delta * tHit + normal * kEps;
		hit = true;
	}
	else {
		// 2) 离散兜底
		QPointF diff = ball->pos - headCenter;
		double dist2 = QPointF::dotProduct(diff, diff);

		if (dist2 < combinedR * combinedR) {
			double dist = std::sqrt(std::max(dist2, 1e-12));
			if (dist > 1e-6) {
				normal = diff / dist;
				double penetration = combinedR - dist;
				ball->pos += normal * penetration;
			}
			else {
				normal = QPointF(1.0, 0.0);
				ball->pos += normal * 0.1;
			}
			hit = true;
		}
	}

	if (!hit) return false;

	// 3) 速度响应
	QPointF vRel = ball->velocity - player->velocity;
	double vn = QPointF::dotProduct(vRel, normal);

	if (vn < 0.0) {
		const double mPlayer = 60.0;
		const double mBall = 0.5;
        const double restitution = 0.5;

		double j = -(1.0 + restitution) * vn / (1.0 / mPlayer + 1.0 / mBall);

		ball->velocity += (j / mBall) * normal;
		player->velocity -= (j / mPlayer) * normal;
	}

	// 避免头部碰撞后把球压到地面以下
	checkBallBoundary(ball);
	return true;
}

// 检测足球与球员身体/踢球区域的碰撞并计算反弹。
bool PhysicsCollision::checkCollideBody(Player* player, Ball* ball)
{
	if (player->iskick) {
     const QPointF kickCenter = player->pos + QPointF(0, Constants::PlayerHeight);
		const QPointF toBall = ball->pos - kickCenter;

		const double forward = toBall.x() * player->playerface;
		const double vertical = std::abs(toBall.y());
		const bool inKickZone =
			forward >= -Constants::KickRangeBackward &&
			forward <= Constants::KickRangeForward &&
			vertical <= Constants::KickRangeVertical;

		if (inKickZone) {
			const double fx = std::clamp(forward / Constants::KickRangeForward, 0.0, 1.0);
			const double vy = std::clamp(vertical / Constants::KickRangeVertical, 0.0, 1.0);

			double loft = (1.0 - fx) * 0.75 + (1.0 - vy) * 0.25 + Constants::KickLiftBias;
			loft = std::clamp(loft, 0.0, 1.0);

			const double angleDeg =
				Constants::KickMinAngleDeg +
				(Constants::KickMaxAngleDeg - Constants::KickMinAngleDeg) * loft;
			const double angleRad = angleDeg * 3.14159265358979323846 / 180.0;

			QPointF kickDirection(player->playerface * std::cos(angleRad), -std::sin(angleRad));

			const double zoneDistanceNorm = std::clamp(std::hypot(fx, vy), 0.0, 1.0);
			const double t = 1.0 - zoneDistanceNorm;
			double kickStrength = Constants::KickMinStrength + (Constants::KickMaxStrength - Constants::KickMinStrength) * t;

			ball->velocity += kickStrength * kickDirection;
			ball->velocity.rx() += player->velocity.x() * 0.35;
			player->iskick = false;
			return true;
		}
	}

	const double bodyLeft = player->pos.x() - Constants::PlayerBodyWidth * 0.5;
	const double bodyTop = player->pos.y() + Constants::PlayerRadius;
	const double bodyRight = bodyLeft + Constants::PlayerBodyWidth;
	const double bodyBottom = bodyTop + Constants::PlayerBodyHeight;

	AABB expanded{
		bodyLeft - Constants::BallRadius,
		bodyTop - Constants::BallRadius,
		bodyRight + Constants::BallRadius,
		bodyBottom + Constants::BallRadius
	};

	bool hit = false;
	QPointF normal(0, 0);
	double tHit = 0.0;

	if (sweptPointVsAABB(ball->prevPos, ball->pos, expanded, tHit, normal)) {
		const QPointF delta = ball->pos - ball->prevPos;
		const double kEps = 0.05;
		ball->pos = ball->prevPos + delta * tHit + normal * kEps;
		hit = true;
	}
	else {
		double cx = clampd(ball->pos.x(), bodyLeft, bodyRight);
		double cy = clampd(ball->pos.y(), bodyTop, bodyBottom);
		QPointF closest(cx, cy);

		QPointF diff = ball->pos - closest;
		double dist2 = diff.x() * diff.x() + diff.y() * diff.y();
		double r = Constants::BallRadius;

		if (dist2 < r * r) {
			double dist = std::sqrt(std::max(dist2, 1e-12));
			if (dist > 1e-6) {
				normal = diff / dist;
				double penetration = r - dist;
				ball->pos += normal * penetration;
			}
			else {
				double toLeft = std::abs(ball->pos.x() - bodyLeft);
				double toRight = std::abs(bodyRight - ball->pos.x());
				double toTop = std::abs(ball->pos.y() - bodyTop);
				double toBottom = std::abs(bodyBottom - ball->pos.y());

				double minD = std::min(std::min(toLeft, toRight), std::min(toTop, toBottom));
				if (minD == toLeft) normal = QPointF(-1, 0);
				else if (minD == toRight) normal = QPointF(1, 0);
				else if (minD == toTop) normal = QPointF(0, -1);
				else normal = QPointF(0, 1);

				ball->pos += normal * (r + 0.1);
			}
			hit = true;
		}
	}

	if (!hit) return false;

	// ===== 关键：踩球支撑约束（避免突然加速飞走）=====
	// normal.y() > 0 代表球在玩家身体下方；玩家竖直速度>0代表正在下落
	const bool playerFalling = player->velocity.y() > 0.0;
	const bool ballBelowBody = normal.y() > 0.5;
	const bool ballNearGround = (Constants::GroundLevel - (ball->pos.y() + Constants::BallRadius)) < 2.0;

	if (playerFalling && ballBelowBody && ballNearGround) {
		// 1) 让玩家“站在球顶”，脚底贴球顶
		const double ballTop = ball->pos.y() - Constants::BallRadius;
		player->pos.setY(ballTop - Constants::PlayerHeight);
		player->velocity.setY(0.0);
		player->onGround = true;

		// 2) 竖直方向不再给球爆发冲量
		ball->velocity.setY(0.0);

		// 3) 保留一点水平联动（可调）
		ball->velocity.setX(ball->velocity.x() * 0.98 + player->velocity.x() * 0.02);

		checkBallBoundary(ball);
		return true;
	}

	// 常规碰撞响应（降低恢复系数，减少“弹飞”）
	double vn = QPointF::dotProduct(ball->velocity - player->velocity, normal);
	if (vn < 0.0) {
		const double restitution = 0.25;
		ball->velocity -= (1.0 + restitution) * vn * normal;

		QPointF tangent(-normal.y(), normal.x());
		double vt = QPointF::dotProduct(ball->velocity, tangent);
		ball->velocity -= 0.10 * vt * tangent;
	}

	checkBallBoundary(ball);
	return true;
}

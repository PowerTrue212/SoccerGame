#pragma once
#include "ball.h"
#include "player.h"
// 物理碰撞检测与响应的工具类。
class PhysicsCollision
{
public:
   // 检测足球与场地边界的碰撞并修正。
	static void checkBallBoundary(Ball* ball);
   // 检测球员与场地边界的碰撞并修正。
	static void checkPlayerBoundary(Player* player);
   // 处理两个球员之间的身体碰撞。
	static void checkPlayerCollision(Player* player1, Player* player2);
   // 检测球与球员头部碰撞并响应。
	static bool checkCollideHead(Player* player, Ball* ball);
   // 检测球与球员身体碰撞并响应。
	static bool checkCollideBody(Player* player, Ball* ball);
   // 检测足球与横梁碰撞并响应。
	static void checkGoalCrossbarCollision(Ball* ball);
};


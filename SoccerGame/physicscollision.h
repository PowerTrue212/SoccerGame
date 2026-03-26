#pragma once
#include "ball.h"
#include "player.h"
class PhysicsCollision
{
public:
	static void checkBallBoundary(Ball* ball);
	static void checkPlayerBoundary(Player* player);
	static void checkPlayerCollision(Player* player1, Player* player2);
	static bool checkCollideHead(Player* player, Ball* ball);
	static bool checkCollideBody(Player* player, Ball* ball);
};


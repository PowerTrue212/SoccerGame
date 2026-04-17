#pragma once

namespace Constants
{
	constexpr int WindowWidth = 1600;
	constexpr int WindowHeight = 900;

	constexpr int GroundLevel = 700;

	constexpr int PlayerRadius = 50;
	constexpr int PlayerHeight = 100; // the height from the ground to the center of the player's head
	constexpr int PlayerBodyHeight = 50;
	constexpr int PlayerBodyWidth = 40;
	constexpr int Playeroffset = 20; // 头中心和脚中心的水平距离

	constexpr int BallRadius = 25;

	constexpr int GoalWidth = 70;
	constexpr int GoalHeight = 240;
	constexpr int GoalBarThickness = 15;

	// physics tuning
	constexpr double Gravity = 0.70;

	constexpr double BallAirDrag = 0.999;
	constexpr double BallGroundFriction = 0.985;
	constexpr double BallRestitutionGround = 0.5;
	constexpr double BallRestitutionWall = 0.75;
	constexpr double BallSleepVy = 0.9;
	constexpr double BallMaxSpeed = 26.0;

	constexpr double PlayerGroundAccel = 1.2;
	constexpr double PlayerAirAccel = 0.45;
	constexpr double PlayerGroundDrag = 0.80;
	constexpr double PlayerAirDrag = 0.98;
	constexpr double PlayerMaxSpeedGround = 9.0;
	constexpr double PlayerMaxSpeedAir = 6.5;
	constexpr double JumpSpeed = 16.0;

	constexpr double KickMinStrength = 20.0;
	constexpr double KickMaxStrength = 60.0;

	constexpr double KickRangeForward = 100.0;
	constexpr double KickRangeBackward = 25.0;
	constexpr double KickRangeVertical = 100.0;

	constexpr double KickMinAngleDeg = 22.0;
	constexpr double KickMaxAngleDeg = 50.0;
	constexpr double KickLiftBias = 0.35;

}
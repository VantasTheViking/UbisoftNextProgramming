#pragma once
#include <vector>
#include <string>
#include <windows.h> 
#include "LevelManager.h"
using namespace std;

class PlayerManager
{
	struct PlayerBall {
		//Current Position
		float xPos, yPos;
		//Previous Position
		float pxPos, pyPos;
		//Current Velocity
		float xVel, yVel;
		//Current Acceleration
		float xAcc, yAcc;
		//Radius
		float radius;
		int playerID;
		float mass;
		float testTimeRemaining;

	};

public:
	PlayerManager(float setPlayerRadius, float setPlayerSegments, float setSwingStrength, float setFriction);
	void UpdateLineVec(LevelManager& levelManager);
	void DrawPlayers();
	void CreatePlayer(float sx, float sy, float m = 1);
	void Update(float deltaTime);
	void SimulatePhysics(float deltaTime);
	void CheckPlayerCollisions();
	void ApplyElasticCollision();
	bool CheckPointCollision(PlayerBall player, float px, float py);
	void OnClick();
	void WhileSelected();
	void ApplyPhysics(float deltaTime);
	string text;

private:
	vector<PlayerBall> playerBalls;
	vector<PlayerBall*> fakePlayers;
	vector<pair<PlayerBall*, PlayerBall*>> collidingPlayers;
	float playerRadius;
	int playerCount = 0;
	float playerSegments;
	PlayerBall* pSelectedPlayer;
	bool selected;
	float mouseXPos;
	float mouseYPos;
	float swingStrength;
	float friction;
	int simulatedPhysicsTests = 6;
	int maxSimulatedSteps = 15;
	vector<LevelManager::LineSegment> vecLines;
};


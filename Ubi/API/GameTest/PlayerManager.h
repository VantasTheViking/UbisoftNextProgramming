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
		//ID
		int playerID;
		//Ball Mass
		float mass;
		//Inside Hole
		float finished;
		//Score
		int stroke = 0;

		float testTimeRemaining;



	};

public:
	PlayerManager(float setPlayerRadius, float setPlayerSegments, float setSwingStrength, float setFriction);
	void UpdateLineVec(LevelManager& levelManager);
	void DrawPlayers();
	void CreatePlayer(float sx, float sy, float m = 1);
	void Update(float deltaTime);
	void CheckLevelCompleation();
	bool GetLevelFinished();
	void SimulatePhysics(float deltaTime);
	void CheckPlayerCollisions();
	void ApplyElasticCollision();
	void RespawnPlayer(int id);
	void ExilePlayer(int id);
	bool CheckPointCollision(PlayerBall player, float px, float py);
	void OnClick();
	void WhileSelected();
	void ApplyPhysics(float deltaTime);

	void SetRespawnPoints(vector<float> respawnCoords);

	//For Debugging
	string text;
	string text2;
	bool writeWinText;
	//float highestRecordedSpeed;
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
	
	float speedBoost = 1.005f;
	
	int simulatedPhysicsTests = 6;
	int maxSimulatedSteps = 15;
	vector<LevelManager::LineSegment> vecLines;



	float respawnXPos1;
	float respawnYPos1;
	float respawnXPos2;
	float respawnYPos2;

	int player1Stroke;
	int player2Stroke;
	
	bool levelFinished;
};


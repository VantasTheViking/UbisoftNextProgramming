#include "stdafx.h"
#include "PlayerManager.h"
#include <math.h>  
#include "app\app.h"
#include "Calculator.h"
#include <stdlib.h>

using namespace std;

PlayerManager::PlayerManager(float setPlayerRadius, float setPlayerSegments, float setSwingStrength, float setFriction) {

	playerRadius = setPlayerRadius;
	playerSegments = setPlayerSegments;
	swingStrength = setSwingStrength;
	friction = setFriction;
	/*
	* float sx, float sy, float sRadius, int id
	xPos = sx;
	yPos = sy;
	size = sSize;
	playerID = id;
	*/

}

void PlayerManager::UpdateLineVec(LevelManager &levelManager) {

	const std::vector<LevelManager::LineSegment>& lines = levelManager.GetLines();
	

	vecLines.clear();
	vecLines.insert(vecLines.end(), lines.begin(), lines.end());

}

void PlayerManager::CreatePlayer(float sx, float sy, float m) {
	PlayerBall playerBall;
	playerBall.xPos = sx;
	playerBall.yPos = sy;
	playerBall.xVel = 0;
	playerBall.yVel = 0;
	playerBall.xAcc = 0;
	playerBall.yAcc = 0;
	playerBall.mass = m;
	playerBall.radius = playerRadius;
	playerBall.playerID = playerCount;
	playerBall.testTimeRemaining = 0;
	playerCount++;


	playerBalls.emplace_back(playerBall);

}

void PlayerManager::Update(float deltaTime) {
	
	pair<float, float> mousePos = App::GetMousePos();

	mouseXPos = mousePos.first;
	mouseYPos = mousePos.second;

	OnClick();
	SimulatePhysics(deltaTime);
	DrawPlayers();

	//text = to_string(vecLines[0].sXPos);

}

void PlayerManager::SimulatePhysics(float deltaTime) {
	float timePerTest = deltaTime / simulatedPhysicsTests;

	for (int i = 0; i < 6; i++) {
		for (auto& p : playerBalls) {
			p.testTimeRemaining = timePerTest;
		}

		for (int j = 0; j < 20; j++) {
			ApplyPhysics(timePerTest);
			CheckPlayerCollisions();
			
		}

		
	}
}

void PlayerManager::DrawPlayers() {
	for each (PlayerBall player in playerBalls)
	{
		float increment = 2 * 3.14159 / playerSegments;

		for (int i = 0; i <= playerSegments; i++) {
			float start = i * increment;
			float end = (i + 1) * increment;
			float sX = player.xPos + player.radius * cos(start);

			float sY = player.yPos + player.radius * sin(start);
			float eX = player.xPos + player.radius * cos(end);

			float eY = player.yPos + player.radius * sin(end);

			App::DrawLine(sX, sY, eX, eY, 1, 1, 1);

		}
	}

	WhileSelected();
	
}

bool PlayerManager::CheckPointCollision(PlayerBall player, float px, float py) {
	

	//Distance between first targeted player, and second targeted player
	float distance = GetDistance(player.xPos, player.yPos, px, py);
	
	//IsColliding
	if (distance < player.radius) {
		//text = to_string(player.radius);
		return true;
	}
	else return false;
}

void PlayerManager::OnClick() {

	if (App::GetController().CheckButton(XINPUT_GAMEPAD_A, true))
	{
		if (!selected) {
			pSelectedPlayer = nullptr;
			

			for (auto& player : playerBalls) {
				if (CheckPointCollision(player, mouseXPos, mouseYPos)) {
					selected = true;
					text = "PP";
					pSelectedPlayer = &player;
					break;


				}
			}
		}
		
		else if (selected) {

			pSelectedPlayer->xVel = swingStrength * ((pSelectedPlayer->xPos) - mouseXPos);
			pSelectedPlayer->yVel = swingStrength * ((pSelectedPlayer->yPos) - mouseYPos);

			selected = false;
			pSelectedPlayer = nullptr;
			text = "";
		}
		

	}
}

void PlayerManager::WhileSelected() {
	if (pSelectedPlayer != nullptr) {
		
		//pSelectedPlayer->xPos = mouseXPos;
		//pSelectedPlayer->yPos = mouseYPos;
		
		//Show Player Swing
		App::DrawLine(pSelectedPlayer->xPos, pSelectedPlayer->yPos, mouseXPos, mouseYPos, 0, 1, 1);
		playerCount++;
		text = to_string(GetDistance(pSelectedPlayer->xPos, pSelectedPlayer->yPos, mouseXPos, mouseYPos));
	}
}

void PlayerManager::CheckPlayerCollisions() {

	collidingPlayers.clear();

	for (auto &p1 : playerBalls) {

		//Player To Edge
		for (auto& edge : vecLines) {
			float lineX1 = edge.eXPos - edge.sXPos;
			float lineY1 = edge.eYPos - edge.sYPos;

			float lineX2 = p1.xPos - edge.sXPos;
			float lineY2 = p1.yPos - edge.sYPos;

			float edgeLength = pow(lineX1, 2) + pow(lineY1, 2);

			float t = max(0, min(edgeLength, (lineX1 * lineX2 + lineY1 * lineY2))) / edgeLength;

			float closestPointX = edge.sXPos + t * lineX1;
			float closestPointY = edge.sYPos + t * lineY1;

			float distance = GetDistance(p1.xPos, p1.yPos, closestPointX, closestPointY);

			if (distance <= p1.radius) {

				PlayerBall* fakePlayer = new PlayerBall();
				fakePlayer->radius = 1;
				fakePlayer->mass = p1.mass * 1;
				fakePlayer->xPos = closestPointX;
				fakePlayer->yPos = closestPointY;
				fakePlayer->xVel = -p1.xVel;
				fakePlayer->yVel = -p1.yVel;

				fakePlayers.push_back(fakePlayer);
				collidingPlayers.push_back({ &p1, fakePlayer });

				float overlapDistance = 1.0f * (distance - p1.radius - fakePlayer->radius);

				p1.xPos -= overlapDistance * (p1.xPos - fakePlayer->xPos) / distance / 2;
				p1.yPos -= overlapDistance * (p1.yPos - fakePlayer->yPos) / distance / 2;



			}

			

		}


		//Player to Player
		for (auto &p2 : playerBalls) {

			if (p1.playerID != p2.playerID) {

				//Distance between first targeted player, and second targeted player
				float distance = GetDistance(p1.xPos, p1.yPos, p2.xPos, p2.yPos);

				//IsColliding
				if (distance < p1.radius + p2.radius) {

					//Add to Colliding Players
					collidingPlayers.push_back({ &p1, &p2 });


					//Force Players Away from each other
					float overlapDistance = (distance - p1.radius - p2.radius);

					p1.xPos -= overlapDistance * (p1.xPos - p2.xPos) / distance / 2;
					p1.yPos -= overlapDistance * (p1.yPos - p2.yPos) / distance / 2;
					
					p2.xPos += overlapDistance * (p1.xPos - p2.xPos) / distance / 2;
					p2.yPos += overlapDistance * (p1.yPos - p2.yPos) / distance / 2;


					

				}
			}
		}

		//Update testTimeRemaining
		float intendedSpeed = sqrtf(p1.xVel * p1.xVel + p1.yVel * p1.yVel);
		float intendedDistance = intendedSpeed * p1.testTimeRemaining;
		float distanceCovered = GetDistance(p1.pxPos, p1.pyPos, p1.xPos, p1.yPos);
		float actualTimeElapsed = distanceCovered / intendedSpeed;

		p1.testTimeRemaining = p1.testTimeRemaining - actualTimeElapsed;
	}



	ApplyElasticCollision();
}

void PlayerManager::ApplyElasticCollision() {
	
	for (auto playerPair : collidingPlayers) {

		PlayerBall* p1 = playerPair.first;
		PlayerBall* p2 = playerPair.second;

		float distance = GetDistance(p1->xPos, p1->yPos, p2->xPos, p2->yPos);

		float normX = (p2->xPos - p1->xPos) / distance;
		float normY = (p2->yPos - p1->yPos) / distance;

		float tanX = -normY;
		float tanY = normX;

		float dotTan1 = p1->xVel * tanX + p1->xVel * tanY;
		float dotTan2 = p2->xVel * tanX + p2->xVel * tanY;

		float dotNorm1 = p1 -> xVel * normX + p1->yVel * normY;
		float dotNorm2 = p2 -> xVel * normX + p2->yVel * normY;

		float momentum1 = (dotNorm1 * (p1->mass - p2->mass) + 2.0f * p2->mass * dotNorm2) / (p1->mass + p2->mass);
		float momentum2 = (dotNorm2 * (p2->mass - p1->mass) + 2.0f * p1->mass * dotNorm1) / (p1->mass + p2->mass);


		//Update Velocities
		p1->xVel = tanX * dotTan1 + normX * momentum1;
		p1->yVel = tanY * dotTan1 + normY * momentum1;

		p2->xVel = tanX * dotTan2 + normX * momentum2;
		p2->yVel = tanY * dotTan2 + normY * momentum2;

	}

	//Clear fake balls
	for (auto& p : fakePlayers) {
		delete p;
	}
	fakePlayers.clear();
	collidingPlayers.clear();
}

void PlayerManager::ApplyPhysics(float deltaTime) {
	for (auto& p : playerBalls) {
		if (p.testTimeRemaining > 0.0f) {

			//Store Position;
			p.pxPos = p.xPos;
			p.pyPos = p.yPos;

			//Update Acceleration
			p.xAcc = -p.xVel * friction;
			p.yAcc = -p.yVel * friction;

			//Update Velocity
			p.xVel += p.xAcc * p.testTimeRemaining;
			p.yVel += p.yAcc * p.testTimeRemaining;

			//Update Position
			p.xPos += p.xVel * p.testTimeRemaining;
			p.yPos += p.yVel * p.testTimeRemaining;

			//Cap Player Speed
			if (p.xVel > 50) {
				p.xVel = 50;
			}
			if (p.yVel > 50) {
				p.yVel = 50;
			}
			if (p.xVel < -50) {
				p.xVel = -50;
			}
			if (p.yVel < -50) {
				p.yVel = -50;
			}
			
			//Stop player when below speed;
			if (abs(p.xVel) < 0.001f) {
				p.xVel = 0;
			}
			if (abs(p.yVel) < 0.001f) {
				p.yVel = 0;
			}
		}
		
	}
}
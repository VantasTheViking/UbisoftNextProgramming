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
	CheckLevelCompleation();

	//text = to_string(vecLines[0].sXPos);

}

void PlayerManager::CheckLevelCompleation() {
	bool levelIncomplete = false;
	for (auto& p : playerBalls) {
		if (!p.finished) {
			levelIncomplete = true;
		}

	}

	if (!levelIncomplete) {
		levelFinished = true;
	}
}

bool PlayerManager::GetLevelFinished() {
	return levelFinished;
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

			if (player.playerID == 0) {

				App::DrawLine(sX, sY, eX, eY, 1, 0, 0);
			}
			else {

				App::DrawLine(sX, sY, eX, eY, 0, 0, 1);
			}

		}
	}
	for each (auto& player in fakePlayers)
	{
		float increment = 2 * 3.14159 / playerSegments;

		for (int i = 0; i <= playerSegments; i++) {
			float start = i * increment;
			float end = (i + 1) * increment;
			float sX = player->xPos + 10 * cos(start);

			float sY = player->yPos + 10 * sin(start);
			float eX = player->xPos + 10 * cos(end);

			float eY = player->yPos + 10 * sin(end);

			App::DrawLine(sX, sY, eX, eY, 1, 1, 1);

		}
	}

	//text = to_string(respawnXPos1);
	//text2 = to_string(respawnYPos1);
	if (writeWinText) {

		if (playerBalls[0].stroke < playerBalls[1].stroke) {

			text = "P1 WINS with Strokes: " + to_string(playerBalls[0].stroke);
			text2 = "P2 LOSES with Strokes: " + to_string(playerBalls[1].stroke);
		}else if (playerBalls[0].stroke > playerBalls[1].stroke) {

			text = "P1 LOSES with Strokes: " + to_string(playerBalls[0].stroke);
			text2 = "P2 WINS with Strokes: " + to_string(playerBalls[1].stroke);
		}
		else {

			text = "P1 TIES with Strokes: " + to_string(playerBalls[0].stroke);
			text2 = "P2 TIES with Strokes: " + to_string(playerBalls[1].stroke);

		}

	}
	else {

		text = "P1 Strokes: " + to_string(playerBalls[0].stroke);
		text2 = "P2 Strokes: " + to_string(playerBalls[1].stroke);

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
					//
					// text = "PP";
					pSelectedPlayer = &player;
					break;


				}
			}
		}
		
		else if (selected) {

			pSelectedPlayer->xVel = swingStrength * ((pSelectedPlayer->xPos) - mouseXPos);
			pSelectedPlayer->yVel = swingStrength * ((pSelectedPlayer->yPos) - mouseYPos);
			if (!writeWinText) {

				pSelectedPlayer->stroke++;
			}
			selected = false;
			pSelectedPlayer = nullptr;
			//text = "";
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
		//text = to_string(GetDistance(pSelectedPlayer->xPos, pSelectedPlayer->yPos, mouseXPos, mouseYPos));
	}
}

void PlayerManager::CheckPlayerCollisions() {

	collidingPlayers.clear();

	for (auto &p1 : playerBalls) {

		//Player To Edge
		for (auto& edge : vecLines) {
			
			//Find closest point to line
			float lineX1 = edge.eXPos - edge.sXPos;
			float lineY1 = edge.eYPos - edge.sYPos;

			float lineX2 = p1.xPos - edge.sXPos;
			float lineY2 = p1.yPos - edge.sYPos;
			
			

			
			float edgeLength = pow(lineX1, 2) + pow(lineY1, 2);
			
			//float edgeLength = GetDistance(edge.sXPos, edge.sYPos, edge.eXPos, edge.eYPos);

			float t = max(0, min(edgeLength, (lineX1 * lineX2 + lineY1 * lineY2))) / edgeLength;

			float closestPointX = edge.sXPos + t * lineX1;
			float closestPointY = edge.sYPos + t * lineY1;
			



			float distance = GetDistance(p1.xPos, p1.yPos, closestPointX, closestPointY);

			if (distance <= p1.radius) {
				

				if (edge.type == LevelManager::Obstacle) {
					//Hits Obstacle
					PlayerBall* fakePlayer = new PlayerBall();
					fakePlayer->radius = 1;
					fakePlayer->mass = p1.mass * 0.8f;
					fakePlayer->xPos = closestPointX;
					fakePlayer->yPos = closestPointY;
					fakePlayer->xVel = -p1.xVel;
					fakePlayer->yVel = -p1.yVel;


					fakePlayers.push_back(fakePlayer);
					collidingPlayers.push_back({ &p1, fakePlayer });

					float overlapDistance = distance - p1.radius;

					p1.xPos -= overlapDistance * (p1.xPos - fakePlayer->xPos) / distance;
					p1.yPos -= overlapDistance * (p1.yPos - fakePlayer->yPos) / distance;
				}
				//Goes inside pit
				else if (edge.type == LevelManager::Pit) {
					RespawnPlayer(p1.playerID);
				}
				//Triggers a speedboost edge
				else if (edge.type == LevelManager::Speedboost) {
					p1.xVel *= speedBoost;
					p1.yVel *= speedBoost;

				}
				//Triggers Goal Hole
				else if (edge.type == LevelManager::Hole) {
					//If player is slow enough
					if (sqrt(p1.xVel * p1.xVel + p1.yVel * p1.yVel) < 0.06f) {

						ExilePlayer(p1.playerID);
					}
				}
				


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
					float overlapDistance = 0.5f * (distance - p1.radius - p2.radius);

					p1.xPos -= overlapDistance * (p1.xPos - p2.xPos) / distance;
					p1.yPos -= overlapDistance * (p1.yPos - p2.yPos) / distance;
					
					p2.xPos += overlapDistance * (p1.xPos - p2.xPos) / distance;
					p2.yPos += overlapDistance * (p1.yPos - p2.yPos) / distance;


					

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

		float kx = (p1->xVel - p2->xVel);
		float ky = (p1->yVel - p2->yVel);
		float p = 2.0f * (normX * kx + normY * ky) / (p1->mass + p2->mass);

		p1->xVel = p1->xVel - p * p2->mass * normX;
		p1->yVel = p1->yVel - p * p2->mass * normY;
		p2->xVel = p2->xVel + p * p1->mass * normX;
		p2->yVel = p2->yVel + p * p1->mass * normY;

		

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

			/*
			if (sqrt(p.xVel * p.xVel + p.yVel * p.yVel) > highestRecordedSpeed) {
				highestRecordedSpeed = sqrt(p.xVel * p.xVel + p.yVel * p.yVel);
			}

			text = to_string(highestRecordedSpeed);
			*/

			//Cap Player Speed
			if (p.xVel > 5) {
				p.xVel = 5;
			}
			else if (p.xVel < -5) {
				p.xVel = -5;
			}
			if (p.yVel > 5) {
				p.yVel = 5;
			}
			else if (p.yVel < -5) {
				p.yVel = -5;
			}

			//Update Position
			p.xPos += p.xVel * p.testTimeRemaining;
			p.yPos += p.yVel * p.testTimeRemaining;

			
			
			
			
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

void PlayerManager::SetRespawnPoints(vector<float> respawnCoords) {
	respawnXPos1 = respawnCoords[0];
	respawnYPos1 = respawnCoords[1];
	respawnXPos2 = respawnCoords[2];
	respawnYPos2 = respawnCoords[3];
	levelFinished = false;
}

void PlayerManager::RespawnPlayer(int id) {
	for (auto &p : playerBalls) {
		if (p.playerID == id) {
			if (p.playerID == 0) {
				p.xPos = respawnXPos1;
				p.yPos = respawnYPos1;
			}
			else {
				p.xPos = respawnXPos2;
				p.yPos = respawnYPos2;
			}
			p.xVel = 0;
			p.yVel = 0;
			p.xAcc = 0;
			p.yAcc = 0;
			p.finished = false;
		}

	}
	
}

void PlayerManager::ExilePlayer(int id) {
	for (auto& p : playerBalls) {
		if (p.playerID == id) {
			if (p.playerID == 0) {
				p.xPos = 2000;
				p.yPos = 2000;
			}
			else {
				p.xPos = 3000;
				p.yPos = 3000;
			}
			p.xVel = 0;
			p.yVel = 0;
			p.xAcc = 0;
			p.yAcc = 0;

			p.finished = true;;
		}

	}

}
#include "stdafx.h"
#include "LevelManager.h"
#include <vector>
#include "app\app.h"
using namespace std;

LevelManager::LevelManager() {

}

void LevelManager::LinesUpdate() {

	DrawLines();

}

const vector<LevelManager::LineSegment>& LevelManager::GetLines() const {

	return vecLines;

}
void LevelManager::CreateLine(float sx, float sy, float ex, float ey, LineType type) {

	LineSegment newLine;
	newLine.sXPos = sx;
	newLine.sYPos = sy;
	newLine.eXPos = ex;
	newLine.eYPos = ey;
	newLine.type = type;
	vecLines.emplace_back(newLine);

}

void LevelManager::CreateBox(float sx, float sy, float ex, float ey, LineType type) {
	//Left Side
	CreateLine(sx, sy, sx, ey, type);
	//Bottom Side
	CreateLine(sx, sy, ex, sy, type);
	//Top Side
	CreateLine(sx, ey, ex, ey, type);
	//Right Side
	CreateLine(ex, sy, ex, ey, type);


}

void LevelManager::CreatePit(float sx, float sy, float ex, float ey) {
	CreateBox(sx, sy, ex, ey, PitOutline);
	CreateBox(sx + 5, sy + 5, ex - 5, ey - 5, Pit);
}

void LevelManager::DrawLines() {

	for each (LineSegment line in vecLines) {
		if (line.type == Obstacle) {

			App::DrawLine(line.sXPos, line.sYPos, line.eXPos, line.eYPos, 0.588f, 0.376f, 0);
		}
		if (line.type == PitOutline) {

			App::DrawLine(line.sXPos, line.sYPos, line.eXPos, line.eYPos, 1, 0, 0);
		}
		if (line.type == Pit) {

			App::DrawLine(line.sXPos, line.sYPos, line.eXPos, line.eYPos, 1, 0, 1);
		}
		if (line.type == Speedboost) {

			App::DrawLine(line.sXPos, line.sYPos, line.eXPos, line.eYPos, 0, 0, 1);
		}
		if (line.type == Hole) {

			App::DrawLine(line.sXPos, line.sYPos, line.eXPos, line.eYPos, 1, 1, 0);
		}


	}

}

void LevelManager::CreateLevel1() {
	ClearLevel();

	SetRespawnPoint(25, 50, 75, 50);
	//Border
	CreateBox(1, 1, 1023, 767, Obstacle);
	
	//Walls
	CreateLine(256, 1, 256, 667, Obstacle);
	CreateLine(512, 767, 512, 123, Obstacle);
	CreateLine(767, 1, 767, 667, Obstacle);

	//Corner
	CreateLine(1, 667, 101, 767, Obstacle);
	CreateLine(412, 767, 512, 667, Obstacle);
	CreateLine(256, 101, 356, 1, Obstacle);
	CreateLine(512, 667, 612, 767, Obstacle);
	CreateLine(668,1,768,101,Obstacle);

	//Finish Hole
	CreateBox(881, 123, 911, 153, Hole);
	
	//Speed boosts
	CreateBox(1, 123, 256, 153, Speedboost);
	CreateBox(512, 123, 767, 153, Speedboost);

}

void LevelManager::CreateLevel2() {
	ClearLevel();
	SetRespawnPoint(487, 384, 487, 334);
	CreateBox(1, 1, 1023, 767, Obstacle);

	//Pits
	CreatePit(123, 217, 223, 767);
	CreatePit(1, 1, 1023, 117);
	CreatePit(223, 667, 823, 767);
	CreatePit(823, 217, 1023, 767);
	CreatePit(323, 467, 623, 567);
	CreatePit(523, 217, 623, 467);
	CreatePit(223, 217, 523, 267);

	//Lines
	CreateLine(623, 117, 723, 177,Obstacle);
	CreateLine(723, 177,823,117,Obstacle);
	CreateLine(1, 117, 51, 167, Obstacle);
	CreateLine(51, 167, 1, 217, Obstacle);

	//Create Finish Hole
	CreateBox(46, 652, 76, 682, Hole);
}

void LevelManager::CreateWinLevel() {
	ClearLevel();
	SetRespawnPoint(25, 50, 75, 50);
	CreateBox(1, 1, 1023, 767, Obstacle);
}

void LevelManager::ClearLevel() {
	vecLines.clear();
}

vector<float> LevelManager::GetRespawnPoints() {
	return { levelRespawnPosX1,levelRespawnPosY1,levelRespawnPosX2,levelRespawnPosY2 };
}

void LevelManager::SetRespawnPoint(float x1, float y1, float x2, float y2) {
	//Player1
	levelRespawnPosX1 = x1;
	levelRespawnPosY1 = y1;
	//Player2
	levelRespawnPosX2 = x2;
	levelRespawnPosY2 = y2;
}

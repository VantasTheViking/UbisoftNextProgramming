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

void LevelManager::CreatePit(float sx, float sy, float ex, float ey, LineType type) {
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
	CreateLine(1, 1, 1023, 1, Obstacle);
	CreateLine(1, 1, 1, 767, Obstacle);
	CreateLine(1023, 1, 1023, 767, Obstacle);
	CreateLine(1, 767, 1023, 767, Obstacle);
	CreateLine(1, 667, 101, 767, Obstacle);

}

void LevelManager::ClearLevel() {
	vecLines.clear();
}

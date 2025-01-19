#include "stdafx.h"
#include "LevelManager.h"
#include <vector>
#include "app\app.h"
using namespace std;

LevelManager::LevelManager() {

}

void LevelManager::LinesUpdate(float r, float g, float b) {

	DrawLines(r, g, b);

}

const vector<LevelManager::LineSegment>& LevelManager::GetLines() const {

	return vecLines;

}
void LevelManager::CreateLine(float sx, float sy, float ex, float ey) {

	LineSegment newLine;
	newLine.sXPos = sx;
	newLine.sYPos = sy;
	newLine.eXPos = ex;
	newLine.eYPos = ey;
	vecLines.emplace_back(newLine);

}
void LevelManager::DrawLines(float r, float g, float b) {

	for each (LineSegment line in vecLines) {

		App::DrawLine(line.sXPos, line.sYPos, line.eXPos, line.eYPos, r, g, b);

	}

}
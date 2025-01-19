#pragma once
#include <vector>
#include "app\app.h"
using namespace std;
class LevelManager
{
	

public:
	enum LineType {
		Obstacle,
		PitOutline,
		Pit,
		Speedboost,
		Hole
	};
	struct LineSegment {
		float sXPos, sYPos;
		float eXPos, eYPos;
		LineType type;
	};

	LevelManager();
	void LinesUpdate();
	const vector<LineSegment> &GetLines() const;
	void CreateLine(float sx, float sy, float ex, float ey, LineType type);
	void CreateBox(float sx, float sy, float ex, float ey, LineType type);
	void CreatePit(float sx, float sy, float ex, float ey, LineType type);
	void DrawLines();

	void CreateLevel1();

	void ClearLevel();

	float levelRespawnPosX;
	float levelRespawnPosY;
private:
	vector<LineSegment> vecLines;
};


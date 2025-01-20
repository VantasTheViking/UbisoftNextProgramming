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
	void CreatePit(float sx, float sy, float ex, float ey);
	void DrawLines();

	void CreateLevel1();
	void CreateLevel2();
	void CreateWinLevel();
	void ClearLevel();

	vector<float> GetRespawnPoints();

	void SetRespawnPoint(float x1, float y1, float x2, float y2);

	//Player1
	float levelRespawnPosX1;
	float levelRespawnPosY1;
	//Player2
	float levelRespawnPosX2;
	float levelRespawnPosY2;
private:
	vector<LineSegment> vecLines;
};


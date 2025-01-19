#pragma once
#include <vector>
#include "app\app.h"
using namespace std;
class LevelManager
{
	

public:

	struct LineSegment {
		float sXPos, sYPos;
		float eXPos, eYPos;
	};

	LevelManager();
	void LinesUpdate(float r, float g, float b);
	const vector<LineSegment> &GetLines() const;
	void CreateLine(float sx, float sy, float ex, float ey);
	void DrawLines(float r, float g, float b);

private:
	vector<LineSegment> vecLines;
};


///////////////////////////////////////////////////////////////////////////////
// Filename: GameTest.cpp
// Provides a demo of how to use the API
///////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------
#include "stdafx.h"
//------------------------------------------------------------------------
#include <windows.h> 
#include <math.h>  
//------------------------------------------------------------------------
#include "app\app.h"
#include <PlayerManager.h>
#include <LevelManager.h>
#include <string>
#include "Calculator.h"
using namespace std;
//------------------------------------------------------------------------

//CSimpleSprite *testSprite;
//PlayerManager player1(100,100,20,1);
PlayerManager playerManager(20, 20, 0.01f, 0.001f);
LevelManager levelManager;
int currentLevel = 1;




//------------------------------------------------------------------------
// Called before first update. Do any initial setup here.
//------------------------------------------------------------------------
void Init()
{
	
	playerManager.CreatePlayer(0, 0, 1);
	playerManager.CreatePlayer(0, 0, 1);


	levelManager.CreateLevel1();
	playerManager.SetRespawnPoints(levelManager.GetRespawnPoints());
	playerManager.UpdateLineVec(levelManager);
	playerManager.RespawnPlayer(0);
	playerManager.RespawnPlayer(1);
}

//------------------------------------------------------------------------
// Update your simulation here. deltaTime is the elapsed time since the last update in ms.
// This will be called at no greater frequency than the value of APP_MAX_FRAME_RATE
//------------------------------------------------------------------------
void Update(const float deltaTime)
{
	playerManager.Update(deltaTime);
	
	if (playerManager.GetLevelFinished() && currentLevel == 1) {
		levelManager.CreateLevel2();
		playerManager.SetRespawnPoints(levelManager.GetRespawnPoints());
		playerManager.UpdateLineVec(levelManager);
		playerManager.RespawnPlayer(0);
		playerManager.RespawnPlayer(1);
		currentLevel = 2;
	}

	if (playerManager.GetLevelFinished() && currentLevel == 2) {
		levelManager.CreateWinLevel();
		playerManager.SetRespawnPoints(levelManager.GetRespawnPoints());
		playerManager.UpdateLineVec(levelManager);
		playerManager.RespawnPlayer(0);
		playerManager.RespawnPlayer(1);
		playerManager.writeWinText = true;
	}
	

}

//------------------------------------------------------------------------
// Add your display calls here (DrawLine,Print, DrawSprite.) 
// See App.h 
//------------------------------------------------------------------------
void Render()
{	
	

	App::Print(100, 737, playerManager.text.c_str(),1,0,0);
	App::Print(100, 707, playerManager.text2.c_str(), 0, 0, 1);
	levelManager.LinesUpdate();
	playerManager.DrawPlayers();

	

}


//------------------------------------------------------------------------
// Add your shutdown code here. Called when the APP_QUIT_KEY is pressed.
// Just before the app exits.
//------------------------------------------------------------------------
void Shutdown()
{	
	//------------------------------------------------------------------------
	// Example Sprite Code....
	//delete testSprite;
	//------------------------------------------------------------------------
}
#include "VMEngine2D/GameStates/PlayState.h"
#include "VMEngine2D/GameObjects/Characters/PlayerChar.h"
#include "VMEngine2D/Text.h"
#include "VMEngine2D/GameObjects/Characters/Enemy.h"
#include "VMEngine2D/Game.h"
#include "VMEngine2D/GameStates/GameOverState.h"
#include "VMEngine2D/GameObjects/Characters/Collectible.h"

PlayState::PlayState(SDL_Window* Window, SDL_Renderer* Renderer)
	: GameState(Window, Renderer)
{
	ScoreText = nullptr;
	GoalText = nullptr;
	SpawnTimer = 0.0;
	SpawnTime = 0.5;
	Player = nullptr;
}

void PlayState::BeginState()
{
	//this runs the parent function
	GameState::BeginState();
	Game::GetGameInstance().GameScore = 0;
	Player = new PlayerChar(Vector2(100.0f, 100.0f), StateRenderer);

	//Add the character into the game object stack
	SpawnGameObject(Player);

	/// CREATE SCORE TEXT ///

	//create an empty text object
	ScoreText = new Text(StateRenderer);

	//Adjust the text settings
	STTextInfo TestInfo;
	TestInfo.TextStr = "Hello World!";
	TestInfo.Size = 40;
	TestInfo.Position = Vector2(25.0f, 25.0f);

	//initialise the text with a font and settings
	ScoreText->InitText("Content/images/Fonts/Oswald-Medium.ttf", TestInfo);

	//add the text to the game state
	ActivateTextObject(ScoreText);

	/// CREATE LIVES TEXT ///

	LivesText = new Text(StateRenderer);

	//intialise a width (w) and height (h) int
	int w, h = 0;

	// get the window size and set the width and height
	SDL_GetWindowSize(StateWindow, &w, &h);

	//adjust the starting setting
	STTextInfo LivesInfo;
	LivesInfo.TextStr = "Lives: ##";
	LivesInfo.Size = 40;
	//set the y position of the text using the height of the window
	LivesInfo.Position = Vector2(25.0f, static_cast<float>(h) - 75.0f);

	//create the text using the settings and font
	LivesText->InitText("Content/images/Fonts/Oswald-Medium.ttf", LivesInfo);

	//Add the lives text to the gamestate
	ActivateTextObject(LivesText);

	/// CREATE GOAL TEXT ///

	//create an empty text object
	GoalText = new Text(StateRenderer);

	//Adjust the text settings
	STTextInfo GoalInfo;
	GoalInfo.TextStr = "Obj: Get 5000 points!";
	GoalInfo.Size = 40;
	GoalInfo.Position = Vector2(25.0f, 75.0f);

	//initialise the text with a font and settings
	GoalText->InitText("Content/images/Fonts/Oswald-Medium.ttf", GoalInfo);

	//add the text to the game state
	ActivateTextObject(GoalText);
}

void PlayState::ProcessInput(Input* PlayerInput)
{
	GameState::ProcessInput(PlayerInput);

}

void PlayState::Update(float DeltaTime)
{
	//Run the parent Update
	GameState::Update(DeltaTime);

	SpawnTimer += DeltaTime;

	//after 5 seconds spawn enemy then reset timer
	if (SpawnTimer > SpawnTime) {
		//set up variables to recieve the app window width and height
		int WinWidth, WinHeight = 0;

		//Use SDL function to set the dimensions
		SDL_GetWindowSize(StateWindow, &WinWidth, &WinHeight);

		//Increase Window Width by 1
		WinWidth += 1;
		WinWidth -= 128;

		//get a random number between 0 and the window width
		//rand() gets random number between 0 and number afteer %
		int SpawnEnemyX = rand() % WinWidth;

		// spawn an enemy based on a random screen x location
		Enemy* NewEnemy = new Enemy(Vector2(SpawnEnemyX, -128.0f), StateRenderer);

		//add the enemy to the game object stack
		ActivateGameObject(NewEnemy);

		//Reset Timer to 0 and start again
		SpawnTimer = 0.0;
		SpawnTime *= 0.99;

		//won't let spawn timer spawn faster than 1 second
		if (SpawnTime < 1.0f) {
			SpawnTime = 1.0f;
		}

	}
	
	//Set up Collectible Timer
	CollectTimer += DeltaTime;

	//after 5 seconds spawn enemy then reset timer
	if (CollectTimer > CollectTime) {
		//set up variables to recieve the app window width and height
		int WinWidth, WinHeight = 0;

		//Use SDL function to set the dimensions
		SDL_GetWindowSize(StateWindow, &WinWidth, &WinHeight);

		//Increase Window Width by 1
		WinWidth += 1;
		
		WinHeight -= 1;

		//get a random number between 0 and the window width
		//rand() gets random number between 0 and number afteer %
		int SpawnCollectX = rand() % WinWidth;
		int SpawnCollectY = rand() % WinHeight;

		// spawn an enemy based on a random screen x location
		Collectible* NewCollect = new Collectible(Vector2(SpawnCollectX, SpawnCollectY), StateRenderer);

		//add the enemy to the game object stack
		ActivateGameObject(NewCollect);

		//Reset Timer to 0 and start again
		CollectTimer = 0.0;
		CollectTime *= 0.99;

		//won't let spawn timer spawn faster than 3 seconds
		if (CollectTime < 2.0f) {
			CollectTime = 2.0f;
		}

	}

	//update the score each frame
	ScoreText->SetText("Score: " + to_string(Game::GetGameInstance().GameScore));

	//update the lives each frame
	LivesText->SetText("Lives: " + to_string(Player->GetLives()));

	//after score is obtained switch to game over
	if (Game::GetGameInstance().GameScore >= 5000 || Player->GetLives() == 0) {
		GameOverState* NewState = new GameOverState(StateWindow, StateRenderer);
		Game::GetGameInstance().GetGameStates()->SwitchState(NewState);
	}
}

void PlayState::Draw(SDL_Renderer* Renderer)
{
	GameState::Draw(Renderer);
}

void PlayState::EndState()
{
	GameState::EndState();

	ScoreText = nullptr;
	GoalText = nullptr;
}

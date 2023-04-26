#include "VMEngine2D/GameStates/PlayState.h"
#include "VMEngine2D/GameObjects/Characters/PlayerChar.h"
#include "VMEngine2D/Text.h"
#include "VMEngine2D/GameObjects/Characters/Enemy.h"
#include "VMEngine2D/Game.h"
#include "VMEngine2D/GameStates/GameOverState.h"
#include "VMEngine2D/GameObjects/Characters/Collectible.h"
#include "sdl2/SDL_mixer.h"

PlayState::PlayState(SDL_Window* Window, SDL_Renderer* Renderer)
	: GameState(Window, Renderer)
{
	ScoreText = nullptr;
	SpawnTimer = 0.0;
	SpawnTime = 0.5;
	SpawnTimerRare = 0.0;
	SpawnTimeRare = 5.0;

	Player = nullptr;

	//load a music file using mixer
	BGM = Mix_LoadMUS("Content/Audio/bg.wav");
}

void PlayState::BeginState()
{
	//this runs the parent function
	GameState::BeginState();
	Game::GetGameInstance().GameScore = 0;

	//Start Audio
	if (Mix_PlayMusic(BGM, -1) == -1) {
		std::cout << "Music failed to load" << std::endl;
	}

	//Change volume of music
	//Mix_VolumeMusic(25);

	Player = new PlayerChar(Vector2(100.0f, 100.0f), StateRenderer);

	// so debug will work
	Game::GetGameInstance().SetDebug(Player);

	//Add the character into the game object stack
	SpawnGameObject(Player);

	/// CREATE SCORE TEXT ///

	//create an empty text object
	ScoreText = new Text(StateRenderer);

	//Adjust the text settings
	STTextInfo TestInfo;
	TestInfo.TextStr = "Score: ##";
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

	// SPAWN BASIC ENEMY //
	
	//after 5 seconds spawn basic enemy then reset timer
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
		int SpawnEnemyX2 = rand() % WinWidth;

		// spawn an enemy based on a random screen x location
		Enemy* NewEnemy = new Enemy(EnemyAnims::BASE, Vector2(SpawnEnemyX, -128.0f), StateRenderer);
		Enemy* Enemy2 = new Enemy(EnemyAnims::BASE2, Vector2(SpawnEnemyX2, -128.0f), StateRenderer);

		//add the enemy to the game object stack
		ActivateGameObject(NewEnemy);
		ActivateGameObject(Enemy2);

		//Reset Timer to 0 and start again
		SpawnTimer = 0.0;
		SpawnTime *= 0.99;

		//ensure they don't spawn faster than 3 seconds
		if (SpawnTime < 1.0f) {
			SpawnTime = 1.0f;
		}

	}

	// SPAWN RARE ENEMY //

	SpawnTimerRare += DeltaTime;

	if (SpawnTimerRare > SpawnTimeRare) {

		//set up variables to recieve the app window width and height
		int WinWidth, WinHeight = 0;

		//Use SDL function to set the dimensions
		SDL_GetWindowSize(StateWindow, &WinWidth, &WinHeight);

		//Increase Window Width by 1
		WinWidth += 1;
		WinWidth -= 128;

		//get a random number between 0 and the window width
		//rand() gets random number between 0 and number afteer %
		int SpawnRareX = rand() % WinWidth;

		//spawn an enemy based on a random X screen location
		Enemy* Enemy3 = new Enemy(EnemyAnims::BASE3, Vector2(SpawnRareX, -128.0f), StateRenderer);

		//add the enemy to the game object stack
		ActivateGameObject(Enemy3);

		//Reset Timer to 0 and start again
		SpawnTimerRare = 0.0;

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
		CollectTime = 120.0;

		//won't let spawn timer spawn faster than 3 seconds
		if (CollectTime < 2.0f) {
			CollectTime = 120.0f;
		}

	}

	//update the score each frame
	ScoreText->SetText("Score: " + to_string(Game::GetGameInstance().GameScore));

	//update the lives each frame
	LivesText->SetText("Lives: " + to_string(Player->GetLives()));

	//after player runs out of lives, end game
	if (Player->GetLives() == 0) {
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
	LivesText = nullptr;

	//Stop and unload music
	if (BGM != nullptr) {
		Mix_HaltMusic();
		Mix_FreeMusic(BGM);
	}
}

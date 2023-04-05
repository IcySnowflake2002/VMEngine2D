#include "VMEngine2D/GameStates/GameOverState.h"
#include "VMEngine2D/GameStates/PlayState.h"
#include "VMEngine2D/Text.h"
#include "VMEngine2D/Game.h"
#include "VMEngine2D/Input.h"

GameOverState::GameOverState(SDL_Window* Window, SDL_Renderer* Renderer) 
	: GameState(Window, Renderer)
{
	ScoreText = nullptr;
	RestartText = nullptr;
	GOText = nullptr;
}

void GameOverState::BeginState()
{
	GameState::BeginState();

	/// CREATE SCORE TEXT ///

	//create an empty text object
	ScoreText = new Text(StateRenderer);

	//Adjust the text settings
	STTextInfo TestInfo;
	TestInfo.TextStr = "Final Score: " + to_string(Game::GetGameInstance().GameScore);
	TestInfo.Size = 45;
	TestInfo.Position = Vector2(25.0f, 25.0f);

	//initialise the text with a font and settings
	ScoreText->InitText("Content/images/Fonts/Oswald-Medium.ttf", TestInfo);

	//add the text to the game state
	ActivateTextObject(ScoreText);

	/// CREATE RESET TEXT ///

	RestartText = new Text(StateRenderer);
	TestInfo.Size = 24;
	TestInfo.Position = Vector2(25.0f, 150.0f);
	TestInfo.TextStr = "Press Space to Restart the Game";

	//initialise the text with a font and settings
	RestartText->InitText("Content/images/Fonts/Oswald-Medium.ttf", TestInfo);

	//add the text to the game state
	ActivateTextObject(RestartText);

}

void GameOverState::ProcessInput(Input* PlayerInput)
{
	GameState::ProcessInput(PlayerInput);

	if (PlayerInput->IsKeyDown(SDL_SCANCODE_SPACE)) {
		PlayState* NewState = new PlayState(StateWindow, StateRenderer);
		Game::GetGameInstance().GetGameStates()->SwitchState(NewState);
	}

}

void GameOverState::Update(float DeltaTime)
{
	GameState::Update(DeltaTime);
}

void GameOverState::Draw(SDL_Renderer* Renderer)
{
	GameState::Draw(Renderer);
}

void GameOverState::EndState()
{
	GameState::EndState();

	ScoreText = nullptr;
	RestartText = nullptr;
	GOText = nullptr;
}

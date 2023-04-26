#include "VMEngine2D/GameStates/GameOverState.h"
#include "VMEngine2D/GameStates/PlayState.h"
#include "VMEngine2D/Text.h"
#include "VMEngine2D/Game.h"
#include "VMEngine2D/Input.h"
#include "sdl2/SDL_mixer.h"

GameOverState::GameOverState(SDL_Window* Window, SDL_Renderer* Renderer) 
	: GameState(Window, Renderer)
{
	ScoreText = nullptr;
	RestartText = nullptr;
	GOText = nullptr;

	//load a music file using mixer
	EGM = Mix_LoadMUS("Content/Audio/endgame.wav");
}

void GameOverState::BeginState()
{
	GameState::BeginState();

	//Start Audio
	if (Mix_PlayMusic(EGM, -1) == -1) {
		std::cout << "Music failed to load" << std::endl;
	}

	/// CREATE GAME OVER TEXT ///
	GOText = new Text(StateRenderer);
	STTextInfo GOInfo;
	GOInfo.TextStr = "GAME OVER";
	GOInfo.Size = 60;
	GOInfo.Position = Vector2(350.0f, 25.0f);

	GOText->InitText("Content/images/Fonts/Oswald-Medium.ttf", GOInfo);

	ActivateTextObject(GOText);

	/// CREATE SCORE TEXT ///

	//create an empty text object
	ScoreText = new Text(StateRenderer);

	//Adjust the text settings
	STTextInfo TestInfo;
	TestInfo.TextStr = "Final Score: " + to_string(Game::GetGameInstance().GameScore);
	TestInfo.Size = 45;
	TestInfo.Position = Vector2(75.0f, 170.0f);

	//initialise the text with a font and settings
	ScoreText->InitText("Content/images/Fonts/Oswald-Medium.ttf", TestInfo);

	//add the text to the game state
	ActivateTextObject(ScoreText);

	/// CREATE RESET TEXT ///

	RestartText = new Text(StateRenderer);
	TestInfo.Size = 24;
	TestInfo.Position = Vector2(75.0f, 250.0f);
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

	//Stop and unload music
	if (EGM != nullptr) {
		Mix_HaltMusic();
		Mix_FreeMusic(EGM);
	}
}

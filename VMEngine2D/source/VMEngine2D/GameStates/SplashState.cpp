#include "VMEngine2D/GameStates/SplashState.h"
#include "VMEngine2D/GameStates/PlayState.h"
#include "VMEngine2D/Text.h"
#include "VMEngine2D/Game.h"
#include "VMEngine2D/Input.h"

SplashState::SplashState(SDL_Window* Window, SDL_Renderer* Renderer) :
	GameState(Window, Renderer)
{
	SplashText = nullptr;
	AuthorText = nullptr;
	StartText = nullptr;
}

void SplashState::BeginState()
{
	// CREATE SPLASH TEXT //

	SplashText = new Text(StateRenderer);

	//Text Settings
	STTextInfo SplashInfo;
	SplashInfo.TextStr = "MOTHERSHIP";
	SplashInfo.Size = 80;
	SplashInfo.Position = Vector2(290.0f, 50.0f);

	//intialise text
	SplashText->InitText("Content/images/Fonts/Oswald-Medium.ttf", SplashInfo);

	//add the text to the game state
	ActivateTextObject(SplashText);

	// CREATE AUTHOR TEXT //

	AuthorText = new Text(StateRenderer);

	STTextInfo AuthorInfo;
	AuthorInfo.TextStr = "By Dylan Miller, Student Number: 9092";
	AuthorInfo.Size = 40;
	AuthorInfo.Position = Vector2(190.0f, 200.0f);

	AuthorText->InitText("Content/images/Fonts/Oswald-Medium.ttf", AuthorInfo);
	ActivateTextObject(AuthorText);

	// CREATE START TEXT //

	StartText = new Text(StateRenderer);

	STTextInfo StartInfo;
	StartInfo.TextStr = "PRESS SPACE TO START";
	StartInfo.Size = 40;
	StartInfo.Position = Vector2(310.0f, 400.f);

	StartText->InitText("Content/images/Fonts/Oswald-Medium.ttf", StartInfo);
	ActivateTextObject(StartText);
}

void SplashState::ProcessInput(Input* PlayerInput)
{
	GameState::ProcessInput(PlayerInput);

	if (PlayerInput->IsKeyDown(SDL_SCANCODE_SPACE)) {
		PlayState* NewState = new PlayState(StateWindow, StateRenderer);
		Game::GetGameInstance().GetGameStates()->SwitchState(NewState);
	}
}

void SplashState::Update(float DeltaTime)
{
	GameState::Update(DeltaTime);
}

void SplashState::Draw(SDL_Renderer* Renderer)
{
	GameState::Draw(Renderer);
}

void SplashState::EndState()
{
	SplashText = nullptr;
	AuthorText = nullptr;
	StartText = nullptr;
}

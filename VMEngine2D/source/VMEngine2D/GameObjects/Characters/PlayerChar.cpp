#include "VMEngine2D/GameObjects/Characters/PlayerChar.h"
#include "VMEngine2D/Input.h"
#include "VMEngine2D/AnimStateMachine.h"
#include "VMEngine2D/Game.h"

PlayerChar::PlayerChar(Vector2 StartPosition, SDL_Renderer* Renderer)
	: Character(StartPosition)
{
	BoostersIndex = PlayerAnims::BOO_IDLE;
	Scale = 3.0f;

	STAnimationData AnimData = STAnimationData();
	AnimData.FPS = 0;

	// Add ship texture into AnimState - 0
	AddAnimation(Renderer,
		"Content/Main Ship/Bases/base.png",
		AnimData);

	// Add the engine to the ship into Animstate - 1
	AddAnimation(Renderer,
		"Content/Main Ship/Engine/sc_eng.png",
		AnimData);

	//Update AnimData to handle boosters idle animation
	//Most sprite animations use FPS 12 but FPS 24 provides a smoother look
	AnimData.FPS = 24;
	AnimData.MaxFrames = 4;
	AnimData.EndFrame = 3;

	//Add the boosters idle animation to AnimState - 2
	AddAnimation(Renderer,
		"Content/Main Ship/Enginefx/sc_idle.png",
		AnimData);

	//Add the boosters powered animation to AnimState - 3
	AddAnimation(Renderer,
		"Content/Main Ship/Enginefx/sc_power.png",
		AnimData);

}

	

PlayerChar::~PlayerChar()
{

}

void PlayerChar::ProcessInput(Input* PlayerInput)
{
	// set the AnimIndex to play the first animation by default
	BoostersIndex = PlayerAnims::BOO_IDLE;
	//set the player to idle movement by default
	InputDir = Vector2::Zero(); //Essentially is like saying Vector2(0.0f, 0.0f)

	// update the input direction based on the inputs being pressed
	if (PlayerInput->IsKeyDown(SDL_SCANCODE_UP)) {
		//set input y to up
		InputDir.y = -2.0f;
	}

	if (PlayerInput->IsKeyDown(SDL_SCANCODE_DOWN)) {
		//set input y to down
		InputDir.y = 2.0f;
	}

	if (PlayerInput->IsKeyDown(SDL_SCANCODE_LEFT)) {
		//set input x to left
		InputDir.x = -2.0f;
	}

	if (PlayerInput->IsKeyDown(SDL_SCANCODE_RIGHT)) {
		//set input x to right
		InputDir.x = 2.0f;
	}

	//if we are moving
	if (InputDir.Length() > 0.0f) {
		BoostersIndex = PlayerAnims::BOO_POWER;
	}

}

void PlayerChar::Update()
{
	//set the direction based on input and move speed
	Vector2 Direction = InputDir.Normalised() * MaxMoveSpeed;
	//Move the player based on time
	Position += Direction * Game::GetGameInstance().GetFDeltaTime();
}

void PlayerChar::Draw(SDL_Renderer* Renderer)
{

	//Draw the engine to the screen
	CharacterAnimations->Draw(Renderer, PlayerAnims::ENG_SC, Position, Rotation, Scale, bFlipped);

	//Draw and play the relevant booster animation
	CharacterAnimations->Draw(Renderer, BoostersIndex, Position, Rotation, Scale, bFlipped);

	// Make sure the character method still runs
	Character::Draw(Renderer);
}

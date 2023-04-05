#include "VMEngine2D/GameObjects/Characters/PlayerChar.h"
#include "VMEngine2D/Input.h"
#include "VMEngine2D/GameObjects/Components/Physics.h"
#include "VMEngine2D/GameObjects/Components/Collision.h"
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
	MovementDir = Vector2::Zero(); //Essentially is like saying Vector2(0.0f, 0.0f)
	//default idle rotation at up
	Rotation = 0.0;

	// update the input direction based on the inputs being pressed
	if (PlayerInput->IsKeyDown(SDL_SCANCODE_UP)) {
		//set input y to up
		MovementDir.y = -5.0f;
		
	}

	if (PlayerInput->IsKeyDown(SDL_SCANCODE_DOWN)) {
		//set input y to down
		MovementDir.y = 5.0f;
		//make player look down
		Rotation = -180.0;
	}

	if (PlayerInput->IsKeyDown(SDL_SCANCODE_LEFT)) {
		//set input x to left
		MovementDir.x = -5.0f;
		// make player look left
		Rotation = -90.0;
	}

	if (PlayerInput->IsKeyDown(SDL_SCANCODE_RIGHT)) {
		//set input x to right
		MovementDir.x = 5.0f;
		// make player look right
		Rotation = 90.0;
	}

	//if we are moving
	if (MovementDir.Length() > 0.0f) {
		BoostersIndex = PlayerAnims::BOO_POWER;
	}
}

void PlayerChar::Update()
{
	//Run the parent class update first
	Character::Update();

	CharPhysics->AddForce(MovementDir, 900.0f);

	if (CharCollision->IsOverlappingTag("Enemy")) {
		bOverlapDetected = true;

		//getting all overlapped enemies and destroy them
		for (Collision* Enemy : CharCollision->GetOverlappedByTag("Enemy")) {
			//if enemy is not being destroyed, destroy them
			if (!Enemy->GetOwner()->ShouldDestroy()) {
				Enemy->GetOwner()->DestroyGameObject();
				//add to score
				Game::GetGameInstance().GameScore += 100;
			}
		}
	}

	if (CharCollision->IsOverlappingTag("Rocket")) {
		bOverlapDetected = true;

		//getting all overlapped collectibles and destroying them
		for (Collision* Collectible : CharCollision->GetOverlappedByTag("Rocket")) {
			if (!Collectible->GetOwner()->ShouldDestroy()) {
				Collectible->GetOwner()->DestroyGameObject();
				Game::GetGameInstance().GameScore += 50;

			}
		}
	}
	else {
		bOverlapDetected = false;
	}
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

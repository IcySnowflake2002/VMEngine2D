#include "VMEngine2D/GameObjects/Characters/PlayerChar.h"
#include "VMEngine2D/Input.h"
#include "VMEngine2D/GameObjects/Components/Physics.h"
#include "VMEngine2D/GameObjects/Components/Collision.h"
#include "VMEngine2D/AnimStateMachine.h"
#include "VMEngine2D/Game.h"
#include "VMEngine2D/GameObjects/Projectile.h"
#include "VMEngine2D/GameState.h"

PlayerChar::PlayerChar(Vector2 StartPosition, SDL_Renderer* Renderer)
	: Character(StartPosition)
{
	BoostersIndex = PlayerAnims::BOO_IDLE;
	DmgIndex = PlayerAnims::BASE_FULL;
	ShdIndex = PlayerAnims::EMPTY;
	Scale = 2.0f;

	STAnimationData AnimData = STAnimationData();
	AnimData.FPS = 0;

	Lives = 4;

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
	
	//Update AnimData to handle the Shield animation
	AnimData.FPS = 24;
	AnimData.MaxFrames = 10;
	AnimData.EndFrame = 9;

	//Add the Shield animation to AnimState - 4
	AddAnimation(Renderer,
		"Content/Main Ship/Shields/inv_shd.png",
		AnimData);


	//Adding in the damaged variants
	AnimData.FPS = 0;
	AnimData.MaxFrames = 1;
	AnimData.EndFrame = 0;

	//Add the small damage sprite to AnimState - 5
	AddAnimation(Renderer,
		"Content/Main Ship/Bases/base_smldmg.png",
		AnimData);

	//Add the medium damage sprite to AnimState - 6
	AddAnimation(Renderer,
		"Content/Main Ship/Bases/base_meddmg.png",
		AnimData);

	//Add the large damage sprite to AnimState - 7
	AddAnimation(Renderer,
		"Content/Main Ship/Bases/base_lrgdmg.png",
		AnimData);

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

	//how long the player has to wait in-between shots
	static float FireTimer = 0.25f;
	FireTimer += Game::GetGameInstance().GetFDeltaTime();

	//Fire Projectile
	if (PlayerInput->IsKeyDown(SDL_SCANCODE_Z) && FireTimer >= 0.25f) {
		Projectile* P = new Projectile();

		//Setting up neccessary information
		P->Position = Position;
		P->Position.x += 20.0f;
		P->Position.y += 20.0f;
		P->Acceleration = 1000.0f;
		P->Direction = Vector2(0.0f, -1.0f);
		P->ProjIndex = ProjAnims::PlayerProj;
		P->TargetTag = "Enemy";

		//Spawning Projectile
		Game::GetGameInstance().GetGameStates()->GetCurrentState()->SpawnGameObject(P);
		std::cout << "Spawned Projectile" << std::endl;

		//Reset Firing Timer
		FireTimer = 0.0f;
	}
}

void PlayerChar::Update()
{
	//Run the parent class update first
	Character::Update();

	CharPhysics->AddForce(MovementDir, 900.0f);

	// Enemy & Damage Changes
	// Enemy Collision
	if (CharCollision->IsOverlappingTag("Enemy")) {
		bOverlapDetected = true;

		//getting all overlapped enemies and destroy them
		for (Collision* Enemy : CharCollision->GetOverlappedByTag("Enemy")) {
			//if enemy is not being destroyed, destroy them
			if (!Enemy->GetOwner()->ShouldDestroy()) {
				dynamic_cast<Character*>(Enemy->GetOwner())->RemoveLives(1);
				//Remove life from player
				RemoveLives(1);
				//Check if player has shield and remove it
				if (ShdIndex = PlayerAnims::SHIELD) {
					ShdIndex = PlayerAnims::EMPTY;
				}
			}
		}
	}

	//Damage Anims
	if (Lives == 4) {
		DmgIndex = PlayerAnims::BASE_FULL;
	}

	if (Lives == 3) {
		DmgIndex = PlayerAnims::BASE_SMLDMG;
	}

	if (Lives == 2) {
		DmgIndex = PlayerAnims::BASE_MEDDMG;
	}

	if (Lives == 1) {
		DmgIndex = PlayerAnims::BASE_LRGDMG;
	}

	//Add Shield
	if (CharCollision->IsOverlappingTag("Shield")) {
		bOverlapDetected = true;

		//getting all overlapped collectibles and destroying them
		for (Collision* Collectible : CharCollision->GetOverlappedByTag("Shield")) {
			if (!Collectible->GetOwner()->ShouldDestroy()) {
				dynamic_cast<Character*>(Collectible->GetOwner())->RemoveLives(1);
				//Add Shield to Player so long as it is not max lives
				if (Lives != MaxLives) {
					//Check if Shield has already been applied
					if (ShdIndex = PlayerAnims::EMPTY) {
						ShdIndex = PlayerAnims::SHIELD;
					}
					AddLives(1);
				}
				
			}
		}
	}

	//Enemy Projectile Collision
	if (CharCollision->IsOverlappingTag("Player")) {
		bOverlapDetected = true;

		//getting all overlapped enemy projectiles and destroying them
		for (Collision* Projectile : CharCollision->GetOverlappedByTag("Player")) {
			if (!Projectile->GetOwner()->ShouldDestroy()) {
				dynamic_cast<Character*>(Projectile->GetOwner())->RemoveLives(1);
				//Remove life from player
				RemoveLives(1);
				//Check if player has shield and remove it
				if (ShdIndex = PlayerAnims::SHIELD) {
					ShdIndex = PlayerAnims::EMPTY;
				}
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

	//draw the ship to the screen
	CharacterAnimations->Draw(Renderer, DmgIndex, Position, Rotation, Scale, bFlipped);

	//Offset the shield so it lines up with the player
	Vector2 Shd_Offset = Vector2(-15.0f, -15.0f);

	//Draw and play the relevant shield animation
	CharacterAnimations->Draw(Renderer, ShdIndex, Position + Shd_Offset, Rotation, Scale, bFlipped);

	//Draw and play the relevant booster animation
	CharacterAnimations->Draw(Renderer, BoostersIndex, Position, Rotation, Scale, bFlipped);

	// Make sure the character method still runs
	//Character::Draw(Renderer);
}

void PlayerChar::ActivateShield()
{
	//For DEBUG menu //
	if (ShdIndex = PlayerAnims::EMPTY) {
		ShdIndex = PlayerAnims::SHIELD;
	}
}

void PlayerChar::SetMaxLives()
{
	// FOR DEBUG MENU //
	Lives = 5;
}

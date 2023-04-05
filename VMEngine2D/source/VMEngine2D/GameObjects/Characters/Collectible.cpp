#include "VMEngine2D/GameObjects/Characters/Collectible.h"
#include "VMEngine2D/GameObjects/Components/Physics.h"
#include "VMEngine2D/AnimStateMachine.h"
#include "VMEngine2D/Game.h"

Collectible::Collectible(Vector2 StartPosition, SDL_Renderer* Renderer)
	: Character(StartPosition)
{
	Scale = 2.0f;
	MovementDir = Vector2(0.0f, 1.0f);
	Rotation = 180.0;
	CharPhysics->MaxVelocity = 300.0f;
	Tag = "Rocket";

	STAnimationData AnimData = STAnimationData();
	AnimData.FPS = 24;
	AnimData.MaxFrames = 15;
	AnimData.StartFrame = 0;
	AnimData.EndFrame = 14;

	//Add the collectible to the animstate
	AddAnimation(Renderer, 
		"Content/Pickups/pui-rocket.png", 
		AnimData);
	
}

Collectible::~Collectible()
{
}

void Collectible::Update()
{
	//Run Parent class update first
	Character::Update();
}

void Collectible::Draw(SDL_Renderer* Renderer)
{
	//draw the collectible
	CharacterAnimations->Draw(Renderer, CollectAnims::ROCKET, Position, Rotation, Scale, bFlipped);
}

#include "VMEngine2D/GameObjects/Characters/Enemy.h"
#include "VMEngine2D/GameObjects/Components/Physics.h"
#include "VMEngine2D/AnimStateMachine.h"
#include "VMEngine2D/Game.h"

Enemy::Enemy(Vector2 StartPosition, SDL_Renderer* Renderer)
	: Character(StartPosition)
{
	Scale = 3.0f;
	MovementDir = Vector2(0.0f, 1.0f);
	Rotation = 180.0;
	Physics->MaxVelocity = 300.0f;

	STAnimationData AnimData = STAnimationData();
	AnimData.FPS = 0;

	//Add the ship base as the main sprite to AnimState - 0
	AddAnimation(Renderer,
		"Content/Enemy/Ships/NS_bmb_ship.png",
		AnimData);

	//set the anim data for the booster animation
	AnimData.FPS = 24;
	AnimData.MaxFrames = 8;
	AnimData.EndFrame = 7;

	//Add the booster animation to AnimState - 1
	AddAnimation(Renderer,
		"Content/Enemy/Enginefx/NS_bmb_fx.png", 
		AnimData);

}

Enemy::~Enemy()
{
}

void Enemy::Update()
{
	//Run the parent class update first
	Character::Update();

	Physics->AddForce(MovementDir, 300.0f);

	//Teleport the enemy back up if they reach the bottom of the screen
	//if (Position.y > 540.0f) {
	//	Position.y = -64.0f * Scale;
	//}
}

void Enemy::Draw(SDL_Renderer* Renderer)
{
	//draw the enemy
	Character::Draw(Renderer);

	//draw the boosters
	CharacterAnimations->Draw(Renderer, EnemyAnims::BOOSTERS, Position, Rotation, Scale, bFlipped);
}

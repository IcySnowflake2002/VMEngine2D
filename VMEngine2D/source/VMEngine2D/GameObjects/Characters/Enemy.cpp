#include "VMEngine2D/GameObjects/Characters/Enemy.h"
#include "VMEngine2D/AnimStateMachine.h"

Enemy::Enemy(Vector2 StartPosition, SDL_Renderer* Renderer)
	: Character(StartPosition)
{
	Scale = 3.0f;

	STAnimationData AnimData = STAnimationData();
	AnimData.FPS = 0;

	//Add the ship base as the main sprite to AnimState - 0
	AddAnimation(Renderer,
		"Content/Enemy/Ships/NS_bmb.png",
		AnimData);

	//set the anim data for the booster animation
	//AnimData.FPS = 
	//AnimData.MaxFrames =
	//AnimData.EndFrame = 

	//

}

Enemy::~Enemy()
{
}

void Enemy::Update()
{
}

void Enemy::Draw(SDL_Renderer* Renderer)
{
	//draw the enemy
	Character::Draw(Renderer);

	//draw the boosters
	CharacterAnimations->Draw(Renderer, EnemyAnims::BOOSTERS, Position, Scale, bFlipped);
}

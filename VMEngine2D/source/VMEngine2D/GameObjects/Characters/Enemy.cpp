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
	CharPhysics->MaxVelocity = 300.0f;
	Tag = "Enemy";

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

	CharPhysics->AddForce(MovementDir, 300.0f);

	//check if lives are 0
	if (GetLives() == 0) {
		//add to score
		Game::GetGameInstance().GameScore += 100;
		//destroy self if 0
		this->DestroyGameObject();
	}
}

void Enemy::Draw(SDL_Renderer* Renderer)
{
	//draw the enemy
	Character::Draw(Renderer);

	//draw the boosters
	CharacterAnimations->Draw(Renderer, EnemyAnims::BOOSTERS, Position, Rotation, Scale, bFlipped);
}

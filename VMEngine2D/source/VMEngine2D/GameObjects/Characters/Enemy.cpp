#include "VMEngine2D/GameObjects/Characters/Enemy.h"
#include "VMEngine2D/GameObjects/Components/Physics.h"
#include "VMEngine2D/GameObjects/Projectile.h"
#include "VMEngine2D/AnimStateMachine.h"
#include "VMEngine2D/Game.h"
#include "VMEngine2D/GameState.h"

Enemy::Enemy(EnemyAnims EnemyType, Vector2 StartPosition, SDL_Renderer* Renderer)
	: Character(StartPosition)
{
	Scale = 2.0f;
	MovementDir = Vector2(0.0f, 1.0f);
	Rotation = 180.0;
	CharPhysics->MaxVelocity = 300.0f;
	Tag = "Enemy";
	this->EnemyType = EnemyType;
	this->EnemyDestroy = EnemyDestroy;

	STAnimationData AnimData = STAnimationData();
	AnimData.FPS = 0;

	//Add the ship base as the main sprite to AnimState - 0
	AddAnimation(Renderer,
		"Content/Enemy/Ships/NS_bmb_ship.png",
		AnimData);

	//Add a second enemy into AnimState - 1
	AddAnimation(Renderer,
		"Content/Enemy/Ships/ns_fight_ship.png",
		AnimData);

	//Add a third enemy into AnimState - 2
	AddAnimation(Renderer,
		"Content/Enemy/Ships/ns_sct_ship.png",
		AnimData);

	//set the anim data for the booster animation
	AnimData.FPS = 24;
	AnimData.MaxFrames = 8;
	AnimData.EndFrame = 7;

	//Add the booster animation to AnimState - 3
	AddAnimation(Renderer,
		"Content/Enemy/Enginefx/NS_bmb_fx.png", 
		AnimData);

	//set the animdata for the base1 destruction
	AnimData.FPS = 24;
	AnimData.MaxFrames = 9;
	AnimData.EndFrame = 8;

	//Add the Destruction animation to AnimState - 4
	AddAnimation(Renderer,
		"Content/Enemy/Destruction/NS_bmb.png",
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
		//Check what enemy it is
		if (EnemyType == EnemyAnims::BASE) {
			//add to score
			Game::GetGameInstance().GameScore += 100;
			EnemyType == EnemyAnims::BASE_DESTRUCTION;
			SDL_Delay(40);
		}
		else if (EnemyType == EnemyAnims::BASE2) {
			//add to score
			Game::GetGameInstance().GameScore += 200;
		}
		else if (EnemyType == EnemyAnims::BASE3) {
			//add to score
			Game::GetGameInstance().GameScore += 500;
		}
		//destroy self if 0
		this->DestroyGameObject();
	}

	//Fire a projectile
	if (EnemyType == EnemyAnims::BASE2) {
		//how long the enemy has to wait in-between shots
		static float FireTimer = 3.0f;
		FireTimer += Game::GetGameInstance().GetFDeltaTime();
		
		if (FireTimer > 3.0f) {
			//Fire Projectile
			Projectile* E = new Projectile();

			//Setting up neccessary information
			E->Position = Position;
			E->Position.x += 64.0f;
			E->Position.y += 64.0f;
			E->Rotation = 180.0f;
			E->Acceleration = 1000.0f;
			E->Direction = Vector2(0.0f, 1.0f);
			E->ProjIndex = ProjAnims::EnemyProj;
			E->TargetTag = "Player";

			//Spawning Projectile
			Game::GetGameInstance().GetGameStates()->GetCurrentState()->SpawnGameObject(E);
			//Reset Firing Timer
			FireTimer = 0.0f;
		}
	}
}

void Enemy::Draw(SDL_Renderer* Renderer)
{
	//draw the enemy
	CharacterAnimations->Draw(Renderer, EnemyType, Position, Rotation, Scale, bFlipped);

	//draw the boosters
	CharacterAnimations->Draw(Renderer, EnemyAnims::BOOSTERS, Position, Rotation, Scale, bFlipped);


}

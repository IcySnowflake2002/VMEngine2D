#include "VMEngine2D/GameObjects/Projectile.h"
#include "VMEngine2D/AnimStateMachine.h"
#include "VMEngine2D/GameObjects/Components/Collision.h"
#include "VMEngine2D/GameObjects/Components/Physics.h"
#include "VMEngine2D/Game.h"
#include "VMEngine2D/GameState.h"
#include "VMEngine2D/GameObjects/Character.h"

Projectile::Projectile()
{
	DeathTimer = 10.0f;
	Acceleration = 1000.0f;
	Direction = Vector2::Zero();
	Rotation = 0.0;
	Scale = 3.0f;
	TargetTag = " ";
	ProjIndex = EmptyProj;

	Animations = new AnimStateMachine();
	
	Proj_Col = new Collision(this);
	Proj_Col->Dimensions.Position = Position;
	Proj_Col->Dimensions.Width = 25.0f;
	Proj_Col->Dimensions.Height = 25.0f;

	Proj_Phy = new Physics(this);
	Proj_Phy->MaxVelocity = 1000.0f;
	Proj_Phy->Drag = 1.0f;

	STAnimationData AnimData;
	AnimData.FPS = 24;
	AnimData.MaxFrames = 3;
	AnimData.StartFrame = 0;
	AnimData.EndFrame = 2;

	SDL_Renderer* R = Game::GetGameInstance().GetGameStates()->GetCurrentState()->GetRenderer();

	//Load Player Projectile
	Animations->AddAnimation(R, "Content/Main Ship/Projectiles/rocket.png", AnimData);

	//Enemy Projectile
	AnimData.FPS = 24;
	AnimData.MaxFrames = 8;
	AnimData.EndFrame = 7;

	Animations->AddAnimation(R, "Content/Enemy/Weaponfx/NS_bul.png", AnimData);

}

Projectile::~Projectile()
{
	//delete the animations
	delete Animations;

	Animations = nullptr;
}

void Projectile::Update()
{
	GameObject::Update();

	//move the projectile constant in a direction
	Proj_Phy->AddForce(Direction, Acceleration);

	//check if we're overlapping a collider with the target tag
	if (Proj_Col->IsOverlappingTag(TargetTag)) {
		//loop through all targets
		for (Collision* Target : Proj_Col->GetOverlappedByTag(TargetTag)) {
			//remove 1 life from characters and destroy self
			if (!Target->GetOwner()->ShouldDestroy()) {
				dynamic_cast<Character*>(Target->GetOwner())->RemoveLives(1);
				this->DestroyGameObject();
			}
		}
	}
}

void Projectile::Draw(SDL_Renderer* Renderer)
{
	GameObject::Draw(Renderer);

	Animations->Draw(Renderer, ProjIndex, Position, Rotation, Scale, false);

}

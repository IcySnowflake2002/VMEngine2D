#pragma once
#include "VMEngine2D/GameObject.h"

class AnimStateMachine;
class Physics;
class Collision;

enum ProjAnims : unsigned int {
	PlayerProj = 0,
	EnemyProj = 1,
	EmptyProj = 2
};

class Projectile : 
	public GameObject {

public:
	Projectile();
	virtual ~Projectile();

	virtual void Update() override;
	virtual void Draw(SDL_Renderer* Renderer) override;

	//time until the death of the projectile
	float DeathTimer;

	//How fast the projectile achieves its max speed
	float Acceleration;
	//The direction of the projectile
	Vector2 Direction;

	//The rotation of the projectile image
	double Rotation;
	//the size of the projectile image
	float Scale;
	//any character with this tag will take damage
	const char* TargetTag;

	//Store the different projectiles
	unsigned int ProjIndex;

	//Where the projectile is fired from
	Vector2 ProjOffset;

protected:
	//store the texture/animation for the projectile
	AnimStateMachine* Animations;

	//store the collision of the projectile
	Collision* Proj_Col;
	
	//driver of the position of the projectile
	Physics* Proj_Phy;
	

};
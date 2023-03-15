#include "VMEngine2D/GameObjects/Components/Physics.h"
#include "VMEngine2D/GameObject.h"
#include "VMEngine2D/Game.h"
#include <iostream>

PhysicsComponent::PhysicsComponent(GameObject* OwnerToAttatch) : Components(OwnerToAttatch)
{
	MaxVelocity = 150.0f;
	Drag = 1.0f;
}

void PhysicsComponent::Update()
{
	//increase velocity by the current acceleration
	Velocity += Acceleration * Game::GetGameInstance().GetFDeltaTime();

	if (Velocity.Length() > MaxVelocity) {
		//setting velocity to itself normalised which will return -1 and 1 in the x and y
		//Multiply the normalised values by the max velocity to set the x and y to max
		Velocity = Velocity.Normalised() * MaxVelocity;
	}

	//the min() will choose the smaller number
	//if drag * DeltaTime is greater than 1 for some reason, min will correct the number and cap it at 1
	//because we're multiplying by deltatime we won't know what number will reach above 1
	float DragScale = std::min(Drag * Game::GetGameInstance().GetFDeltaTime(), 1.0f);

	//multiply velocity by a number below 1 and above 0 to make velocity smaller over time
	Vector2 DragVelocity = Velocity * DragScale;

	// remove the drag velocity from velocity
	Velocity -= DragVelocity;

	// move the gameobject based on velocity 
	OwnerObject->Position += Velocity * Game::GetGameInstance().GetFDeltaTime();

	//reset acceleration each frame
	Acceleration = Vector2::Zero();
}

void PhysicsComponent::AddForce(Vector2 Direction, float Force)
{
	//normalise the direction and multiply it by force
	Acceleration = Direction.Normalised() * Force;
}

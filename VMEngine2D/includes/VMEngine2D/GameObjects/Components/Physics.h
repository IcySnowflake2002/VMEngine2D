#pragma once
#include "VMEngine2D/GameObjects/Components.h"
#include "VMEngine2D/Vector2.h"

class Physics :
	public Components {
public: 
	//functions
	Physics(GameObject* OwnerToAttatch);

	void Update() override;

	// add acceleration in a direction multiplied by a strength force
	void AddForce(Vector2 Direction, float Force);

public:
	//variables

	//Current Speed
	Vector2 Velocity;

	//Max Speed for the velocity
	float MaxVelocity;

	//how fast to reach max speed
	Vector2 Acceleration;

	//how fast the velocity decelerates
	float Drag;

};

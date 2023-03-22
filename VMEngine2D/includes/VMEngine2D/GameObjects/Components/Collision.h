#pragma once
#include "VMEngine2D/GameObjects/Components.h"
#include "VMEngine2D/Vector2.h"
#include <iostream>
#include <vector>

struct STColDimensions {
	float Width = 25.0f;
	float Height = 25.0f;
	Vector2 Position;
};

class Collision : public Components {
public:
	Collision(GameObject* OwnerObject);
	~Collision();

	void Update() override;

	//return true if any collision is overlapped
	bool IsOverlappingAny();

	//return true if collision is overlapping gameobject with tag
	bool IsOverlappingTag(const char* Tag);

	//return the gameobjects this collision is overlapping with this tag
	std::vector<Collision*> GetOverlappedByTag(const char* Tag);

	//remove a collider from the overlapped collisions
	void RemoveCollisionFromOverlapped(Collision* Collision);

public:
	//dimensions and position for the collision
	STColDimensions Dimensions;

	//store the colliders that are being overlapped
	std::vector<Collision*> OverlappedCollisions;
};
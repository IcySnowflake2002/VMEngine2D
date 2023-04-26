#pragma once
#include "VMEngine2D/GameObjects/Character.h"

enum CollectAnims : unsigned int {
	PU_SHIELD = 0
};

class Collectible : public Character {
public:
	Collectible(Vector2 StartPos, SDL_Renderer* Renderer);
	virtual ~Collectible();

	virtual void Update() override;

	virtual void Draw(SDL_Renderer* Renderer) override;
};
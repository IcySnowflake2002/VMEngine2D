#pragma once
#include "VMEngine2D/GameObjects/Character.h"

enum EnemyAnims : unsigned int {
	BASE = 0,
	BASE2 = 1,
	BOOSTERS = 2
};

class Enemy :
	public Character {
public:
	Enemy(Vector2 StartPosition, SDL_Renderer* Renderer);
	virtual ~Enemy();

	virtual void Update() override;

	virtual void Draw(SDL_Renderer* Renderer) override;
};

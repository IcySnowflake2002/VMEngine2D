#pragma once
#include "VMEngine2D/GameObjects/Character.h"

enum EnemyAnims : unsigned int {
	BASE = 0,
	BASE2 = 1,
	BASE3 = 2,
	BOOSTERS = 3,
	BASE_DESTRUCTION = 4
};

class Enemy :
	public Character {
public:
	Enemy(EnemyAnims EnemyType, Vector2 StartPosition, SDL_Renderer* Renderer);
	virtual ~Enemy();

	virtual void Update() override;

	virtual void Draw(SDL_Renderer* Renderer) override;

public: 
	EnemyAnims EnemyType;
};

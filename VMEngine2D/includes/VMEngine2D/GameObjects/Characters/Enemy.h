#pragma once
#include "VMEngine2D/GameObjects/Character.h"

struct Mix_Chunk;

enum EnemyAnims : unsigned int {
	BASE = 0,
	BASE2 = 1,
	BASE3 = 2,
	BOOSTERS = 3,
	BASE_DESTRUCTION = 4,
	BASE2_DESTRUCTION = 5,
	BASE3_DESTRUCTION = 6
};

class Enemy :
	public Character {
public:
	Enemy(EnemyAnims EnemyType, Vector2 StartPosition, SDL_Renderer* Renderer);
	virtual ~Enemy();

	virtual void Update() override;

	virtual void Draw(SDL_Renderer* Renderer) override;

	bool bIsDestroyed;

	float DestroyTimer;

public: 
	//Store animations for the enemy type and destruction anims
	EnemyAnims EnemyType;

	EnemyAnims EnemyDestroy;

	//Store the audio for the destruction of enemy
	Mix_Chunk* sfx_Destroy[1] = { nullptr };
};

#pragma once
#include "VMEngine2D/GameObjects/Character.h"

enum PlayerAnims : unsigned int {
	BASE_FULL = 0,
	ENG_SC = 1,
	BOO_IDLE = 2,
	BOO_POWER = 3,
	SHIELD = 4,
	BASE_SMLDMG = 5,
	BASE_MEDDMG = 6,
	BASE_LRGDMG = 7,
	EMPTY = 8
};

class PlayerChar :
	public Character {
public:
	PlayerChar(Vector2 StartPosition, SDL_Renderer* Renderer);

	//Override these methods to create a more specialized design for the Player Character
	virtual void ProcessInput(Input* PlayerInput) override;

	virtual void Update() override;

	virtual void Draw(SDL_Renderer* Renderer) override;

protected:
	// the direction of the player based on input
	Vector2 InputDir;

	// play the correct animation for the boosters
	unsigned int BoostersIndex;

	unsigned int DmgIndex;

	// play the correct animation for the shield
	unsigned int ShdIndex;

	//set the max amount of lives
	unsigned int MaxLives = 4;
};
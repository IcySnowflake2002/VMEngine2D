#pragma once
#include "VMEngine2D/GameObject.h"

class AnimStateMachine;
struct STAnimationData;

class Character :
	public GameObject {
public:
	Character(Vector2 StartPosition);
	~Character();

	// virtual allows for derived classes to override
	// override changes the definition of the parent method to this one.
	virtual void Draw(SDL_Renderer* Renderer) override;

	// Add an animation into the AnimStateMachine
	void AddAnimation(SDL_Renderer* Renderer, const char* SpriteSheetPath, STAnimationData AnimationData);

protected:
	//Store all animations
	AnimStateMachine* CharacterAnimations;

	//Animation index used by the anim state machine
	unsigned int AnimIndex;

	//flip the animation
	bool bFlipped;

	//Maximum Velocity/speed
	float MaxMoveSpeed;
};
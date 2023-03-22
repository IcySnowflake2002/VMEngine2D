#pragma once
#include "VMEngine2D/GameObject.h"

class AnimStateMachine;
struct STAnimationData;
class Physics;
class Collision;

class Character :
	public GameObject {
public:
	Character(Vector2 StartPosition);
	~Character();

	//override the parent update()
	virtual void Update() override;

	// virtual allows for derived classes to override
	// override changes the definition of the parent method to this one.
	virtual void Draw(SDL_Renderer* Renderer) override;

	// Add an animation into the AnimStateMachine
	void AddAnimation(SDL_Renderer* Renderer, const char* SpriteSheetPath, STAnimationData AnimationData);

protected:
	//Store all animations
	AnimStateMachine* CharacterAnimations;

	// direction to move in
	Vector2 MovementDir;

	//Animation index used by the anim state machine
	unsigned int AnimIndex;

	//flip the animation
	bool bFlipped;

	//store a reference to the physics component
	Physics* CharPhysics;

	//store a reference to the collision component
	Collision* CharCollision;

	//register the overlap as detected and change to green
	bool bOverlapDetected;

	//show collision if true
	bool bDebugCollision;
};
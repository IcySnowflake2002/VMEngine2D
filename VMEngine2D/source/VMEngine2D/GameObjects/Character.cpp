#include "VMEngine2D/GameObjects/Character.h"
#include "VMEngine2D/AnimStateMachine.h"
#include "VMEngine2D/Input.h"

Character::Character(Vector2 StartPosition)
{
	CharacterAnimations = new AnimStateMachine();
	AnimIndex = 0;
	MaxMoveSpeed = 150.0f;
	Scale = 2.0f;
	bFlipped = false;
	Position = StartPosition;

	std::cout << "Character created" << std::endl;
}

Character::~Character()
{
	// remove the anim state machine from memory
	delete CharacterAnimations;

	std::cout << "Character Destroyed." << std::endl;
}

void Character::ProcessInput(Input* PlayerInput)
{
	// set the AnimIndex to play the first animation by default
	AnimIndex = 0;

	//If we're holding down W, then play the second animation instead
	if (PlayerInput->IsKeyDown(SDL_SCANCODE_W)) {
		AnimIndex = 1;
	}
	//If we're holding down E, then play the third animation instead
	if (PlayerInput->IsKeyDown(SDL_SCANCODE_E)) {
		AnimIndex = 2;
	}
}

void Character::Update()
{
}

void Character::Draw(SDL_Renderer* Renderer)
{
	// draw the animations for the character
	CharacterAnimations->Draw(Renderer, AnimIndex, Position, Scale, bFlipped);
}

void Character::AddAnimation(SDL_Renderer* Renderer, const char* SpriteSheetPath, STAnimationData AnimationData)
{
	// add an animation to the anim state machine using the anim state machine function
	CharacterAnimations->AddAnimation(Renderer, SpriteSheetPath, AnimationData);
}

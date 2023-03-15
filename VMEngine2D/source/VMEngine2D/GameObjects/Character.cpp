#include "VMEngine2D/GameObjects/Character.h"
#include "VMEngine2D/GameObjects/Components/Physics.h"
#include "VMEngine2D/AnimStateMachine.h"
#include "VMEngine2D/Input.h"

Character::Character(Vector2 StartPosition)
{
	CharacterAnimations = new AnimStateMachine();
	AnimIndex = 0;
	Scale = 2.0f;
	bFlipped = false;
	Position = StartPosition;

	//create a new PhysicsComponent and store it
	Physics = new PhysicsComponent(this);

	std::cout << "Character created" << std::endl;
}

Character::~Character()
{
	// remove the anim state machine from memory
	delete CharacterAnimations;

	std::cout << "Character Destroyed." << std::endl;
}

void Character::Update()
{
	//Run the parent class update first
	GameObject::Update();
}

void Character::Draw(SDL_Renderer* Renderer)
{
	// draw the animations for the character
	CharacterAnimations->Draw(Renderer, AnimIndex, Position, Rotation, Scale, bFlipped);
}

void Character::AddAnimation(SDL_Renderer* Renderer, const char* SpriteSheetPath, STAnimationData AnimationData)
{
	// add an animation to the anim state machine using the anim state machine function
	CharacterAnimations->AddAnimation(Renderer, SpriteSheetPath, AnimationData);
}

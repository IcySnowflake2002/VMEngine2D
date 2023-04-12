#include "VMEngine2D/GameObjects/Character.h"
#include "VMEngine2D/GameObjects/Components/Physics.h"
#include "VMEngine2D/AnimStateMachine.h"
#include "VMEngine2D/Input.h"
#include "VMEngine2D/GameObjects/Components/Collision.h"
#include "sdl2/SDL.h"

Character::Character(Vector2 StartPosition)
{
	CharacterAnimations = new AnimStateMachine();
	AnimIndex = 0;
	Scale = 2.0f;
	bFlipped = false;
	Position = StartPosition;
	bOverlapDetected = false;
	bDebugCollision = true;

	//make sure nothing is dead on spawn
	Lives = 1;

	//create a new PhysicsComponent and store it
	CharPhysics = new Physics(this);

	//create a new CollisionComponent and store it
	CharCollision = new Collision(this);

	//resize the collision custom to the object
	CharCollision->Dimensions.Width = 150.0f;
	CharCollision->Dimensions.Height = 150.0f;

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

	if (bDebugCollision = true) {
		//draw the collision
		if (bOverlapDetected) {
			//this will draw green
			//r g b a = red, green, blue, alpha(transparency)
			//0 - 255 for different shades of color
			SDL_SetRenderDrawColor(Renderer, 0, 255, 0, 255);
		}
		else {
			//draw the rectangle as red
			SDL_SetRenderDrawColor(Renderer, 255, 0, 0, 255);
		}

		//Convert Dimensions into an SDL_FRect
		SDL_FRect ColRect =
		{ CharCollision->Dimensions.Position.x,
			CharCollision->Dimensions.Position.y,
			CharCollision->Dimensions.Width,
			CharCollision->Dimensions.Height };

		//Draw the collider as a box around the object
		SDL_RenderDrawRectF(Renderer, &ColRect);
	}
}

void Character::AddAnimation(SDL_Renderer* Renderer, const char* SpriteSheetPath, STAnimationData AnimationData)
{
	// add an animation to the anim state machine using the anim state machine function
	CharacterAnimations->AddAnimation(Renderer, SpriteSheetPath, AnimationData);
}

void Character::AddLives(int Amount)
{
	//clamp the value - prevent a negative number from being used
	Amount = std::max(0, Amount);

	//Add the amount to the lives
	Lives += Amount;
}

void Character::RemoveLives(int Amount)
{
	//Create a fake integer and clamp it instead
	int NewLives = Lives - Amount;

	NewLives = std::max(0, NewLives);

	//Remove the amount from the Lives using the clamped integer
	Lives = NewLives;
}

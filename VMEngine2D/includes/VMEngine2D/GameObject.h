#pragma once
#include "Vector2.h"

class Input;
struct SDL_Renderer;

class GameObject {
public:
	GameObject();
	// destructor needs to be virtual or the inherited class destructor won't run
	virtual ~GameObject();

	// Handle the input detection for this game object
	virtual void ProcessInput(Input* PlayerInput) {};

	// Run the logic of the Game Object
	virtual void Update();

	// Render the game object
	virtual void Draw(SDL_Renderer* Renderer) {};

protected:
	//Screen Position of the game object
	Vector2 Position;
};

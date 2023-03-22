#pragma once
#include "Vector2.h"
#include <iostream>
#include <vector>

class Input;
struct SDL_Renderer;
class Components;

// change the name of the type to reuse simpler wording
// the complier will read "ComponentStack" as "std::vector<components>
typedef std::vector<Components*> ComponentStack;

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

	//add a component into the component stack
	void AddComponent(Components* NewComponent);

	//return the tag of the game object
	const char* GetTag() const { return Tag; }

	//set the gameobject to be destroyed in the handle garbage function
	void DestroyGameObject() { bShouldDestroy = true; };

	//get the should destroy boolean
	bool ShouldDestroy() const { return bShouldDestroy; };

	//Screen Position of the game object
	Vector2 Position;
protected:

	//Scale of the object
	float Scale;

	//Rotation of the whole object
	double Rotation;

	//store an array of components
	ComponentStack AllComponents;

	//tag the gameobject for detection
	const char* Tag;
private:
	//if set game should destroy the object
	bool bShouldDestroy;
};

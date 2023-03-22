#pragma once
#include <vector>
#include <iostream>
#include "sdl2/SDL.h"

class GameObject;
class Input;
class Collision;

//at compile time the compiler will turn the type definition into the proper one
// this is purely for readability
typedef std::vector<Collision*>::iterator ColIterator;
typedef std::vector<GameObject*>::iterator GOIterator;

class Game {
public:

	//Get the game singleton class
	//this will allow us to access global functions and variables -
	// - in the game class
	static Game& GetGameInstance();

	//Destroy the game
	static void DestroyGameInstance();

	//Initialise the Game before the loop runs
	void Start(const char* WTitle, bool bFullScreen, int WWidth, int WHeight);

	//For precise timing
	double GetDeltaTime() const { return DeltaTime;  }

	//for average timing
	float GetFDeltaTime() const { return static_cast<float>(DeltaTime); }

	//Sets bGameOver to true which closes the app
	void CloseApp() { bIsGameOver = true; }

	//Add a collision component to the components stack
	void AddCollisionToGame(Collision* Collider);

	//remove a collision from the component stack
	void RemoveCollisionFromGame(Collision* Collider);

	//Return all the colliders in the game
	std::vector<Collision*> GetGameColliders() const { return AllColliders; };

private: 
	//Constructor
	Game();
	//Destructor
	~Game();

	//Handle the input of the user
	void ProcessInput();

	//Handle the game logic
	void Update();

	//Render the graphics to the screen
	void Draw();

	//Run the game loop
	void Run();

	//Clean up any memory we need to clean
	void CloseGame();

	//load game logic before the game update runs
	void BeginPlay();

	//Sort out Garbage
	void HandleGarbage();

private:
	//handle the game loop condition
	//on true end game
	bool bIsGameOver;

	//Store the app window
	SDL_Window* SdlWindow;

	//store the renderer to render graphics
	SDL_Renderer* SdlRenderer;

	//hold the time between each frame
	double DeltaTime;

	//Detection for player input
	Input* PlayerInput;

	//GameObject stack
	std::vector<GameObject*> AllGameObjects;

	//Collision Stack
	std::vector<Collision*> AllColliders;
};
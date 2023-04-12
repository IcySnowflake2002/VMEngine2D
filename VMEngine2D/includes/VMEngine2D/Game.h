#pragma once
#include <vector>
#include <iostream>
#include "sdl2/SDL.h"

class GameObject;
class Input;
class Collision;
class GameStateMachine;

class Game {
public:

	//Get the game singleton class
	//this will allow us to access global functions and variables -
	// - in the game class
	static Game& GetGameInstance();

	//Destroy the game
	static void DestroyGameInstance();

	//Add things into the game
	void Instantiate();

	//Initialise the Game before the loop runs
	void Start(const char* WTitle, bool bFullScreen, int WWidth, int WHeight);

	//For precise timing
	double GetDeltaTime() const { return DeltaTime;  }

	//for average timing
	float GetFDeltaTime() const { return static_cast<float>(DeltaTime); }

	//Sets bGameOver to true which closes the app
	void CloseApp() { bIsGameOver = true; }

	//Return all the colliders in the game
	std::vector<Collision*> GetGameColliders() const;

	//Return the GameStateMachine
	GameStateMachine* GetGameStates() { return GameStates; }

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

public:
	//store the current players score to be shared across states
	unsigned int GameScore;

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

	//store the current game state
	GameStateMachine* GameStates;
};
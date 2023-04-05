//Required Headers
#include "VMEngine2D/Game.h"
#include "VMEngine2D/Vector2.h"
#include "VMEngine2D/Input.h"
#include "VMEngine2D/GameObject.h"
#include "VMEngine2D/GameState.h"

//Game States
#include "VMEngine2D/GameStates/PlayState.h"

//SDL2
#include "sdl2/SDL_ttf.h"

using namespace std;

Game& Game::GetGameInstance()
{
	//Create a game instance only if there isn't one already
	//Local static variables can only be created once
	static Game* GameInstance = new Game();
	return *GameInstance;
}

void Game::DestroyGameInstance()
{
	//Store the game instance in a static variable
	static Game* GameInstance = &GetGameInstance();
	//remove the game instance from memory
	delete GameInstance;
}

std::vector<Collision*> Game::GetGameColliders() const
{
	// return the collisions of the current state
	return GameStates->GetCurrentState()->GetGameStateCollisions();
}


Game::Game()
{
	cout << "Initialised Game Instance!" << endl;
	bIsGameOver = false;
	SdlWindow = nullptr;
	DeltaTime = 0.0;
	SdlRenderer = nullptr;
	PlayerInput = nullptr;

	GameStates = nullptr;
	GameScore = 0;

}

Game::~Game()
{
	cout << "Destroyed Game Instance..." << endl;
}

void Game::Start(const char* WTitle, bool bFullScreen, int WWidth, int WHeight)
{
	// Intialise SDL and end the game if it fails
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		//Error Handle
		cout << "SDL Failed to load: " << SDL_GetError() << endl;
	}

	// Default to Window Mode
	//Hold ctrl to see where SDL_WINDOW_SHOWN goes to and view other documentation.
	int FullScreenFlag = SDL_WINDOW_SHOWN;

	//If fullscreen is true then overwrite to full screen
	if (bFullScreen == true) {
		FullScreenFlag = SDL_WINDOW_FULLSCREEN;
	}

	//Create Window
	SdlWindow = SDL_CreateWindow(
		WTitle,
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		WWidth, WHeight,
		FullScreenFlag
	);

	//If the window wasn't created
	if (SdlWindow == nullptr) {
		//Error log
		cout << "SDL Window Creation failed: " << SDL_GetError() << endl;
		CloseGame();
		return;
	}

	//set up the renderer
	SdlRenderer = SDL_CreateRenderer(SdlWindow, 0, -1);
	if (SdlRenderer == nullptr) {
		cout << "SDL Renderer creation failed: " << SDL_GetError() << endl;
		//remove the window
		SDL_DestroyWindow(SdlWindow);
		//uninitalise SDL
		CloseGame();
		return;
	}

	//intialise TTF and if it equals -1 then it failed; 0 means it succeeded
	if (TTF_Init() < 0) {
		std::cout << "SDL TTF failed to intialise: " << TTF_GetError() << endl;

		CloseGame();
		return;
	}

	//Create an object of the input class
	PlayerInput = new Input();

	//randomise the seed using the current calender time so we always have a different random seed
	srand(time(NULL));

	Run();
}

void Game::ProcessInput()
{
	//this must run before all other process input detection
	PlayerInput->ProcessInput();

	// run the input of the current game state
	GameStates->GetCurrentState()->ProcessInput(PlayerInput);
}

void Game::Update()
{
	//static variables in methods will only initialise once
	// update with the previous fraames time passed
	static double LastTickTime = 0.0;
	// get the current time milliseconds that has passed since the game has started
	// GetTicks64 returns a Uint64 which means we need to convert it into the double
	double CurrentTickTime = static_cast<double>(SDL_GetTicks64());
	//get the difference between last tick time and current tick time
	double DeltaMil = CurrentTickTime - LastTickTime;
	//set delta time but convert it to seconds
	DeltaTime = DeltaMil / 1000.0;
	//set the last tick time as the current time for the next frame
	LastTickTime = CurrentTickTime;

	// run the update of the current state and pass in float delta time
	GameStates->GetCurrentState()->Update(GetFDeltaTime());

}

void Game::Draw()
{
	//set background colour of app
	SDL_SetRenderDrawColor(SdlRenderer, 0, 50, 200, 255);
	//clear the previous frame
	SDL_RenderClear(SdlRenderer);

	//do anything that needs to be drawn to the screen here
	//run the current game state draw and pass in the renderer
	GameStates->GetCurrentState()->Draw(SdlRenderer);

	//Show the new frame
	SDL_RenderPresent(SdlRenderer);
}

void Game::Run()
{
	if (!bIsGameOver) {
		BeginPlay();
	}

	//check if the game over is false (something has thrown an error)
	//If not false run game loop
	while (!bIsGameOver) {
		ProcessInput();
		Update();
		Draw();
		HandleGarbage();
	}
	
	CloseGame();
}

void Game::CloseGame()
{
	// clean up any values that were initialised into memory

	if (GameStates != nullptr) {
		//handle game asset deletion
		cout << "Deleting Game Assets..." << endl;
		delete GameStates;
	}

	if (PlayerInput != nullptr) {
		// delete player input from memory
		cout << "Deleting top level systems..." << endl;
		delete PlayerInput;
	}

	//Handle SDL unintialisation
	cout << "Cleaning up SDL" << endl;

	if (SdlWindow != nullptr) {
		//destroy the sdl window
		SDL_DestroyWindow(SdlWindow);
	}

	if (SdlRenderer != nullptr) {
		//destroy the sdlrenderer
		SDL_DestroyRenderer(SdlRenderer);
	}

	SDL_Quit();
}

void Game::BeginPlay()
{
	cout << "Load Game Assets..." << endl;

	//create a game state for the starting state
	PlayState* StartingState = new PlayState(SdlWindow, SdlRenderer);
	//create a game state machine and add the starting state
	GameStates = new GameStateMachine(StartingState);
}

void Game::HandleGarbage()
{
	GameStates->GetCurrentState()->HandleGarbage();
}


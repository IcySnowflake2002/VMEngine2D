#include "VMEngine2D\Game.h"
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

Game::Game()
{
	cout << "Initialised Game Instance!" << endl;
	bIsGameOver = false;
	SdlWindow = nullptr;
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
		//Unintialise SDL
		SDL_Quit();
		return;
	}

	Run();
}

void Game::ProcessInput()
{
	//This stores the inputs as an event
	SDL_Event PollEvent;

	//This will listen to the event and end the loop after all inputs are detected.
	while (SDL_PollEvent(&PollEvent)) {
		//detect the type of event that was input
		switch (PollEvent.type) {
		case SDL_QUIT :
			bIsGameOver = true;
			break;
		default:
			break;
		}
	}
}

void Game::Update()
{
	cout << "Updating!" << endl;
}

void Game::Draw()
{

}

void Game::Run()
{
	//check if the game over is false (something has thrown an error)
	//If not false run game loop
	while (!bIsGameOver) {
		ProcessInput();
		Update();
		Draw();
		SDL_Delay(5);
	}
	
	CloseGame();
}

void Game::CloseGame()
{
	//Handle SDL unintialisation
	cout << "Cleaning up SDL" << endl;
	SDL_DestroyWindow(SdlWindow);
	SDL_Quit();
}


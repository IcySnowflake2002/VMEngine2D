#include "VMEngine2D\Game.h"
#include "VMEngine2D/Vector2.h"
#include "VMEngine2D/Texture.h"
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
	Texture1 = nullptr;
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

	//set up the renderer
	SdlRenderer = SDL_CreateRenderer(SdlWindow, 0, -1);
	if (SdlRenderer == nullptr) {
		cout << "SDL Renderer creation failed: " << SDL_GetError() << endl;
		//remove the window
		SDL_DestroyWindow(SdlWindow);
		//uninitalise SDL
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

}

void Game::Draw()
{
	//set background colour of app
	SDL_SetRenderDrawColor(SdlRenderer, 0, 50, 200, 255);
	//clear the previous frame
	SDL_RenderClear(SdlRenderer);

	//Draw the texture
	if (Texture1 != nullptr) {
		Texture1->Draw(SdlRenderer, Vector2(100.0f, 100.0f), nullptr, 1.0f, false);
	}
	if (Texture2 != nullptr) {
		Texture2->Draw(SdlRenderer, Vector2(210.0f, 150.0f), nullptr, 0.3f, true);
	}
	if (Texture3 != nullptr) {
		Texture3->Draw(SdlRenderer, Vector2(300.0f, 100.0f), nullptr, 1.0f, false);
	}
	if (Texture4 != nullptr) {
		Texture4->Draw(SdlRenderer, Vector2(400.0f, 150.0f), nullptr, 0.3f, true);
	}
	if (Texture5 != nullptr) {
		Texture5->Draw(SdlRenderer, Vector2(500.0f, 100.0f), nullptr, 1.0f, false);
	}
	if (Texture6 != nullptr) {
		Texture6->Draw(SdlRenderer, Vector2(150.0f, 300.0f), nullptr, 0.3f, true);
	}
	if (Texture7 != nullptr) {
		Texture7->Draw(SdlRenderer, Vector2(270.0f, 350.0f), nullptr, 1.0f, false);
	}
	if (Texture8 != nullptr) {
		Texture8->Draw(SdlRenderer, Vector2(370.0f, 300.0f), nullptr, 0.3f, true);
	}
	if (Texture9 != nullptr) {
		Texture9->Draw(SdlRenderer, Vector2(470.0f, 350.0f), nullptr, 1.0f, false);
	}
	if (Texture10 != nullptr) {
		Texture10->Draw(SdlRenderer, Vector2(570.0f, 300.0f), nullptr, 0.3f, true);
	}

	//Show the new frame
	SDL_RenderPresent(SdlRenderer);
}

void Game::Run()
{
	if (!bIsGameOver) {
		Texture1 = new Texture();
		if (!Texture1->LoadImageFromFile("Content/images/Letters/HRed.png", SdlRenderer)) {
			Texture1 = nullptr;
		}

		Texture2 = new Texture();
		if (!Texture2->LoadImageFromFile("Content/Images/Letters/EBlue.png", SdlRenderer)) {
			Texture2 = nullptr;
		}
		Texture3 = new Texture();
		if (!Texture3->LoadImageFromFile("Content/images/Letters/L.png", SdlRenderer)) {
			Texture3 = nullptr;
		}
		Texture4 = new Texture();
		if (!Texture4->LoadImageFromFile("Content/images/Letters/LBlue.png", SdlRenderer)) {
			Texture4 = nullptr;
		}
		Texture5 = new Texture();
		if (!Texture5->LoadImageFromFile("Content/images/Letters/ORed.png", SdlRenderer)) {
			Texture5 = nullptr;
		}
		Texture6 = new Texture();
		if (!Texture6->LoadImageFromFile("Content/images/Letters/W.png", SdlRenderer)) {
			Texture6 = nullptr;
		}
		Texture7 = new Texture();
		if (!Texture7->LoadImageFromFile("Content/images/Letters/ORed.png", SdlRenderer)) {
			Texture7 = nullptr;
		}
		Texture8 = new Texture();
		if (!Texture8->LoadImageFromFile("Content/images/Letters/RBlue.png", SdlRenderer)) {
			Texture8 = nullptr;
		}
		Texture9 = new Texture();
		if (!Texture9->LoadImageFromFile("Content/images/Letters/LRed.png", SdlRenderer)) {
			Texture9 = nullptr;
		}
		Texture10 = new Texture();
		if (!Texture10->LoadImageFromFile("Content/images/Letters/D.png", SdlRenderer)) {
			Texture10 = nullptr;
		}
	}

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


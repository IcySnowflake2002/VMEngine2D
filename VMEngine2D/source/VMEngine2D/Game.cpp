#include "VMEngine2D/Game.h"
#include "VMEngine2D/Vector2.h"
#include "VMEngine2D/Animation.h"
#include "VMEngine2D/Input.h"
#include "VMEngine2D/GameObjects/Characters/PlayerChar.h"
#include "VMEngine2D/GameObjects/Characters/Enemy.h"
#include "VMEngine2D/GameObject.h"
#include "VMEngine2D/Animation.h"

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

void Game::AddCollisionToGame(Collision* Collider)
{
	//Add a collision into the AllColliders stack
	AllColliders.push_back(Collider);

	std::cout << "Added Collision into the game." << std::endl;
}

void Game::RemoveCollisionFromGame(Collision* Collider)
{
	//the find function finds an object or data using the object in a vector stack
	// @param 1 - look from the start of the array
	// @param 2 - to the end of the array
	// @param 3 - the object that we are searching for
	ColIterator ColToRemove = std::find(AllColliders.begin(), AllColliders.end(), Collider);

	// the find function will set the iterator to AllColliders.end() if it doesn't find anything
	if (ColToRemove == AllColliders.end()) {
		// didn't find a collision
		return; // return will kill the rest of the function if it's run
	}

	//remove the collider if find function found the collider
	AllColliders.erase(ColToRemove);
	std::cout << "Collision successfully removed from the game." << std::endl;
}


Game::Game()
{
	cout << "Initialised Game Instance!" << endl;
	bIsGameOver = false;
	SdlWindow = nullptr;
	DeltaTime = 0.0;
	SdlRenderer = nullptr;
	PlayerInput = nullptr;

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

	//Create an object of the input class
	PlayerInput = new Input();

	Run();
}

void Game::ProcessInput()
{
	//this must run before all other process input detection
	PlayerInput->ProcessInput();

	//process the input of each GameObject
	for (GameObject* SingleGameObject : AllGameObjects) {
		SingleGameObject->ProcessInput(PlayerInput);
	}
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

	// run the game objects logic
	for (GameObject* SingleGameObject : AllGameObjects) {
		SingleGameObject->Update();
	}

	//set a static timer to count up based on DeltaTime
	// static variables don't reininitalise
	static double SpawnTimer = 0.0;
	SpawnTimer += DeltaTime;

	//after 5 seconds spawn enemy then reset timer
	if (SpawnTimer > 5.0) {
		//set up variables to recieve the app window width and height
		int WinWidth, WinHeight = 0;

		//Use SDL function to set the dimensions
		SDL_GetWindowSize(SdlWindow, &WinWidth, &WinHeight);

		//Increase Window Width by 1
		WinWidth += 1;

		//get a random number between 0 and the window width
		//rand() gets random number between 0 and number afteer %
		int SpawnEnemyX = rand() % WinWidth;

		// spawn an enemy based on a random screen x location
		Enemy* NewEnemy = new Enemy(Vector2(SpawnEnemyX, -128.0f), SdlRenderer);

		//add the enemy to the game object stack
		AllGameObjects.push_back(NewEnemy);

		//Reset Timer to 0 and start again
		SpawnTimer = 0.0f;
	}

}

void Game::Draw()
{
	//set background colour of app
	SDL_SetRenderDrawColor(SdlRenderer, 0, 50, 200, 255);
	//clear the previous frame
	SDL_RenderClear(SdlRenderer);

	//do anything that needs to be drawn to the screen here
	
	//cycle through all of the ganeobjects in the AllGameObjects array
	//each loop reassign the SingleGameObject pointer with the next item in the array
	for (GameObject* SingleGameObject : AllGameObjects) {
		//each loop run the draw function for each gameobject
		SingleGameObject->Draw(SdlRenderer);
	}
	
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
	//handle game asset deletion
	cout << "Deleting Game Assets..." << endl;
	// delete player input from memory
	delete PlayerInput;

	//Handle SDL unintialisation
	cout << "Cleaning up SDL" << endl;
	SDL_DestroyWindow(SdlWindow);
	SDL_Quit();
}

void Game::BeginPlay()
{
	cout << "Load Game Assets..." << endl;

	PlayerChar* MyCharacter = new PlayerChar(Vector2(100.0f, 100.0f), SdlRenderer);
	Enemy* Bomber = new Enemy(Vector2(300.0f, 100.0f), SdlRenderer);
	Enemy* Bomber2 = new Enemy(Vector2(500.0f, 250.0f), SdlRenderer);

	//Add the character into the Game Object stack
	AllGameObjects.push_back(Bomber);
	AllGameObjects.push_back(Bomber2);
	AllGameObjects.push_back(MyCharacter);
}

void Game::HandleGarbage()
{
	//loop through all of the gameobjects and assign the iterator each loop
	for (GOIterator Object = AllGameObjects.begin(); Object != AllGameObjects.end();) {
		//if the object is not marked for delete then skip to the next one
		if (!(*Object)->ShouldDestroy()) {
			Object++;
			continue;
		}
		
		//delete the gameobject
		delete* Object;

		//remove the object from the array and resize the array
		Object = AllGameObjects.erase(Object);

		std::cout << "Deleted game object." << std::endl;
	}
}


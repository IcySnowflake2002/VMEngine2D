#include "VMEngine2D/GameState.h"
#include "VMEngine2D/GameObject.h"
#include "sdl2/SDL.h"

//move these libraries later to a PlayState subclass
#include "VMEngine2D/GameObjects/Characters/Enemy.h"
#include "VMEngine2D/GameObjects/Characters/PlayerChar.h"

GameState::GameState(SDL_Window* Window, SDL_Renderer* Renderer)
{
	StateWindow = Window;
	StateRenderer = Renderer;
	bHasActivated = false;
}

GameState::~GameState()
{
}

void GameState::ProcessInput(Input* PlayerInput)
{
	//process the input of each GameObject
	for (GameObject* SingleGameObject : StateGameObjects) {
		SingleGameObject->ProcessInput(PlayerInput);
	}
}

void GameState::Update(float DeltaTime)
{
	if (!bHasActivated) {
		bHasActivated = true;
		BeginState();
	}

	// run the game objects logic
	for (GameObject* SingleGameObject : StateGameObjects) {
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
		SDL_GetWindowSize(StateWindow, &WinWidth, &WinHeight);

		//Increase Window Width by 1
		WinWidth += 1;
		WinWidth -= 128;

		//get a random number between 0 and the window width
		//rand() gets random number between 0 and number afteer %
		int SpawnEnemyX = rand() % WinWidth;

		// spawn an enemy based on a random screen x location
		Enemy* NewEnemy = new Enemy(Vector2(SpawnEnemyX, -128.0f), StateRenderer);

		//add the enemy to the game object stack
		StateGameObjects.push_back(NewEnemy);

		//Reset Timer to 0 and start again
		SpawnTimer = 0.0f;
	}
}

void GameState::Draw(SDL_Renderer* Renderer)
{ 
	//cycle through all of the ganeobjects in the AllGameObjects array
	//each loop reassign the SingleGameObject pointer with the next item in the array
	for (GameObject* SingleGameObject : StateGameObjects) {
		//each loop run the draw function for each gameobject
		SingleGameObject->Draw(StateRenderer);
	}
}

void GameState::HandleGarbage()
{
	//loop through all of the gameobjects and assign the iterator each loop
	for (GOIterator Object = StateGameObjects.begin(); Object != StateGameObjects.end();) {
		//if the object is not marked for delete then skip to the next one
		if (!(*Object)->ShouldDestroy()) {
			Object++;
			continue;
		}

		//delete the gameobject
		delete* Object;

		//remove the object from the array and resize the array
		Object = StateGameObjects.erase(Object);

		std::cout << "Deleted game object." << std::endl;
	}
}

void GameState::AddCollisionToGameState(Collision* StateCollision)
{
	//Add a collision into the AllColliders stack
	StateCollisions.push_back(StateCollision);
}

void GameState::RemoveCollisionFromGameState(Collision* StateCollision)
{
	//the find function finds an object or data using the object in a vector stack
	// @param 1 - look from the start of the array
	// @param 2 - to the end of the array
	// @param 3 - the object that we are searching for
	ColIterator ColToRemove = std::find(StateCollisions.begin(), StateCollisions.end(), StateCollision);

	// the find function will set the iterator to AllColliders.end() if it doesn't find anything
	if (ColToRemove == StateCollisions.end()) {
		// didn't find a collision
		return; // return will kill the rest of the function if it's run
	}

	//remove the collider if find function found the collider
	StateCollisions.erase(ColToRemove);
}

void GameState::BeginState()
{
	PlayerChar* MyCharacter = new PlayerChar(Vector2(100.0f, 100.0f), StateRenderer);

	//Add the character into the Game Object stack
	StateGameObjects.push_back(MyCharacter);
}

void GameState::EndState()
{
	// mark all game objects for deletion
	for (GameObject* SingleGameObject : StateGameObjects) {
		SingleGameObject->DestroyGameObject();
	}

	// run the handle garbage to delete them
	HandleGarbage();
}

GameStateMachine::GameStateMachine(GameState* StartingState)
{
	SwitchState(StartingState);
}

GameStateMachine::~GameStateMachine()
{
	// run through all the states, end them, then delete them
	for (GameState* SingleGameState : GameStateStack) {
		SingleGameState->EndState();
		delete SingleGameState;
	}

	// clear the stack
	GameStateStack.clear();
}

void GameStateMachine::SwitchState(GameState* NewState)
{
	//take out the current top state
	PopState();
	//add in a new state
	PushState(NewState);
}

void GameStateMachine::PushState(GameState* NewState)
{
	//Add the state to the top of the stack
	GameStateStack.push_back(NewState);
}

void GameStateMachine::PopState()
{
	if (GameStateStack.size() > 1) {
		// cache the old state - we can delete it after it's removed
		GameState* OldState = GetCurrentState();
		//remove it from the stack
		GameStateStack.pop_back();
		//end the old state then delete it
		OldState->EndState();
		delete OldState;
	}
}

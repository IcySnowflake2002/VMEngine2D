#include "VMEngine2D/GameState.h"
#include "VMEngine2D/GameObject.h"
#include "sdl2/SDL.h"
#include "VMEngine2D/Text.h"

GameState::GameState(SDL_Window* Window, SDL_Renderer* Renderer)
{
	StateWindow = Window;
	StateRenderer = Renderer;
	bHasActivated = false;
}

GameState::~GameState()
{
	//remove any of the text objects in the state
	for (Text* SingleText : StateTextObjects) {
		delete SingleText;
	}

	//clear the array
	StateTextObjects.clear();

	//delete all of the objects in the objectstospawn list
	for (GameObject* SingleObject : ObjectsToSpawn) {
		delete SingleObject;
	}

	ObjectsToSpawn.clear();
}

void GameState::ProcessInput(Input* PlayerInput)
{
	//stop the function if we haven't activated the state
	if (!bHasActivated) {
		return;
	}

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
}

void GameState::Draw(SDL_Renderer* Renderer)
{ 
	//stop the function if we haven't activated the state
	if (!bHasActivated) {
		return;
	}

	//cycle through all of the ganeobjects in the AllGameObjects array
	//each loop reassign the SingleGameObject pointer with the next item in the array
	for (GameObject* SingleGameObject : StateGameObjects) {
		//each loop run the draw function for each gameobject
		SingleGameObject->Draw(StateRenderer);
	}
	
	//draw all text objects in the state and make sure to draw them last
	// - so that they draw on top of the gameobjects
	for (Text* SingleText : StateTextObjects) {
		SingleText->Draw(Renderer);
	}
}

void GameState::Instantiate()
{
	// insert is inserting the whole objects to spawn list into the state game objects at beginning of the array
	StateGameObjects.insert(StateGameObjects.begin(), ObjectsToSpawn.begin(), ObjectsToSpawn.end());

	//Remove the objects from the ObjectsToSpawn
	ObjectsToSpawn.clear();
}

void GameState::HandleGarbage()
{
	//stop the function if we haven't activated the state
	if (!bHasActivated) {
		return;
	}

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
	//when the state starts

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

void GameState::SpawnGameObject(GameObject* ObjectToSpawn)
{
	ObjectsToSpawn.push_back(ObjectToSpawn);
}

void GameState::ActivateGameObject(GameObject* ObjectToAdd)
{
	StateGameObjects.push_back(ObjectToAdd);
}

void GameState::ActivateTextObject(Text* TextToAdd)
{
	StateTextObjects.push_back(TextToAdd);
}


//Game State Machine Code Below


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
	if (GameStateStack.size() > 0) {
		// cache the old state - we can delete it after it's removed
		GameState* OldState = GetCurrentState();
		//remove it from the stack
		GameStateStack.pop_back();
		//end the old state then delete it
		OldState->EndState();
		delete OldState;
	}
}

#pragma once
#include <iostream>
#include <vector>

struct SDL_Renderer;
struct SDL_Window;
class Input;
class GameObject;
class Collision;

class Text;

using namespace std;

//at compile time the compiler will turn the type definition into the proper one
// this is purely for readability

typedef vector<Collision*>::iterator ColIterator;
typedef vector<GameObject*>::iterator GOIterator;

class GameState {
public:
	GameState(SDL_Window* Window, SDL_Renderer* Renderer);
	~GameState();

	//Game Loop functions for the specific state |
	virtual void ProcessInput(Input* PlayerInput);

	virtual void Update(float DeltaTime);

	virtual void Draw(SDL_Renderer* Renderer);

	void HandleGarbage();

	//Collisions |

	// Add a collision component into the game state collisions stack
	void AddCollisionToGameState(Collision* StateCollision);

	//remove a collision component from the game state collisions stack
	void RemoveCollisionFromGameState(Collision* StateCollision);

	// return the collisions for the current state
	vector<Collision*> GetGameStateCollisions() { return StateCollisions; }

	//State Updates |

	//run when the state updates for the first time
	virtual	void BeginState();
	
	//run when the game state has ended
	virtual void EndState();

protected:
	//add gameobjects to the state
	void ActivateGameObject(GameObject* ObjectToAdd);

	//add text object to the state
	void ActivateTextObject(Text* TextToAdd);

protected:
	// the window assigned to this state
	SDL_Window* StateWindow;
	// the renderer assigned to this state
	SDL_Renderer* StateRenderer;

private:
	//store all of the game objects specific to the state
	vector<GameObject*> StateGameObjects;

	// store all of the collision components specific to the state
	vector<Collision*> StateCollisions;

	//hold all of the text objects in the state
	vector<Text*> StateTextObjects;

	//will determine when the begin play is activated
	bool bHasActivated;

};

//game state machine class within the game state
class GameStateMachine {
public:
	GameStateMachine(GameState* StartingState);
	~GameStateMachine();

	//switch the current state with a new state
	void SwitchState(GameState* NewState);

	//add a state into the state stack
	//will not destroy the current state
	void PushState(GameState* NewState);

	//removes the last state that was added from the stack
	void PopState();

	//return the last added game state
	GameState* GetCurrentState() const { return GameStateStack.back(); }

private:
	vector<GameState*> GameStateStack;
};
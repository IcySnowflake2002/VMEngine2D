#include "VMEngine2D/Input.h"
#include "VMEngine2D/Game.h"
#include <iostream>

Input::Input()
{
	//Assigning all the states of the keys to the keyboard state
	//MULL means get all the keys
	KeyboardState = SDL_GetKeyboardState(NULL);
}

Input::~Input()
{
	
}

void Input::ProcessInput()
{
	//This stores the inputs as an event
	SDL_Event PollEvent;

	//This will listen to the event and end the loop after all inputs are detected.
	while (SDL_PollEvent(&PollEvent)) {
		//detect the type of event that was input
		switch (PollEvent.type) {
		case SDL_KEYDOWN:
			//set the keyboard state
			KeyboardState = SDL_GetKeyboardState(NULL);
			break;
		case SDL_KEYUP:
			//set the keyboard state
			KeyboardState = SDL_GetKeyboardState(NULL);
			break;
		case SDL_QUIT:
			// Go into the game instance and run the closeApp function.
			Game::GetGameInstance().CloseApp();
			break;
		default:
			break;
		}
	}
}

bool Input::IsKeyDown(SDL_Scancode Key)
{
	if (KeyboardState == nullptr) {
		std::cout << "No Keyboard State detected." << std::endl;
		return false;
	}
	return KeyboardState[Key];
}

#include "VMEngine2D/Input.h"
#include "VMEngine2D/Game.h"
#include "VMEngine2D/WindowMenu.h"
#include "sdl2/SDL_syswm.h"
#include <iostream>
#include "../resource.h"

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
			//Go into the game instance and run the closeApp function.
			Game::GetGameInstance().CloseApp();
			break;
		case SDL_SYSWMEVENT :
			//Check for Win32 Events
			HandleWMEvents(&PollEvent);
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

void Input::HandleWMEvents(SDL_Event* Event)
{
	//Listen out for system window menu button presses
	switch (Event->syswm.msg->msg.win.wParam) {
	case ID_FILE_RESTARTGAME:
		Game::GetGameInstance().RestartGame();
		break;
	case ID_FILE_ENDGAME:
		Game::GetGameInstance().CloseApp();
		break;
	case ID_GAME_CONTROLS:
		Game::GetGameInstance().GetTopMenu()->ActivatePopup(
			"Game Controls",								//title
			"Arrow Keys to Move \n Z to shoot");			//message
		break;
	case ID_HELP_ABOUTVMENGINE2D:
		Game::GetGameInstance().GetTopMenu()->ActivatePopup(
				"About VMEngine2D",																											
				"VMWEngine2D is an SDL2-based C++ 2D game engine created by Dylan Miller in 2023 for Basic Game Engine Programming. \n Gameplay BGM: Kinect Party - Bug Patrol by Double Fine Composers \n All other music obtained via FreeSound.org and YouTube's Royalty-Free music library");	
		break;
		// DEBUG MENU OPTIONS //
	case ID_DEBUG_GIVEMELIVES:
		Game::GetGameInstance().LifeDebug();
		break;
	case ID_DEBUG_GIVEMESHIELD:
		Game::GetGameInstance().ShdDebug();
		break;
	case ID_DEBUG_GIVEME100PTS:
		Game::GetGameInstance().ScoreDebug();
		break;
	case ID_DEBUG_NOGAMEFORYOU:
		Game::GetGameInstance().NoGame();
		break;
	case ID_DEBUG_AREALRESET:
		Game::GetGameInstance().ResetMe();
		break;
	default :
		break;
	}
}
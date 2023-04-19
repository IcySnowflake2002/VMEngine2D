#include "VMEngine2D/WindowMenu.h"
#include "VMEngine2D/Game.h"
#include "sdl2/SDL.h"
#include "sdl2/SDL_syswm.h"
#include "../resource.h" //double dots look outside of the folder structure
#include <Windows.h>
#include <iostream>

WindowMenu::WindowMenu(SDL_Window* Window)
{
	//intialise the window menu values
	this->Window = Window;
	WindowInfo = new SDL_SysWMinfo();

	//update window info with the current version of SDL that we're using
	SDL_VERSION(&WindowInfo->version);

	//aftere we have the version SDL will allows us to assign the rest of the info using an SDL function
	SDL_GetWindowWMInfo(Window, WindowInfo);
}

WindowMenu::~WindowMenu()
{
	//set window to nullptr but don't delete as the Game.cpp handles that
	Window = nullptr;

	//delete the window info object
	delete WindowInfo;
	WindowInfo = nullptr;
}

void WindowMenu::CreateMenuFromResource()
{
	//we need to convert our SDL window info into a system menu
	HWND SysWindow = WindowInfo->info.win.window;
	//we need to get the resource menu that we created in visual studio
	//GetModuleHandle(0) - get the current instance of the window that we are actively in
	HINSTANCE WindowInstance = GetModuleHandle(0);
	//MAKEINTRESOURCE will convert the IDR_MENU1 into a LPCWSTR which has the index for the game
	LPCWSTR VSMenu = MAKEINTRESOURCE(IDR_MENU1);
	//Load the menu selected to the active window
	HMENU SysMenu = LoadMenu(WindowInstance, VSMenu);

	//this adds the menu to the window
	//if it fails, then error log and stop the function
	if (!SetMenu(SysWindow, SysMenu)) {
		std::cout << "ERROR : Menu Couldn't be Set" << std::endl;
		return;
	}

	//if the menu was successfully added then activate the listener for SDL Events
	SDL_EventState(SDL_SYSWMEVENT, SDL_ENABLE);
}

void WindowMenu::RestartGame()
{
	//run the restart game function built into the game logic
	Game::GetGameInstance().RestartGame();
}

void WindowMenu::ExitApp()
{
	//run the close app function built into the game logic
	Game::GetGameInstance().CloseApp();
}

void WindowMenu::ActivatePopup(const char* Title, const char* Message)
{
	//create a message box that will show on screen with the title and message
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, Title, Message, Window);
}

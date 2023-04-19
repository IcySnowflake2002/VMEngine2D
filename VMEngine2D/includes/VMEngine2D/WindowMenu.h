#pragma once

struct SDL_Window;
struct SDL_SysWMinfo;

class WindowMenu {
public:
	WindowMenu(SDL_Window* Window);
	~WindowMenu();

	//Create the menu and add it to the window
	void CreateMenuFromResource();
	//Menu Options
	void RestartGame(); //restart the game without closing the app
	void ExitApp(); //close the app completely
	void ActivatePopup(const char* Title, const char* Message); //load a dialog box with a message

private:
	//SDL window for the menu 
	SDL_Window* Window;
	SDL_SysWMinfo* WindowInfo;
};
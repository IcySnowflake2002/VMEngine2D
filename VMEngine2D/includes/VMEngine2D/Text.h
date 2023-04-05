#pragma once
#include "sdl2/SDL.h"
#include "VMEngine2D/Vector2.h"
#include <iostream>
#include <string>

// structure to simplify the functions parameters and outputs
struct STTextInfo {
	Vector2 Position;
	Vector2 Dimensions;
	SDL_Color Colour { 255, 255, 255, 255 };
	int Size = 24;
	std::string TextStr = "";
};

class Text {
public:
	Text(SDL_Renderer* Renderer);
	~Text();

	// initialise and create the font using the font supplied in the font path
	// use the text info to position, colour, size and set the text
	// Dimensions will be set based on the text input
	//will return nullptr if it fails
	Text* InitText(const char* FontPath, STTextInfo TextInfo);

	void Draw(SDL_Renderer* Renderer);

	// change the text in realtime
	void SetText(std::string NewText);

	//move the position of the text on the screen
	void SetTextPosition(Vector2 Position);

	//get the important text information
	STTextInfo GetTextInfo() const { return TextInfo; }

private:
	//store all the important text information
	STTextInfo TextInfo;
	//store the renderer
	SDL_Renderer* TextRenderer;
	//store the texture
	SDL_Texture* TextTexture;
	//store the font path from resetting the text
	const char* FontPath;
};
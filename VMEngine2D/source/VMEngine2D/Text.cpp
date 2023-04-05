#include "VMEngine2D/Text.h"
#include "sdl2/SDL_ttf.h"

Text::Text(SDL_Renderer* Renderer)
{
	TextRenderer = Renderer;
	TextTexture = nullptr;
}

Text::~Text()
{
	if (TextTexture != nullptr) {
		SDL_DestroyTexture(TextTexture);
	}
	TextRenderer = nullptr;
}

Text* Text::InitText(const char* FontPath, STTextInfo TextInfo)
{
	//if there is already a texture set then destroy it to make room for a new one
	if (TextTexture != nullptr) {
		SDL_DestroyTexture(TextTexture);
		TextTexture = nullptr;
	}

	// import the font
	TTF_Font* Font = TTF_OpenFont(FontPath, TextInfo.Size);

	//error if the font can't load
	if (Font == nullptr) {
		std::cout << "Couldn't load font at: " << FontPath << " - " << TTF_GetError() << std::endl;
		return nullptr;
	}

	//create a surface from the font file
	SDL_Surface* TextSurface = TTF_RenderText_Solid(Font, TextInfo.TextStr.c_str(), TextInfo.Colour);

	//error if the surface didn't work
	if (TextSurface == nullptr) {
		std::cout << "Couldn't create surface for: " << FontPath << " - " << TTF_GetError() << std::endl;
		return nullptr;
	}

	//convert the surface into a texture
	TextTexture = SDL_CreateTextureFromSurface(TextRenderer, TextSurface);

	//fill in and save the text info
	this->TextInfo = TextInfo;
	this->FontPath = FontPath;

	// now that we know everything worked we can get the surface dimensions and save them
	this->TextInfo.Dimensions = Vector2(static_cast<float>(TextSurface->w), static_cast<float>(TextSurface->h));
	
	//clear the surface from memory
	SDL_FreeSurface(TextSurface);

	return this;
}

void Text::Draw(SDL_Renderer* Renderer)
{
	SDL_FRect Dest; 

	//Create a SDL rectangle for SDL to know how to render the text
	Dest.x = TextInfo.Position.x;
	Dest.y = TextInfo.Position.y;
	Dest.w = TextInfo.Dimensions.x;
	Dest.h = TextInfo.Dimensions.y;

	//render the text to the screen using the above rectangle
	SDL_RenderCopyExF(Renderer, TextTexture, NULL, &Dest, 0.0, NULL, SDL_RendererFlip::SDL_FLIP_NONE);
	

}

void Text::SetText(std::string NewText)
{
	//if the text isn't different from the current text then don't run the function
	if (TextInfo.TextStr == NewText) {
		return;
	}
	
	//Update the text in the class
	TextInfo.TextStr = NewText;
	//intialise a new text font
	InitText(FontPath, TextInfo);

}

void Text::SetTextPosition(Vector2 Position)
{
	//update the text info position that is drawn each frame with the new position
	TextInfo.Position = Position;
}

#pragma once

#include "VMEngine2D/GameState.h"

class SplashState :
	public GameState {
public:
	SplashState(SDL_Window* Window, SDL_Renderer* Renderer);

	virtual	void BeginState() override;

	void ProcessInput(Input* PlayerInput) override;

	void Update(float DeltaTime) override;

	void Draw(SDL_Renderer* Renderer) override;

	virtual void EndState() override;

protected:
	//Display the Title of the game
	Text* SplashText;

	//Display the creator of the game
	Text* AuthorText;

	//Display the START button text
	Text* StartText;
};
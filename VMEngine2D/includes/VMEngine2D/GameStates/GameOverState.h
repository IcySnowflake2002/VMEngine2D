#pragma once

#include "VMEngine2D/GameState.h"
#include "sdl2/SDL_mixer.h"

class GameOverState :
	public GameState {
public:
	GameOverState(SDL_Window* Window, SDL_Renderer* Renderer);

	virtual	void BeginState() override;

	void ProcessInput(Input* PlayerInput) override;

	void Update(float DeltaTime) override;

	void Draw(SDL_Renderer* Renderer) override;

	virtual void EndState() override;

protected:
	//store the text for the score
	Text* ScoreText;

	//store the text for restart
	Text* RestartText;

	//store the test for heading
	Text* GOText;

	//store the audio
	Mix_Music* EGM;
};
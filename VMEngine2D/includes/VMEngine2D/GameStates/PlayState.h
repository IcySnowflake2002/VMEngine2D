#pragma once
#include "VMEngine2D/GameState.h"
#include "sdl2/SDL_mixer.h"

class Text;
class PlayerChar;

class PlayState :
	public GameState {
public:
	PlayState(SDL_Window* Window, SDL_Renderer* Renderer);

	virtual	void BeginState() override;

	void ProcessInput(Input* PlayerInput) override;

	void Update(float DeltaTime) override;

	void Draw(SDL_Renderer* Renderer) override;

	virtual void EndState() override;

protected:
	//store the text for the score
	Text* ScoreText;

	//store the text for the Lives
	Text* LivesText;

	//enemy spawn timer
	double SpawnTimer;
	
	//Rare spawn timer
	double SpawnTimerRare;

	//time to spawn basic enemies
	double SpawnTime;

	//Time to spawn rare enemies
	double SpawnTimeRare;

	//collectible spawn timer
	double CollectTimer;

	//time to spawn Collectibles
	double CollectTime;

	//Store the player character
	PlayerChar* Player;

	//store the audio
	Mix_Music* BGM;
};
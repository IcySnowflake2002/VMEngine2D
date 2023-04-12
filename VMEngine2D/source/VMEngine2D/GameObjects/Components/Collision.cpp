#include "VMEngine2D/GameObjects/Components/Collision.h"
#include "VMEngine2D/GameObject.h"
#include "VMEngine2D/Game.h"
#include "VMEngine2D/GameState.h"

Collision::Collision(GameObject* OwnerObject) : Components(OwnerObject)
{
	//Assign the current game state to the collision component
	AttatchedGameState = Game::GetGameInstance().GetGameStates()->GetCurrentState();
}

Collision::~Collision()
{
	//remove self from game when deleted
	AttatchedGameState->RemoveCollisionFromGameState(this);

	//loop through all the collisions we are overlapping
	for (Collision* OtherCol : OverlappedCollisions) {
		//remove self from the overlapped collisions - overlapped collisions vector
		OtherCol->RemoveCollisionFromOverlapped(this);
	}
}

void Collision::Update()
{
	//run the parent class update
	Components::Update();

	//follow the position of the parent game object
	Dimensions.Position = OwnerObject->Position;

	for (Collision* OtherCol : AttatchedGameState->GetGameStateCollisions()) {
		// skip this collision
		if (OtherCol == this) {
			continue; //continue will skip to the next item in the array
		}

		//Convert Other Collider into an SDL_FRect
		SDL_FRect OtherRect;
		OtherRect.x = OtherCol->Dimensions.Position.x;
		OtherRect.y = OtherCol->Dimensions.Position.y;
		OtherRect.w = OtherCol->Dimensions.Width;
		OtherRect.h = OtherCol->Dimensions.Height;

		//Convert this collision into an SDL_FRect
		SDL_FRect ColRect;
		ColRect.x = Dimensions.Position.x;
		ColRect.y = Dimensions.Position.y;
		ColRect.w = Dimensions.Width;
		ColRect.h = Dimensions.Height;

		//Check if the collider is in the overlapped collisions
		std::vector<Collision*>::iterator It = std::find(
			OverlappedCollisions.begin(), OverlappedCollisions.end(), 
			OtherCol);

		//check if this collision is intersecting with the other collision
		if (SDL_HasIntersectionF(&ColRect, &OtherRect)) {
			//if the other collection is not in overlapped collisions
			if (It == OverlappedCollisions.end()) {
				//Add the object into OverlappedCollisions
				OverlappedCollisions.push_back(OtherCol);
			}
		}
		else {

			//if the collision is in the overlapped collisions
			if (It != OverlappedCollisions.end()) {
				//remove it
				OverlappedCollisions.erase(It);
			}
		}
	}
}


bool Collision::IsOverlappingAny()
{
	return OverlappedCollisions.size() > 0;
}

bool Collision::IsOverlappingTag(const char* Tag)
{
	//loop through all the overlapped collisions
	for (Collision* OverCol : OverlappedCollisions) {
		if (OverCol->GetOwner()->GetTag() == Tag) {
			return true;
		}
	}

	//if none of the collisions have the Tag then exit the function with false
	return false;
}

std::vector<Collision*> Collision::GetOverlappedByTag(const char* Tag)
{
	std::vector<Collision*> TaggedCollisions;

	//loop through all the overlapped collisions
	for (Collision* OverCol : OverlappedCollisions) {
		//if it has the tag
		if (OverCol->GetOwner()->GetTag() == Tag) {
			//add the collider into the local vector
			TaggedCollisions.push_back(OverCol);
		}
	}

	return TaggedCollisions;
}

void Collision::RemoveCollisionFromOverlapped(Collision* Collision)
{
	if (OverlappedCollisions.size() <= 0) {
		return;
	}

	//erase the component from the overlapped collisions and resize the array
	OverlappedCollisions.erase(
		//this is like a find iterator function
		std::remove(OverlappedCollisions.begin(), OverlappedCollisions.end(), Collision),
		//this sets the new size of the array
		OverlappedCollisions.end()
	);
}

void Collision::OnActivated()
{
	//add itself into the game collision array
	AttatchedGameState->AddCollisionToGameState(this);
	std::cout << "Collision Activated" << std::endl;
}


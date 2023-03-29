#include "VMEngine2D/GameObjects/Components.h"
#include "VMEngine2D/GameObject.h"

Components::Components(GameObject* OwnerToAttatch)
{
	bHasActivated = false;

	//set the gameobject that owns this component and save it
	OwnerObject = OwnerToAttatch;

	//attach this component to the owner gameobject
	OwnerObject->AddComponent(this);
}

Components::~Components() 
{
	if (OwnerObject != nullptr) {
		OwnerObject = nullptr;
	}
}

void Components::Update()
{
	//check if this components has been activated
	//if not then set it to activated and run the activated function
	//this will only run once per spawn
	if (!bHasActivated) {
		bHasActivated = true;
		OnActivated();
	}
}

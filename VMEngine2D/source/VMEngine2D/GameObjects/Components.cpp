#include "VMEngine2D/GameObjects/Components.h"
#include "VMEngine2D/GameObject.h"

Components::Components(GameObject* OwnerToAttatch)
{
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
#include "VMEngine2D/GameObject.h"
#include "VMEngine2D/GameObjects/Components.h"
#include <iostream>

GameObject::GameObject()
{
	Scale = 0.1f;
	Rotation = 0.0;

	std::cout << "GameObject created." << std::endl;
}

GameObject::~GameObject()
{
	// delete all the components from memory
	for (Components* SingleComponent : AllComponents) {
		delete SingleComponent;
	}
	
	//resize the array
	AllComponents.clear();

	std::cout << "GameObject Destroyed." << std::endl;
}

void GameObject::Update()
{
	//update all component logicfor each component attached to the gameobject
	for (Components* SingleComponent : AllComponents) {
		SingleComponent->Update();
	}
}

void GameObject::AddComponent(Components* NewComponent)
{
	// make sure the component is not null
	if (NewComponent != nullptr) {
		//add it to the stack
		AllComponents.push_back(NewComponent);
	}
	else {
		//error log
		std::cout << "Attempted to add nullptr component to the component stack." << std::endl;
	}
}

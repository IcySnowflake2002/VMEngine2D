#include "VMEngine2D/GameObject.h"
#include <iostream>

GameObject::GameObject()
{
	Scale = 0.1f;

	std::cout << "GameObject created." << std::endl;
}

GameObject::~GameObject()
{
	std::cout << "GameObject Destroyed." << std::endl;
}

void GameObject::Update()
{
}

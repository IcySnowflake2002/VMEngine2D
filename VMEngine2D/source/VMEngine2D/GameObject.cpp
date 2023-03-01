#include "VMEngine2D/GameObject.h"
#include <iostream>

GameObject::GameObject()
{
	std::cout << "GameObject created." << std::endl;
}

GameObject::~GameObject()
{
	std::cout << "GameObject Destroyed." << std::endl;
}

void GameObject::Update()
{
}

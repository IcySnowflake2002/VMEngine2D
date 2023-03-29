#pragma once

class GameObject;

class Components {
public:
	Components(GameObject* OwnerToAttach);
	virtual ~Components();

	//contain all logic for the components
	virtual void Update();

	//other classes to test the owner
	GameObject* GetOwner() const { return OwnerObject; }
protected:
	//store the owner object
	GameObject* OwnerObject;
	
	// run on the first update
	virtual void OnActivated() {}
private:
	// decide if the component update should run
	bool bHasActivated;
};
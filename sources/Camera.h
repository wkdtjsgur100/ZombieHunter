#pragma once

#include <list>
#include "MoveObjectByCamera.h"
#include "ScoreManager.h"

#define NULL 0

using namespace std;

class Camera
{
	list<MoveObjectByCamera*> camObjects;
	list<MoveObjectByCamera*> deadObjects;
	static Camera* camInstance;
private:
	Camera()
	{
	}
public:
	void release();
	static Camera* sharedCameraInstance();
	void addObject(MoveObjectByCamera* obj);
	void move(float deltaX);
	void remove(MoveObjectByCamera* removeObj);
};


#include "Camera.h"

Camera* Camera::camInstance = 0;

Camera* Camera::sharedCameraInstance()
{
	if(camInstance == 0)
		camInstance = new Camera();
	return camInstance;
}
void Camera::release()
{
	deadObjects.clear();

	camObjects.clear();

	if(camInstance != NULL)
	{
		delete camInstance;
		camInstance = NULL;
	}
}
void Camera::addObject(MoveObjectByCamera* obj)
{
	camObjects.push_back(obj);
}
void Camera::move(float deltaX)
{
	list<MoveObjectByCamera*>::iterator it;

	for(it = deadObjects.begin();it!=deadObjects.end();it++)
	{
		camObjects.remove((*it));
		(*it) = 0;
	}

	deadObjects.clear();

	for(it = camObjects.begin(); it != camObjects.end(); it++)
		(*it)->moveByCamera(-deltaX);
}
void Camera::remove(MoveObjectByCamera* removeObj)
{
	deadObjects.push_back(removeObj);
	removeObj = 0;
}

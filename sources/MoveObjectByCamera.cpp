#include "MoveObjectByCamera.h"
#include "Camera.h"

MoveObjectByCamera::MoveObjectByCamera(void)
{
	Camera::sharedCameraInstance()->addObject(this);
}
MoveObjectByCamera::~MoveObjectByCamera(void)
{
	Camera::sharedCameraInstance()->remove(this);
}
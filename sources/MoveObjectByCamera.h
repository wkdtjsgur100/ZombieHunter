#pragma once

class MoveObjectByCamera
{
public:
	MoveObjectByCamera(void);
	~MoveObjectByCamera(void);
	//카메라에 따라서 dX만큼 움직임. 카메라에 영향을 받는 body나 texture의 좌표를 dX만큼 더해주면 됨.
	virtual void moveByCamera(float dX) = 0;
};


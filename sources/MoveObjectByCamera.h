#pragma once

class MoveObjectByCamera
{
public:
	MoveObjectByCamera(void);
	~MoveObjectByCamera(void);
	//ī�޶� ���� dX��ŭ ������. ī�޶� ������ �޴� body�� texture�� ��ǥ�� dX��ŭ �����ָ� ��.
	virtual void moveByCamera(float dX) = 0;
};


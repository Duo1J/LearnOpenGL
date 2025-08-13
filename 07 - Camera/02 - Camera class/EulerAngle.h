#pragma once

/**
* ŷ����
*/
struct EulerAngle
{
public:
	/**
	* Pitch��
	*/
	float pitch = 0;

	/**
	* Yaw��
	*/
	float yaw = 0;

	/**
	* Roll��
	*/
	float roll = 0;

	EulerAngle() : EulerAngle(0, 0, 0)
	{
	}

	explicit EulerAngle(float _pitch, float _yaw, float _roll)
		: pitch(_pitch), yaw(_yaw), roll(_roll)
	{
	}
};
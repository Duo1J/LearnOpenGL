#pragma once

/**
* 킹윗실
*/
struct EulerAngle
{
public:
	/**
	* Pitch실
	*/
	float pitch = 0;

	/**
	* Yaw실
	*/
	float yaw = 0;

	/**
	* Roll실
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
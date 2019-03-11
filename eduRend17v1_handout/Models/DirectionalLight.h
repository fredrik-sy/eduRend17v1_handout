#pragma once

#include "source/vec/mat.h"

using namespace linalg;

class DirectionalLight
{
public:
	DirectionalLight();
	virtual ~DirectionalLight();

	void Move(float VelocityX, float VelocityY, float VelocityZ);
	void Rotate(float Pitch, float Yaw, float Roll);
	mat4f GetViewToWorldMatrix();
	mat4f GetWorldToViewMatrix();
	mat4f GetProjectionMatrix();
	inline vec3f GetPosition();
	inline void SetPosition(float x, float y, float z);

private:
	float m_Left;
	float m_Right;
	float m_Bottom;
	float m_Top;
	float m_zFar;
	float m_zNear;
	vec3f m_Orientation;
	vec3f m_Position;

};


inline void DirectionalLight::SetPosition(float x, float y, float z)
{
	m_Position.x = x;
	m_Position.y = y;
	m_Position.z = z;
}


inline vec3f DirectionalLight::GetPosition()
{
	return m_Position;
}

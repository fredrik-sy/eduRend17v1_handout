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
	void Update(float DeltaTime);
	inline vec3f GetPosition();
	inline void SetPosition(float x, float y, float z);
	inline void SetVelocity(float x, float y, float z);

private:
	float m_Width;
	float m_Height;
	float m_zFar;
	float m_zNear;
	vec3f m_Orientation;
	vec3f m_Position;
	vec3f m_Velocity;

};


inline void DirectionalLight::SetPosition(float x, float y, float z)
{
	m_Position.x = x;
	m_Position.y = y;
	m_Position.z = z;
}

inline void DirectionalLight::SetVelocity(float x, float y, float z)
{
	m_Velocity.x = x;
	m_Velocity.y = y;
	m_Velocity.z = z;
}


inline vec3f DirectionalLight::GetPosition()
{
	return m_Position;
}

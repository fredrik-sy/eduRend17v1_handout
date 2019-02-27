#pragma once

#include "stdafx.h"
#include "source/vec/mat.h"
#include <DirectXMath.h>

using namespace DirectX;
using namespace linalg;

class Camera
{
public:
	Camera();
	virtual ~Camera();

	void Move(float VelocityX, float VelocityY, float VelocityZ);
	void Rotate(float Pitch, float Yaw, float Roll);
	mat4f GetViewToWorldMatrixA();
	mat4f GetWorldToViewMatrixA();
	mat4f GetProjectionMatrixA();
	inline vec3f GetPosition();
	inline void SetAspectRatio(float AspectRatio);
	inline void SetPosition(float x, float y, float z);

private:
	float m_AspectRatio;
	float m_FOV;
	float m_zFar;
	float m_zNear;
	vec3f m_Orientation;
	vec3f m_Position;

};


inline void Camera::SetAspectRatio(float AspectRatio)
{
	m_AspectRatio = AspectRatio;
}


inline void Camera::SetPosition(float x, float y, float z)
{
	m_Position.x = x;
	m_Position.y = y;
	m_Position.z = z;
}


inline vec3f Camera::GetPosition()
{
	return m_Position;
}
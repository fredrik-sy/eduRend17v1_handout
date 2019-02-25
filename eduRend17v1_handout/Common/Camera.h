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

	void Move(XMVECTOR& Velocity);
	void Rotate(float Pitch, float Yaw, float Roll);
	XMMATRIX GetViewToWorldMatrix();
	XMMATRIX GetWorldToViewMatrix();
	XMMATRIX GetProjectionMatrix();
	inline void SetAspectRatio(float AspectRatio);
	inline void SetPosition(float x, float y, float z);

private:
	float m_AspectRatio;
	float m_FOV;
	float m_zFar;
	float m_zNear;
	XMFLOAT3A m_Position;
	XMFLOAT3A m_Orientation;


	////////////////////////////////////////////////////////////////////////////////////////////////////


public:
	void Move(float VelocityX, float VelocityY, float VelocityZ);
	mat4f GetViewToWorldMatrixA();
	mat4f GetWorldToViewMatrixA();
	mat4f GetProjectionMatrixA();
	inline vec3f GetPosition();

private:
	vec3f m_PositionA;

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


////////////////////////////////////////////////////////////////////////////////////////////////////


inline vec3f Camera::GetPosition()
{
	return m_PositionA;
}
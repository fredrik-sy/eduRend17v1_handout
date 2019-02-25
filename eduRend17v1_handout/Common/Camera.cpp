#include "Camera.h"
#include <math.h>


Camera::Camera()
	: m_FOV((float)M_PI / 4.0f),
	m_zNear(1.0f),
	m_zFar(500.0f),
	m_Orientation(0.0f, 0.0f, 0.0f)
{
}


Camera::~Camera()
{
}


void Camera::Move(DirectX::XMVECTOR& Velocity)
{
	XMFLOAT3A Temp;
	XMStoreFloat3A(&Temp, XMVector3Transform(Velocity, GetViewToWorldMatrix()));
	m_Position.x += Temp.x;
	m_Position.y += Temp.y;
	m_Position.z += Temp.z;
}


void Camera::Rotate(float Pitch, float Yaw, float Roll)
{
	m_Orientation.x += Pitch;
	m_Orientation.y += Yaw;
	m_Orientation.z += Roll;
}


XMMATRIX Camera::GetViewToWorldMatrix()
{
	return XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z)
		* XMMatrixRotationRollPitchYaw(m_Orientation.x, m_Orientation.y, 0.0f);
}


XMMATRIX Camera::GetWorldToViewMatrix()
{
	return XMMatrixTranspose(XMMatrixRotationRollPitchYaw(m_Orientation.x, m_Orientation.y, 0.0f))
		* XMMatrixTranslation(-m_Position.x, -m_Position.y, -m_Position.z);
}


XMMATRIX Camera::GetProjectionMatrix()
{
	const float zRange = m_zNear - m_zFar;
	const float tanHalfFOV = tanf(m_FOV / 2.0f);

	float m11 = 1.0f / (tanHalfFOV * m_AspectRatio);
	float m21 = 0.0f;
	float m31 = 0.0f;
	float m41 = 0.0f;

	float m12 = 0.0f;
	float m22 = 1.0f / tanHalfFOV;
	float m32 = 0.0f;
	float m42 = 0.0f;

	float m13 = 0.0f;
	float m23 = 0.0f;
	float m33 = (-m_zNear - m_zFar) / zRange;
	float m43 = (2.0f * m_zFar * m_zNear) / zRange;

	float m14 = 0.0f;
	float m24 = 0.0f;
	float m34 = 1.0f;
	float m44 = 0.0f;

	XMMATRIX ProjectionMatrix(
		m11, m12, m13, m14,
		m21, m22, m23, m24,
		m31, m32, m33, m34,
		m41, m42, m43, m44);

	return ProjectionMatrix;
}


////////////////////////////////////////////////////////////////////////////////////////////////////


void Camera::Move(float VelocityX, float VelocityY, float VelocityZ)
{
	vec4f Velocity = GetViewToWorldMatrixA() * vec4f(VelocityX, VelocityY, VelocityZ, 0.0f);
	m_PositionA += Velocity.xyz();
}


mat4f Camera::GetViewToWorldMatrixA()
{
	return mat4f::translation(m_PositionA) * mat4f::rotation(0, m_Orientation.x, m_Orientation.y);
}


mat4f Camera::GetWorldToViewMatrixA()
{
	mat4f rotation = mat4f::rotation(0, m_Orientation.x, m_Orientation.y);
	rotation.transpose();
	return rotation * mat4f::translation(-m_PositionA);
}


mat4f Camera::GetProjectionMatrixA()
{
	return mat4f::projection(m_FOV, m_AspectRatio, m_zNear, m_zFar);
}

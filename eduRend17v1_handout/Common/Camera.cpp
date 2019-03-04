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


void Camera::Rotate(float Pitch, float Yaw, float Roll)
{
	m_Orientation.x += Pitch;
	m_Orientation.y += Yaw;
	m_Orientation.z += Roll;
}


void Camera::Move(float VelocityX, float VelocityY, float VelocityZ)
{
	vec4f Velocity = GetViewToWorldMatrix() * vec4f(VelocityX, VelocityY, VelocityZ, 0.0f);
	m_Position += Velocity.xyz();
}


mat4f Camera::GetViewToWorldMatrix()
{
	return mat4f::translation(m_Position) * mat4f::rotation(0, m_Orientation.x, m_Orientation.y);
}


mat4f Camera::GetWorldToViewMatrix()
{
	mat4f rotation = mat4f::rotation(0, m_Orientation.x, m_Orientation.y);
	rotation.transpose();
	return rotation * mat4f::translation(-m_Position);
}


mat4f Camera::GetProjectionMatrix()
{
	return mat4f::projection(m_FOV, m_AspectRatio, m_zNear, m_zFar);
}

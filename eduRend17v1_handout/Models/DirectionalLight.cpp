#include "DirectionalLight.h"


DirectionalLight::DirectionalLight()
	:
	m_Left(-1.0f),
	m_Right(1.0f),
	m_Bottom(-1.0f),
	m_Top(1.0f),
	m_zNear(1.0f),
	m_zFar(10.0f)
{
}


DirectionalLight::~DirectionalLight()
{
}


void DirectionalLight::Move(float VelocityX, float VelocityY, float VelocityZ)
{
	vec4f Velocity = GetViewToWorldMatrix() * vec4f(VelocityX, VelocityY, VelocityZ, 0.0f);
	m_Position += Velocity.xyz();
}


void DirectionalLight::Rotate(float Pitch, float Yaw, float Roll)
{
	m_Orientation.x += Pitch;
	m_Orientation.y += Yaw;
	m_Orientation.z += Roll;
}


mat4f DirectionalLight::GetViewToWorldMatrix()
{
	return mat4f::translation(m_Position) * mat4f::rotation(0, m_Orientation.x, m_Orientation.y);
}


mat4f DirectionalLight::GetWorldToViewMatrix()
{
	mat4f rotation = mat4f::rotation(0, m_Orientation.x, m_Orientation.y);
	rotation.transpose();
	return rotation * mat4f::translation(-m_Position);
}


mat4f DirectionalLight::GetProjectionMatrix()
{
	return mat4f(
		2 / (m_Right - m_Left), 0.0f, 0.0f, 0.0f,
		0.0f, 2 / (m_Top - m_Bottom), 0.0f, 0.0f,
		0.0f, 0.0f, -2 / (m_zFar - m_zNear), 0.0f,
		-(m_Right + m_Left) / (m_Right - m_Left), -(m_Top + m_Bottom) / (m_Top - m_Bottom), -(m_zFar + m_zNear) / (m_zFar - m_zNear), 1.0f);
}

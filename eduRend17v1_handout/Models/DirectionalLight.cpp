#include "DirectionalLight.h"


DirectionalLight::DirectionalLight()
	:
	m_Width(10),
	m_Height(10),
	m_zNear(1),
	m_zFar(10)
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
		2.0f / m_Width, 0.0f, 0.0f, 0.0f,
		0.0f, 2.0f / m_Height, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f / (m_zNear - m_zFar), 0.0f,
		0.0f, 0.0f, m_zNear / (m_zNear - m_zFar), 1.0f);
}

void DirectionalLight::Update(float DeltaTime)
{
	if (m_Position.z > 30)
		m_Velocity.z = -m_Velocity.z;

	if (m_Position.z < -30)
		m_Velocity.z = -m_Velocity.z;

	m_Position += m_Velocity * DeltaTime;
}

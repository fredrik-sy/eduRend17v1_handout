#include "PointLight.h"

#include <exception>

PointLight::PointLight()
	: m_FOV(fPI / 4.0f),
	m_zNear(1.0f),
	m_zFar(500.0f)
{
}


PointLight::~PointLight()
{
}


void PointLight::Move(float VelocityX, float VelocityY, float VelocityZ)
{
	m_Position += vec3f(VelocityX, VelocityY, VelocityZ);
}


mat4f PointLight::GetWorldToViewMatrix(POINT_LIGHT_FACE PointLightFace)
{
	mat4f rotation;

	switch (PointLightFace)
	{
	case POINT_LIGHT_FACE_LEFT:
		break;
	case POINT_LIGHT_FACE_RIGHT:
		break;
	case POINT_LIGHT_FACE_FORWARD:
		rotation = mat4f::rotation(0, 0, 0);
		break;
	case POINT_LIGHT_FACE_BACKWARD:
		rotation = mat4f::rotation(0, fPI, 0);
		break;
	case POINT_LIGHT_FACE_UP:
		break;
	case POINT_LIGHT_FACE_DOWN:
		break;
	default:
		throw std::exception("GetWorldToViewMatrix Failed");
	}

	rotation.transpose();
	return rotation * mat4f::translation(-m_Position);
}


mat4f PointLight::GetProjectionMatrix()
{
	return mat4f::projection(m_FOV, m_AspectRatio, m_zNear, m_zFar);
}

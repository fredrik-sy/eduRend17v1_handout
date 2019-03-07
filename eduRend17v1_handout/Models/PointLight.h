#pragma once

#include "source/vec/mat.h"

using namespace linalg;


typedef enum POINT_LIGHT_FACE
{
	POINT_LIGHT_FACE_LEFT			= 1,
	POINT_LIGHT_FACE_RIGHT			= 2,
	POINT_LIGHT_FACE_FORWARD		= 3,
	POINT_LIGHT_FACE_BACKWARD		= 4,
	POINT_LIGHT_FACE_UP				= 5,
	POINT_LIGHT_FACE_DOWN			= 6
};


class PointLight
{
public:
	PointLight();
	virtual ~PointLight();

	void Move(float VelocityX, float VelocityY, float VelocityZ);
	mat4f GetWorldToViewMatrix(POINT_LIGHT_FACE PointLightFace);
	mat4f GetProjectionMatrix();
	inline vec3f GetPosition();
	inline void SetAspectRatio(float AspectRatio);
	inline void SetPosition(float x, float y, float z);

private:
	float m_AspectRatio;
	float m_FOV;
	float m_zFar;
	float m_zNear;
	vec3f m_Position;

};


inline void PointLight::SetAspectRatio(float AspectRatio)
{
	m_AspectRatio = AspectRatio;
}


inline void PointLight::SetPosition(float x, float y, float z)
{
	m_Position.x = x;
	m_Position.y = y;
	m_Position.z = z;
}


inline vec3f PointLight::GetPosition()
{
	return m_Position;
}

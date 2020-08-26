#ifndef SPACE_H
#define SPACE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <string>
#include "Integrator.hpp"
#include "CelestialBody.h"

typedef glm::dvec3 state_type;

class Space
{
private:
	double StepSize;

	glm::dvec3 ReferenceFramePos;
	const glm::dvec3 InitReferenceFrame = { 0.0, 0.0, 0.0 };

	template<typename RET, typename VEC>
	RET vector_scale(VEC vector)
	{
		return pow(pow(vector.x, 2) + pow(vector.y, 2) + pow(vector.z, 2), 0.5);
	}

public:
	Space(double StepSize);

	void Ephemeris(Planet planet, int TimeStep);//ephemeris: 천체력, 움직임 담당 

	void SetReferencePlanet(Planet ReferencePlanet, int ReferenceFrameMode);

	void CelestialSurfaceFrame(Planet TargetPlanet);//surface frame

	void CelestialCenteredInertialFrame(Planet TargetPlanet);//ECI

	void BarycentreAlignedFrame(Planet TargetPlanet);//EMB

	void CelestialCenteredAlignedFrame(Planet TargetPlanet);//ECSA
};

#endif // !SPACE_H
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
	int ReferenceFrameMode;
	int PlanetMode;

public:
	Space(int ReferenceFrameMode, int PlanetMode);

	void Ephemeris(Planet planet);//ephemeris: 천체력, 움직임 담당 

	void CelestialSurfaceFrame();//surface frame

	void CelestialCenteredInertialFrame();//ECI

	void BarycentreAlignedFrame();//EMB

	void CelestialCenteredAlignedFrame();//ECSA
};

#endif // !SPACE_H


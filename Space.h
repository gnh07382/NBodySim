#ifndef SPACE_H
#define SPACE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <boost/qvm/quat_operations.hpp>

#include <vector>
#include <string>
#include "Integrator.hpp"
#include "CelestialBody.h"

/*

header file explanation

move planet, spacecraft...
INCLUDE: INTEGRATOR_H(integrator), CELESTIALBODY_H(Planet information, move function)

input: std::vector<planet> to ephemeris function, 
auto update planet position, mass

reference frame selection: recalcualte pre-calculated position, work in progress

*/

typedef std::vector<double> state_type;

class Space
{
private:
	double StepSize;

	glm::dvec3 ReferenceFramePos;
	glm::dvec3 ReferenceVector;
	const glm::dvec3 InitReferenceFrame = { 0.0, 0.0, 0.0 };
	bool massinit = false;

	template<typename RET, typename VEC>
	RET vector_scale(VEC vector)
	{
		return pow(pow(vector.x, 2) + pow(vector.y, 2) + pow(vector.z, 2), 0.5);
	}

	std::vector<double> dvec3Tovector(glm::dvec3 vector)
	{
		std::vector<double> temp;
		temp.push_back(vector.x);
		temp.push_back(vector.y);
		temp.push_back(vector.z);

		return temp;
	}
	glm::dvec3 vectorTodvec3(std::vector<double> vec)
	{
		glm::dvec3 temp;
		vec.at(0) = temp.x;
		vec.at(1) = temp.y;
		vec.at(2) = temp.z;

		return temp;
	}

public:
	Space(double StepSize);

	void Ephemeris(int TimeStep, std::vector<Planet>& planet, int planetnum);//ephemeris: 천체력, 움직임 담당 

	void CelestialSurfaceFrame(Planet TargetPlanet, Planet ReferencePlanet);//surface frame

	void CelestialCenteredInertialFrame(Planet TargetPlanet, Planet ReferencePlanet);//ECI

	void BarycentreAlignedFrame(Planet TargetPlanet, Planet ReferencePlanet);//EMB

	void CelestialCenteredAlignedFrame(Planet TargetPlanet, Planet ReferencePlanet);//ECSA
};

#endif // !SPACE_H
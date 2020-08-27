#include "CelestialBody.h"

Planet::Planet(std::string Name, double mass, double EquatorialRotationVel, glm::dvec3 AxialTilt, double Radius, glm::dvec3 InitialPos, glm::dvec3 InitialVel)
	:Name(Name), PlanetMass(mass), EquatorialRotationVel(EquatorialRotationVel), AxialTilt(AxialTilt), Radius(Radius)
{
	PositionPredict.at(0) = InitialPos;
	VelocityPredict.at(0) = InitialVel;
}
void Planet::ParentBodyInfo(Planet parent)
{
	ParentPositionPredict = parent.PositionPredict;
	parent.exist = true;
	parentmass = parent.PlanetMass;
}

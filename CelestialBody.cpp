#include "CelestialBody.h"

Planet::Planet(std::string Name, double mass, double EquatorialRotationVel, glm::dvec3 AxialTilt, double Radius)
	:Name(Name), PlanetMass(mass), EquatorialRotationVel(EquatorialRotationVel), AxialTilt(AxialTilt), Radius(Radius)
{

}
void Planet::InitializeParentBody(std::string ParentBodyName, double ParentBodyMass, std::vector <glm::dvec3> ParentPositionPredict)
{
	this->ParentBodyName = ParentBodyName;
	this->ParentBodyMass = ParentBodyMass;
	this->ParentPositionPredict = ParentPositionPredict;
}
void Planet::ChangeReferenceFrame(int mode)
{
	if (this->ParentBodyName.empty())
	{

	}
}

void Planet::CelestialSurfaceFrame(state_type& x, state_type& d2xdt2, double t)
{
	//³ªÁß¿¡
}

void Planet::CelestialCenteredInertialFrame(state_type& x, state_type& d2xdt2, double t)
{

}

void Planet::BarycentreAlignedFrame(state_type& x, state_type& d2xdt2, double t)
{

}

void Planet::CelestialCenteredAlignedFrame(state_type& x, state_type& d2xdt2, double t)
{

}
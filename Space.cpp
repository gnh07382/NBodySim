#include "Space.h"

Space::Space(double StepSize)
{
	this->StepSize = StepSize;
	ReferenceFramePos = InitReferenceFrame;
}
void Space::Ephemeris(Planet planet, int TimeStep)
{
	Move move;
	SymplecticForestRuth<state_type> symplectic;

	for (int i = 1; i <= TimeStep; i++)
	{
		symplectic.do_step(move, planet.PositionPredict.at(0), planet.VelocityPredict.at(0), move.d2xdt2, StepSize);
		planet.PositionPredict.push_back(planet.PositionPredict.at(i));
		planet.VelocityPredict.push_back(planet.VelocityPredict.at(i));
	}
	planet.calculated = true;
}
void Space::SetReferencePlanet(Planet ReferencePlanet, int ReferenceFrameMode)
{
	if (ReferenceFrameMode==1)
	{

	}
	else if (ReferenceFrameMode==2)
	{
		for (auto pos : ReferencePlanet.PositionPredict)
		{
			ReferenceFramePos = pos;
		}
	}
	else if (ReferencePlanet.parentexist && ReferenceFrameMode==3)
	{
		for (auto pos : ReferencePlanet.PositionPredict)
		{
			ReferenceFramePos = pos;
		}
	}
	else if (ReferencePlanet.parentexist && ReferenceFrameMode == 4)
	{
		for (auto pos : ReferencePlanet.PositionPredict)
		{
			ReferenceFramePos = pos;
		}
	}
	else{}
}
void Space::CelestialSurfaceFrame(Planet TargetPlanet)//referenceframemode = 1
{
	if (TargetPlanet.calculated == true)
	{

	}
}
void Space::CelestialCenteredInertialFrame(Planet TargetPlanet)//referenceframemode = 2
{
	if (TargetPlanet.calculated == true)
	{

	}
}
void Space::BarycentreAlignedFrame(Planet TargetPlanet)//referenceframemode = 3
{
	if (TargetPlanet.calculated == true)
	{

	}
}
void Space::CelestialCenteredAlignedFrame(Planet TargetPlanet)//referenceframemode = 4
{
	if (TargetPlanet.calculated == true)
	{

	}
}
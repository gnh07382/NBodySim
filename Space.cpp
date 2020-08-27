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
void Space::CelestialSurfaceFrame(Planet TargetPlanet, Planet ReferencePlanet)//referenceframemode = 1
{
	if (TargetPlanet.calculated == true)
	{

	}
}
void Space::CelestialCenteredInertialFrame(Planet TargetPlanet, Planet ReferencePlanet)//referenceframemode = 2
{
	if (TargetPlanet.calculated == true)
	{
		for (auto const& i : boost::combine(ReferencePlanet.PositionPredict, TargetPlanet.PositionPredict))
		{
			state_type refpos;
			state_type trgpos;
			boost::tie(refpos, trgpos) = i;

			TargetPlanet.ReferenceFramePositionPredict.push_back(trgpos-refpos);
		}
	}
}
void Space::BarycentreAlignedFrame(Planet TargetPlanet, Planet ReferencePlanet)//referenceframemode = 3
{
	if (TargetPlanet.calculated == true)
	{
		for (auto const& i : boost::combine(ReferencePlanet.PositionPredict, ReferencePlanet.ParentPositionPredict))
		{
			state_type pos;
			state_type parentpos;
			boost::tie(pos, parentpos) = i;

			ReferenceFramePos = ReferencePlanet.parentmass / ReferencePlanet.PlanetMass + ReferencePlanet.parentmass * (pos - parentpos);


		}
	}
}
void Space::CelestialCenteredAlignedFrame(Planet TargetPlanet, Planet ReferencePlanet)//referenceframemode = 4
{
	if (TargetPlanet.calculated == true)
	{
		for (auto const& pos : ReferencePlanet.PositionPredict)
		{
			ReferenceFramePos = pos;
		}
	}
}
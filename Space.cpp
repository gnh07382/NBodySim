#include "Space.h"

Space::Space(double StepSize)
{
	this->StepSize = StepSize;
	ReferenceFramePos = InitReferenceFrame;
}
void Space::Ephemeris(Planet planet, int TimeStep, std::vector<double> masslist, std::vector<glm::dvec3> distlist)
{
	Move move(masslist, distlist);
	SymplecticForestRuth<state_type> symplectic;

	for (int i = 1; i < TimeStep; i++)
	{
		symplectic.do_step(move, planet.PositionPredict.at(i-1), planet.VelocityPredict.at(i-1), move.d2xdt2, StepSize);
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
		for (auto const& i : boost::combine(ReferencePlanet.PositionPredict, TargetPlanet.PositionPredict, ReferencePlanet.VelocityPredict, TargetPlanet.VelocityPredict))
		{
			state_type pos;
			state_type TargetPos;
			state_type vel;
			state_type TargetVel;
			boost::tie(pos, TargetPos, vel, TargetVel) = i;

			ReferenceFramePos = TargetPlanet.PlanetMass / (ReferencePlanet.PlanetMass + TargetPlanet.PlanetMass) * (pos - TargetPos);
			ReferenceVector = vel - TargetVel;

			//일단 기본 출력부터 해보고 나중에 만들어보기
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
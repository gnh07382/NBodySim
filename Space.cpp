#include "Space.h"

Space::Space(double StepSize)
{
	this->StepSize = StepSize;
	ReferenceFramePos = InitReferenceFrame;
}
void Space::Ephemeris(int TimeStep, std::vector<Planet>& planet, int planetnum)
{
	std::vector<double> masslist;
	std::vector<glm::dvec3> distlist;
	std::vector<double> d2xdt2;

	for (auto& const i : planet)
	{
		masslist.push_back(i.PlanetMass);
		distlist.push_back(i.PositionPredict.back());
	}

	SymplecticForestRuth<state_type> symplectic;

	for (int i = 1; i < TimeStep; i++)
	{
		for (auto& const j : planet)
		{
			Move move(masslist, distlist, j);
			state_type planetpospredict = dvec3Tovector(j.PositionPredict.back());
			state_type planetvelpredict = dvec3Tovector(j.VelocityPredict.back());

			symplectic.do_step(move, planetpospredict, planetvelpredict, d2xdt2, StepSize);
			j.PositionPredict.push_back(glm::dvec3(planetpospredict.at(0)));
			j.VelocityPredict.push_back(glm::dvec3(planetvelpredict.at(0)));
		}
		distlist.clear();

		for (auto& const k : planet)
		{
			distlist.push_back(k.PositionPredict.back());
			k.calculated = true;
		}
	}
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

		//	TargetPlanet.ReferenceFramePositionPredict.push_back(trgpos-refpos);
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

		//	ReferenceFramePos = TargetPlanet.PlanetMass / (ReferencePlanet.PlanetMass + TargetPlanet.PlanetMass) * (pos - TargetPos);
		//	ReferenceVector = vel - TargetVel;

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
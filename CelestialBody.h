#ifndef CELESTIALBODY_H
#define CELESTIALBODY_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <string>
#include "Integrator.hpp"

typedef glm::dvec3 state_type;
const double G = 6.67384e-11;

//행성 만들기만 하고 움직이는 건 딴데서 할거임
class Planet 
{
private:
	template<typename RET, typename VEC>
	RET vector_scale(VEC vector)
	{
		return pow(pow(vector.x, 2) + pow(vector.y, 2) + pow(vector.z, 2), 0.5);
	}

	glm::dvec3 ReferenceFramePos;
	const glm::dvec3 InitReferenceFrame = { 0.0, 0.0, 0.0 };

	const double G = 6.67384e-11;

public:
	Planet(std::string Name, double mass, double EquatorialRotationVel, glm::dvec3 AxialTilt, double Radius);
	
	std::vector <glm::dvec3> PositionPredict;
	std::vector <glm::dvec3> VelocityPredict;

	std::string ParentBodyName;
	double ParentBodyMass;
	std::vector <glm::dvec3> ParentPositionPredict;
	void InitializeParentBody(std::string ParentBodyName, double ParentBodyMass, std::vector <glm::dvec3> ParentPositionPredict);

	std::string Name;

	double PlanetMass;
	double EquatorialRotationVel;
	glm::dvec3 AxialTilt;
	double Radius;
	
	/*
	GMm/r2 합 구하는 건 외부(main)에서 해와서 넣어주는거
	넣어주면 인티그레이터로 잘 처리해서 planet 위치 데이터에 넣어주기(멀티코어 사용)
	시간 가는 건 KSP방식이 아니라 CDE방식으로 갈거임(일단 쉬운 CDE방식 먼저 하고 KSP방식으로 확장하겠다는 소리임)
	기준시간도 설정해야 됨 2000/01/01부터 시작해서 가는걸로
	adaptive step size symplectic integrator도 연구해보셈
	*/

	void ChangeReferenceFrame(int mode);

	std::vector<double> masslist;
	template <typename... M>
	void SaveMass(M... Mass)
	{
		masslist.push_back(Mass);
	}

	std::vector<glm::dvec3> distlist;
	template <typename... D>
	void SaveDist(D... Dist)
	{
		distlist.push_back(Dist);
	}
	
	void CelestialSurfaceFrame(state_type& x, state_type& d2xdt2, double t);

	void CelestialCenteredInertialFrame(state_type& x, state_type& d2xdt2, double t);
	
	void BarycentreAlignedFrame(state_type& x, state_type& d2xdt2, double t);

	void CelestialCenteredAlignedFrame(state_type& x, state_type& d2xdt2, double t);
	
	void System(state_type& q)//이걸로 main에서 PositionPredict에 집어넣는 걸 만들거임
	{
		state_type d2xdt2;

		for (auto const& i : boost::combine(masslist, distlist))//공통질량중심 회전 좌표계
		{
			double Mass;
			glm::dvec3 Dist;

			boost::tie(Mass, Dist) = i;

			d2xdt2.x -= G * Mass * PlanetMass * (Dist.x - q.x) / pow(vector_scale<double, glm::dvec3>(Dist - q), 3);
			d2xdt2.y -= G * Mass * PlanetMass * (Dist.y - q.y) / pow(vector_scale<double, glm::dvec3>(Dist - q), 3);
			d2xdt2.z -= G * Mass * PlanetMass * (Dist.z - q.z) / pow(vector_scale<double, glm::dvec3>(Dist - q), 3);
		}
	}
};

#endif // !CELESTIALBODY_H
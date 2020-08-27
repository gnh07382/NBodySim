#ifndef CELESTIALBODY_H
#define CELESTIALBODY_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <string>
#include <boost/range/combine.hpp>
#include <boost/tuple/tuple.hpp>

typedef glm::dvec3 state_type;
const double G = 6.67384e-11;

//행성 만들기만 하고 움직이는 건 딴데서 할거임
class Planet 
{
private:
	const double G = 6.67384e-11;

public:
	Planet(std::string Name, double mass, double EquatorialRotationVel, glm::dvec3 AxialTilt, double Radius, glm::dvec3 InitialPos, glm::dvec3 InitialVel);
	
	std::vector <glm::dvec3> PositionPredict;
	std::vector <glm::dvec3> VelocityPredict;

	std::vector <glm::dvec3> ReferenceFramePositionPredict;

	void ParentBodyInfo(Planet parent);
	std::vector <glm::dvec3> ParentPositionPredict;
	double parentmass;

	std::string Name;

	double PlanetMass;
	double EquatorialRotationVel;
	glm::dvec3 AxialTilt;
	double Radius;

	bool calculated = false;
	bool exist = true;
	bool parentexist;
};

class Move//행성 한 개를 움직이는 클래스
{
private:
	template<typename RET, typename VEC>
	RET vector_scale(VEC vector)
	{
		return pow(pow(vector.x, 2) + pow(vector.y, 2) + pow(vector.z, 2), 0.5);
	}

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

public:
	Move();

	state_type d2xdt2;

	void operator()(state_type& q)//이걸로 main에서 PositionPredict에 집어넣는 걸 만들거임
	{
		for (auto const& i : boost::combine(masslist, distlist))//공통질량중심 회전 좌표계
		{
			int k = 0;
			double Mass;
			state_type Dist;

			boost::tie(Mass, Dist) = i;
			
			for (auto const& Mass2 : masslist) 
			{
				d2xdt2.x -= G * Mass * Mass2 * (Dist.x - q.x) / pow(vector_scale<double, glm::dvec3>(Dist - q), 3);
				d2xdt2.y -= G * Mass * Mass2 * (Dist.y - q.y) / pow(vector_scale<double, glm::dvec3>(Dist - q), 3);
				d2xdt2.z -= G * Mass * Mass2 * (Dist.z - q.z) / pow(vector_scale<double, glm::dvec3>(Dist - q), 3);
			}
		}
	}
};

#endif // !CELESTIALBODY_H
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

//�༺ ����⸸ �ϰ� �����̴� �� ������ �Ұ���
class Planet 
{
private:
	const double G = 6.67384e-11;

public:
	Planet(std::string Name, double mass, double EquatorialRotationVel, glm::dvec3 AxialTilt, double Radius, glm::dvec3 InitialPos, glm::dvec3 InitialVel);
	
	std::vector <glm::dvec3> PositionPredict;
	std::vector <glm::dvec3> VelocityPredict;

	std::vector <glm::dvec3> ReferenceFramePositionPredict;

	//void ParentBodyInfo(Planet parent);
	//std::vector <glm::dvec3> ParentPositionPredict;
	//double parentmass;

	std::string Name;

	double PlanetMass;
	double EquatorialRotationVel;
	glm::dvec3 AxialTilt;
	double Radius;

	bool calculated = false;
	bool exist = true;
	bool parentexist;
};

class Move//�༺ �� ���� �����̴� Ŭ����
{
private:
	template<typename RET, typename VEC>
	RET vector_scale(VEC vector)
	{
		return pow(pow(vector.x, 2) + pow(vector.y, 2) + pow(vector.z, 2), 0.5);
	}

	std::vector<double> masslist;
	std::vector<glm::dvec3> distlist;
	
	std::vector<double> distlist_vector;

	Planet& planet;

public:
	Move(std::vector<double> masslist, std::vector<glm::dvec3> distlist, Planet& planet) : masslist(masslist), distlist(distlist), planet(planet){}

	void operator()(std::vector<double>& q, std::vector<double>& d2xdt2)//�̰ɷ� main���� PositionPredict�� ����ִ� �� �������
	{
		d2xdt2.push_back(0.0);
		d2xdt2.push_back(0.0);
		d2xdt2.push_back(0.0);

		for (auto const& i : boost::combine(masslist, distlist))//���������߽� ȸ�� ��ǥ��
		{
			double Mass;
			glm::dvec3 Dist;
			boost::tie(Mass, Dist) = i;
			
			d2xdt2.at(0) -= G * Mass * planet.PlanetMass * (Dist.x - q.at(0)) / pow(pow(Dist.x+q.at(0), 2)+ pow(Dist.y + q.at(1), 2)+ pow(Dist.z + q.at(2), 2), 0.5);
			d2xdt2.at(1) -= G * Mass * planet.PlanetMass * (Dist.y - q.at(1)) / pow(pow(Dist.x + q.at(0), 2) + pow(Dist.y + q.at(1), 2) + pow(Dist.z + q.at(2), 2), 0.5);
			d2xdt2.at(2) -= G * Mass * planet.PlanetMass * (Dist.z - q.at(2)) / pow(pow(Dist.x + q.at(0), 2) + pow(Dist.y + q.at(1), 2) + pow(Dist.z + q.at(2), 2), 0.5);
		}

		d2xdt2.clear();
	}
};

#endif // !CELESTIALBODY_H
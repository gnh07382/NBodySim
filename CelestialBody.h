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

//�༺ ����⸸ �ϰ� �����̴� �� ������ �Ұ���
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
	GMm/r2 �� ���ϴ� �� �ܺ�(main)���� �ؿͼ� �־��ִ°�
	�־��ָ� ��Ƽ�׷����ͷ� �� ó���ؼ� planet ��ġ �����Ϳ� �־��ֱ�(��Ƽ�ھ� ���)
	�ð� ���� �� KSP����� �ƴ϶� CDE������� ������(�ϴ� ���� CDE��� ���� �ϰ� KSP������� Ȯ���ϰڴٴ� �Ҹ���)
	���ؽð��� �����ؾ� �� 2000/01/01���� �����ؼ� ���°ɷ�
	adaptive step size symplectic integrator�� �����غ���
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
	
	void System(state_type& q)//�̰ɷ� main���� PositionPredict�� ����ִ� �� �������
	{
		state_type d2xdt2;

		for (auto const& i : boost::combine(masslist, distlist))//���������߽� ȸ�� ��ǥ��
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
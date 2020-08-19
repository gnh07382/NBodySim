#ifndef CELESTIALBODY_H
#define CELESTIALBODY_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <string>

//�༺ ����⸸ �ϰ� �����̴� �� ������ �Ұ���
class Planet 
{
public:
	Planet(std::string ParentBody, double mass, double EquatorialRotationVel, glm::dvec3 AxialTilt, double Radius)
		:ParentBody(ParentBody), mass(mass), EquatorialRotationVel(EquatorialRotationVel), AxialTilt(AxialTilt), Radius(Radius)
	{}
	
	std::vector <glm::dvec3> PositionPredict;
	std::vector <glm::dvec3> VelocityPredict;

	std::string ParentBody;

	double mass;
	double EquatorialRotationVel;
	glm::dvec3 AxialTilt;
	double Radius;

private:
	
};


#endif // !CELESTIALBODY_H

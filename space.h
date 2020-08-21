#ifndef SPACE_H
#define SPACE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

#include "Integrator.h"

class Space 
{
private:
	void MovePrediction();//������ �����ϴ� �Լ�, ���� ��ǥ ����
	glm::dvec3 ReferenceFramePos;
	const glm::dvec3 InitReferenceFrame = { 0.0, 0.0, 0.0 };

public:
	Space();//�����Ӹ� ������

	glm::dvec3 OrbitalMove(double DeltaTime);//1�����Ӹ��� �������ִ� �Լ�
	std::vector<glm::dvec3> ReferenceFrameMove();//���۷��� ������ �������� ������
	void ChangeReferenceFrame();
};

#endif // !SPACE_H
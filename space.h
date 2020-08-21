#ifndef SPACE_H
#define SPACE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

#include "Integrator.hpp"

class Space 
{
private:
	glm::dvec3 ReferenceFramePos;
	const glm::dvec3 InitReferenceFrame = { 0.0, 0.0, 0.0 };

	const double G = 6.67384e-11;

public:
	Space();//�����Ӹ� ������

	void CelestialSurfaceFrame();//ECEF
	void CelestialCenteredInertialFrame();//ECI
	void BarycentreFrame();//EMB
	void CelestialCenteredAlignedFrame();//ESCA
	//�̰� �� operator()�� �����ؼ� �־�� ��

	/*
	GMm/r2 �� ���ϴ� �� �ܺ�(main)���� �ؿͼ� �־��ִ°�
	�־��ָ� ��Ƽ�׷����ͷ� �� ó���ؼ� planet ��ġ �����Ϳ� �־��ֱ�(��Ƽ�ھ� ���)
	�ð� ���� �� KSP����� �ƴ϶� CDE������� ������(�ϴ� ���� CDE��� ���� �ϰ� KSP������� Ȯ���ϰڴٴ� �Ҹ���)
	���ؽð��� �����ؾ� �� 2000/01/01���� �����ؼ� ���°ɷ�
	adaptive step size symplectic integrator�� �����غ���
	*/

	glm::dvec3 Move();

	void ChangeReferenceFrame();
};

#endif // !SPACE_H
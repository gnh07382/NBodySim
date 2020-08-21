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
	Space();//움직임만 제어함

	void CelestialSurfaceFrame();//ECEF
	void CelestialCenteredInertialFrame();//ECI
	void BarycentreFrame();//EMB
	void CelestialCenteredAlignedFrame();//ESCA
	//이거 다 operator()로 개조해서 넣어야 됨

	/*
	GMm/r2 합 구하는 건 외부(main)에서 해와서 넣어주는거
	넣어주면 인티그레이터로 잘 처리해서 planet 위치 데이터에 넣어주기(멀티코어 사용)
	시간 가는 건 KSP방식이 아니라 CDE방식으로 갈거임(일단 쉬운 CDE방식 먼저 하고 KSP방식으로 확장하겠다는 소리임)
	기준시간도 설정해야 됨 2000/01/01부터 시작해서 가는걸로
	adaptive step size symplectic integrator도 연구해보셈
	*/

	glm::dvec3 Move();

	void ChangeReferenceFrame();
};

#endif // !SPACE_H
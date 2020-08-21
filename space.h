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
	void MovePrediction();//움직임 예상하는 함수, 절대 좌표 기준
	glm::dvec3 ReferenceFramePos;
	const glm::dvec3 InitReferenceFrame = { 0.0, 0.0, 0.0 };

public:
	Space();//움직임만 제어함

	glm::dvec3 OrbitalMove(double DeltaTime);//1프레임마다 움직여주는 함수
	std::vector<glm::dvec3> ReferenceFrameMove();//레퍼런스 프레임 기준으로 움직임
	void ChangeReferenceFrame();
};

#endif // !SPACE_H
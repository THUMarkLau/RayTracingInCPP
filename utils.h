#pragma once
#include <glm/glm.hpp>
#include <random>
#include <iostream>
using namespace glm;


class Material;
class RayTracer;
class Camera;
/*
 * ��ײ�ļ�¼
 * t -> ��ײ��������̵Ĳ���
 * p -> ��ײ��
 * normal -> ��ײ��ķ�����
 * mat_ptr -> ��ײ��Ĳ���ָ��
 */
struct HitRecord {
	float t;
	vec3 p;
	vec3 normal;
	Material* material;
};

struct Indice {
	unsigned int a, b, c;
};


/*
* ƽ��������̵��ĸ�������ax + by + cz + d = 0
*/
struct PlaneArgs {
	float a, b, c, d;
};

/*
 * �������еĲ����ṹ��
 */
struct ParallelTracingParameter {
	RayTracer* rayTracer;
	Camera* camera;
	int left, right, bottom, top;
};

class ProgramDebugger {
public:
	static bool debug;
};


vec3 random_in_unit_sphere();
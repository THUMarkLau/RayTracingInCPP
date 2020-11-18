#pragma once
#include <glm/glm.hpp>
#include <random>
#include <iostream>
using namespace glm;


class Material;
class RayTracer;
class Camera;
/*
 * 碰撞的记录
 * t -> 碰撞点参数方程的参数
 * p -> 碰撞点
 * normal -> 碰撞点的法向量
 * mat_ptr -> 碰撞点的材料指针
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
* 平面参数方程的四个参数，ax + by + cz + d = 0
*/
struct PlaneArgs {
	float a, b, c, d;
};

/*
 * 并行运行的参数结构体
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
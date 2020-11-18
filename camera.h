#pragma once
#include <glm/glm.hpp>
#include "Ray.h"
using namespace glm;

class Camera {
public:
	vec3 origin; // 观察的原点
	vec3 lower_left_corner; // 左下角
	vec3 horizonal; // 水平方向的向量
	vec3 vertical; // 竖直方向的向量
	Camera(const vec3& ori, const vec3& llc, const vec3& hori, const vec3& vert) {
		origin = ori;
		lower_left_corner = llc;
		horizonal = hori;
		vertical = vert;
	}

	Ray generateRay(float u, float v);
};
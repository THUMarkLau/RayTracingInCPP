#pragma once
#include <glm/glm.hpp>
#include "Ray.h"
using namespace glm;

class Camera {
public:
	vec3 origin; // �۲��ԭ��
	vec3 lower_left_corner; // ���½�
	vec3 horizonal; // ˮƽ���������
	vec3 vertical; // ��ֱ���������
	Camera(const vec3& ori, const vec3& llc, const vec3& hori, const vec3& vert) {
		origin = ori;
		lower_left_corner = llc;
		horizonal = hori;
		vertical = vert;
	}

	Ray generateRay(float u, float v);
};
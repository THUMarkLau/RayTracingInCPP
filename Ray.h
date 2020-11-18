#pragma once
#include <glm/glm.hpp>
using namespace glm;

class Ray {
public:
	Ray();
	Ray(const vec3& a, const vec3& b);
	vec3 origin() const;
	vec3 direction() const;
	vec3 point_at_t(float t) const;

private:
	vec3 ori, direct;
};
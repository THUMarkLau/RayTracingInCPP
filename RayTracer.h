#pragma once
#include <glm/glm.hpp>
#include "Ray.h"
#include "Object.h"
using namespace glm;

class RayTracer {
public:
	RayTracer();
	vec3 trace(const Ray& r);
	void setWorld(ObjectList* world);
	static float T_MAX;
private:
	vec3 trace(const Ray& r, Object* object, int depth);
	ObjectList* world;
};
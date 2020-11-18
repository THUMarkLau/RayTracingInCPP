#include "RayTracer.h"

float RayTracer::T_MAX = 1e9;

RayTracer::RayTracer() {
	world = nullptr;
}

void RayTracer::setWorld(ObjectList* w) {
	world = w;
}

vec3 RayTracer::trace(const Ray& r, Object* object, int depth) {
	HitRecord rec;
	if (object->isHit(r, 0.001, T_MAX, rec)) {
		Ray scattered;
		vec3 attenuation;
		if (depth < RAY_TRACE_DEPTH_LIMIT && rec.material->scatter(r, rec, attenuation, scattered)) {
			// 递归未结束，发生了反射
			return attenuation * trace(scattered, world, depth + 1);
		}
		else {
			return vec3(0);
		}
	}
	else {
		// 未碰撞物体，返回背景色
		vec3 result;
		if (length(r.direction()) != 0) {
			vec3 unit_direction = normalize(r.direction());
			float t = 0.5f * (unit_direction.y + 1.0f);
			result = (1.0f - t) * vec3(1.0) + t * vec3(0.5, 0.7, 1.0);
		}
		else {
			result = 0.5f * vec3(1.0) + 0.5f * vec3(0.5, 0.7, 1.0);
		}
		return result;
	}
}

vec3 RayTracer::trace(const Ray& r) {
	return trace(r, this->world, 0);
}


#include "Ray.h"

Ray::Ray() {

}

Ray::Ray(const vec3& o, const vec3& d) {
	ori = o;
	direct = d;
}

vec3 Ray::origin() const {
	return ori;
}

vec3 Ray::direction() const {
	return direct;
}

vec3 Ray::point_at_t(float t) const {
	return ori + t * direct;
}
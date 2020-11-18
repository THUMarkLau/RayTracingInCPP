#include "camera.h"

Ray Camera::generateRay(float u, float v) {
	return Ray(origin, lower_left_corner + u * horizonal + v * vertical - origin);
}

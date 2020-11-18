#include "utils.h"

std::default_random_engine dre;
std::uniform_real_distribution<float> urd(0, 1);

vec3 random_in_unit_sphere() {
	vec3 p;
	do {
		try {
			p = 2.0f * vec3(urd(dre), urd(dre), urd(dre)) - vec3(1, 1, 1);
		}
		catch (const char* msg) {
			std::cout << msg << std::endl;
			exit(-1);
		}
	} while (dot(p, p) >= 1.0);
	return p;
}

bool ProgramDebugger::debug = false;
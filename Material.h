#pragma once
#include <glm/glm.hpp>
#include "Ray.h"
#include "config.h"
#include "utils.h"
using namespace glm;

class Material {
public:
	virtual bool scatter(const Ray& r, const HitRecord& rec, vec3& attenuation, Ray& scattered) const = 0;
	void setColor(const vec3& c);
protected:
	vec3 reflect(const vec3& v, const vec3& n) const;
	bool refract(const vec3& v, const vec3& n, float ni_over_nt, vec3& refracted) const;
	float schlick(float cosine, float ref_idx) const;
	vec3 color;
};

class Plastic :public Material {
public:
	Plastic();
	virtual bool scatter(const Ray& r, const HitRecord& rec, vec3& attenuation, Ray& scattered) const;
private:
	float albedo;
};

class Metal :public Material {
public:
	Metal();
	virtual bool scatter(const Ray& r, const HitRecord& rec, vec3& attenuation, Ray& scattered) const;
private:
	float albedo;
	float fuzz;
};

class Glass :public Material {
public:
	Glass(float r);
	virtual bool scatter(const Ray& r, const HitRecord& rec, vec3& attenuation, Ray& scattered) const;
private:
	float ref_idx;
	float fuzz;
};
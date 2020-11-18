#include "Material.h"

std::default_random_engine dre_mat;
std::uniform_real_distribution<float> urd_mat(0, 1);

/*
 * 光线的镜面反射，返回镜面反射的方向
 * v -> 入射光线方向
 * n -> 法向量
 */
vec3 Material::reflect(const vec3& v, const vec3& n)const {
	return v - 2 * dot(v, n) * n;
}

bool Material::refract(const vec3& v, const vec3& n, float ni_over_nt, vec3& refracted) const {
	vec3 uv = normalize(v);
	float dt = dot(uv, n);
	float discriminant = 1.0 - ni_over_nt * ni_over_nt * (1 - dt * dt);
	if (discriminant > 0) {
		refracted = ni_over_nt * (uv - n * dt) - n * sqrt(discriminant);
		return true;
	}
	else
		return false;
}

void Material::setColor(const vec3& c) {
	color = c;
}

float Material::schlick(float cosine, float ref_idx) const {
	float r0 = (1 - ref_idx) / (1 + ref_idx);
	r0 = r0 * r0;
	return r0 + (1 - r0) * pow((1 - cosine), 5);
}

Plastic::Plastic() {
	albedo = PLASTIC_ALBEDO;
}


/*
 * 反射函数，获取反射后的光线
 */
bool Plastic::scatter(const Ray& r, const HitRecord& rec, vec3& attenuation, Ray& scattered) const {
	vec3 target = rec.p + rec.normal + random_in_unit_sphere();
	scattered = Ray(rec.p, target - rec.p);
	attenuation = color * albedo;
	return true;
}

Metal::Metal() {
	albedo = METAL_ALBEDO;
	fuzz = METAL_FUZZ;
}

bool Metal::scatter(const Ray& r, const HitRecord& rec, vec3& attenuation, Ray& scattered) const {
	vec3 reflected = reflect(normalize(r.direction()), rec.normal);
	scattered = Ray(rec.p, reflected + fuzz * random_in_unit_sphere());
	attenuation = albedo * color;
	return (dot(scattered.direction(), rec.normal) > 0);
}

Glass::Glass(float r) {
	ref_idx = r;
    fuzz = 0.1;
}

bool Glass::scatter(const Ray& r, const HitRecord& rec, vec3& attenuation, Ray& scattered) const {
    vec3 outward_normal;
    vec3 reflected = reflect(r.direction(), rec.normal);
    float ni_over_nt;
    attenuation = vec3(1.0, 1.0, 1.0);
    vec3 refracted;
    float reflect_prob;
    float cosine;
    if (dot(r.direction(), rec.normal) > 0) {
        outward_normal = -rec.normal;
        ni_over_nt = ref_idx;
        //         cosine = ref_idx * dot(r_in.direction(), rec.normal) / r_in.direction().length();
        cosine = dot(r.direction(), rec.normal) / r.direction().length();
        cosine = sqrt(1 - ref_idx * ref_idx * (1 - cosine * cosine));
    }
    else {
        outward_normal = rec.normal;
        ni_over_nt = 1.0 / ref_idx;
        cosine = -dot(r.direction(), rec.normal) / r.direction().length();
    }
    if (refract(r.direction(), outward_normal, ni_over_nt, refracted))
        reflect_prob = schlick(cosine, ref_idx);
    else
        reflect_prob = 1.0;
    // 随机数小与反射系数，设为反射光线，反之为折射光线
    if (urd_mat(dre_mat) < reflect_prob)
        scattered = Ray(rec.p, reflected);
    else
        scattered = Ray(rec.p, refracted);
    return true;
}
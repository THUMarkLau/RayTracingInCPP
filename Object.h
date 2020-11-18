#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include <vector>
#include <cmath>
#include <iostream>
#include "Ray.h"
#include "utils.h"
#include "Material.h"

class Object {
public:
	virtual bool isHit(const Ray& r, float t_min, float t_max, HitRecord& rec) const = 0;
	void setMaterial(Material* mat);
	Material* getMaterial();
protected:
	Material* material;
	bool inTriangle(const vec3& p, const vec3& a, const vec3& b, const vec3& c) const;
};


class Sphere :public Object {
public:
	Sphere(vec3& center, float radius);
	virtual bool isHit(const Ray& r, float t_min, float t_max, HitRecord& rec) const;
private:
	vec3 center;
	float radius;
	Material* mat;
};


class BoundingBox;

class Model : public Object {
public:
	Model(vec3& center, float magnification=1.5);
	virtual bool isHit(const Ray& r, float t_min, float t_max, HitRecord& rec) const;
	void load(std::string model_path);
	Indice getIndice(int index) const;
	vec3 getNormal(int index) const;
	vec3 getVertice(int index) const;
	PlaneArgs getPlaneArgs(int index) const;
	void buildBox();
private:
	BoundingBox* buildBox(vec3 leftBottomPoint, float width, float height, float length);
	void getPlaneArgs(vec3& p1, vec3& p2, vec3& p3, PlaneArgs& args)const;
	std::vector<vec3> vertices, normals;
	std::vector<Indice> indices;
	std::vector<PlaneArgs> plane_args;
	int num_vertice, num_triangle;
	vec3 center;
	float magnification;
	float max_x, min_x;
	float max_y, min_y;
	float max_z, min_z;
	BoundingBox* box;
};

class BoundingBox : public Object {
public:
	BoundingBox(float x, float y, float z, float width, float height, float length, bool isLeaf);
	virtual bool isHit(const Ray& r, float t_min, float t_max, HitRecord& rec) const;
	bool isHit(const Ray& r, float t_min, float t_max, int &check_num, HitRecord& rec) const;
	void setModel(Model* model);
	void addIndice(int idx);
	void addSubBox(BoundingBox* sub_box);
private:
	bool hitFront(const Ray& r, float t_min, float t_max) const;
	bool hitBack(const Ray& r, float t_min, float t_max) const;
	bool hitLeft(const Ray& r, float t_min, float t_max) const;
	bool hitRight(const Ray& r, float t_min, float t_max) const;
	bool hitTop(const Ray& r, float t_min, float t_max) const;
	bool hitBottom(const Ray& r, float t_min, float t_max) const;
	bool hitFacet(const Ray& r, const vec3& v1, const vec3& v2, const vec3& v3, PlaneArgs& args, float t_min, float t_max, float &t) const;
	float x, y, z;
	float width, height, length;
	std::vector<BoundingBox*> sub_boxes;
	std::vector<int> index;
	bool isLeaf;
	Model* model;
};

class ObjectList :public Object {
public:
	ObjectList();
	virtual bool isHit(const Ray& r, float t_min, float t_max, HitRecord& rec) const;
	void addModel(Object* obj);
private:
	std::vector<Object*> objects;
};
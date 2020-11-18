#include "Object.h"

void Object::setMaterial(Material* mat) {
	material = mat;
}

Material* Object::getMaterial() {
	return material;
}

bool Object::inTriangle(const vec3& p, const vec3& a, const vec3& b, const vec3& c) const {
	auto v0 = c - a, v1 = b - a, v2 = p - a;
	auto dot00 = glm::dot(v0, v0);
	auto dot01 = glm::dot(v0, v1);
	auto dot02 = glm::dot(v0, v2);
	auto dot11 = glm::dot(v1, v1);
	auto dot12 = glm::dot(v1, v2);
	auto invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
	auto u = (dot11 * dot02 - dot01 * dot12) * invDenom;
	auto v = (dot00 * dot12 - dot01 * dot02) * invDenom;

	return u >= 0 and v >= 0 and (u + v) < 1;
}

BoundingBox::BoundingBox(float _x, float _y, float _z, float _width, float _height, float _length, bool isL) {
	x = _x;
	y = _y;
	z = _z;
	width = _width;
	height = _height;
	length = _length;
	isLeaf = isL;
}

void BoundingBox::addSubBox(BoundingBox* sub_box) {
	sub_boxes.push_back(sub_box);
}

bool BoundingBox::isHit(const Ray& r, float t_min, float t_max, HitRecord& rec) const {
	return false;
}

bool BoundingBox::isHit(const Ray& r, float t_min, float t_max, int& check_num, HitRecord& rec) const {
	// 判断是否撞到了包围盒的前方
	check_num = index.size();
	if (hitFront(r, t_min, t_max) || hitBack(r, t_min, t_max)
		|| hitLeft(r, t_min, t_max) || hitRight(r, t_min, t_max)
		|| hitTop(r, t_min, t_max) || hitBottom(r, t_min, t_max)) {
		// 如果不是叶子包围盒，则继续向子包围盒查找
		bool hitAnything = false;
		int total_check_num = 0, temp_check_num = 0;
		if (!isLeaf) {
			for (int i = 0; i < sub_boxes.size(); ++i) {
				hitAnything = hitAnything || sub_boxes[i]->isHit(r, t_min, rec.t, temp_check_num, rec);
				total_check_num += temp_check_num;
			}
		}
		if (hitAnything) {
			return hitAnything;
		}
		// 如果是叶子包围盒，则直接从面片中进行查找
		float t = t_max;
		for (int i = 0; i < index.size(); ++i) {
			int cur_idx = index[i];
			Indice idx = model->getIndice(cur_idx);
			vec3 v1 = model->getVertice(idx.a);
			vec3 v2 = model->getVertice(idx.b);
			vec3 v3 = model->getVertice(idx.c);
			PlaneArgs args = model->getPlaneArgs(index[i]);
			float _t_max = t;
			bool result = hitFacet(r, v1, v2, v3, args, t_min, _t_max, t);
			if (ProgramDebugger::debug && result) {
				std::cout << "Hit bounding box with t = " << t << std::endl;
			}
			if (result) {
				hitAnything = true;
				rec.normal = model->getNormal(index[i]);
				rec.t = t;
				rec.p = r.point_at_t(t);
				rec.material = model->getMaterial();
			}
		}
		/*if (ProgramDebugger::debug && hitAnything) {
			std::cout << "Hit bounding box with t = " << rec.t << std::endl;
		}*/
		return hitAnything;
	}
	return false;
}

bool BoundingBox::hitFront(const Ray& r, float t_min, float t_max) const {
	if (r.direction().z != 0) {
		float t = (z - r.origin().z) / r.direction().z;
		vec3 p = r.point_at_t(t);
		bool result = t >= t_min && t <= t_max && p.x >= x
			&& p.x <= x + width && p.y >= y && p.y <= y + height;
		if (ProgramDebugger::debug && result) {
			std::cout << "hit front" << std::endl;
		}
		return result;
	}
	return false;
}

bool BoundingBox::hitBack(const Ray& r, float t_min, float t_max) const {
	if (r.direction().z != 0) {
		float t = (z + length - r.origin().z) / r.direction().z;
		vec3 p = r.point_at_t(t);
		bool result = t >= t_min && t <= t_max && p.x >= x && 
			p.x <= x + width && p.y >= y && p.y <= y + height;
		if (ProgramDebugger::debug && result) {
			std::cout << "hit back" << std::endl;
		}
		return result;
	}
	return false;
}

bool BoundingBox::hitLeft(const Ray& r, float t_min, float t_max) const {
	if (r.direction().x != 0) {
		float t = (x - r.origin().x) / r.direction().x;
		vec3 p = r.point_at_t(t);
		bool result =  t >= t_min && t <= t_max && p.y >= y
			&& p.y <= y + height && p.z >= z && p.z <= z + length;
		if (ProgramDebugger::debug && result) {
			std::cout << "hit left" << std::endl;
		}
		return result;
	}
	return false;
}

bool BoundingBox::hitRight(const Ray& r, float t_min, float t_max) const {
	if (r.direction().x != 0) {
		float t = (x + width - r.origin().x) / r.direction().x;
		vec3 p = r.point_at_t(t);
		bool result = t >= t_min && t <= t_max && p.y >= y
			&& p.y <= y + height && p.z >= z && p.z <= z + length;
		if (ProgramDebugger::debug && result) {
			std::cout << "hit right" << std::endl;
		}
		return result;
	}
	return false;
}

bool BoundingBox::hitBottom(const Ray& r, float t_min, float t_max) const {
	if (r.direction().y != 0) {
		float t = (y - r.origin().y) / r.direction().y;
		vec3 p = r.point_at_t(t);
		bool result = t >= t_min && t <= t_max && p.x >= x
			&& p.x <= x + width && p.z >= z && p.z <= z + length;
		if (ProgramDebugger::debug && result) {
			std::cout << "hit bottom" << std::endl;
		}
		return result;
	}
	return false;
}

bool BoundingBox::hitTop(const Ray& r, float t_min, float t_max) const {
	if (r.direction().y != 0) {
		float t = (y + height - r.origin().y) / r.direction().y;
		vec3 p = r.point_at_t(t);
		bool result = t >= t_min && t <= t_max && p.x >= x
			&& p.x <= x + width && p.z >= z && p.z <= z + length;
		if (ProgramDebugger::debug && result) {
			std::cout << "hit top" << std::endl;
		}
		return result;
	}
	return false;
}

/*
*	判断光线有没有碰撞到面片
*/
bool BoundingBox::hitFacet(const Ray& r, const vec3& v1, const vec3& v2, const vec3& v3, PlaneArgs& args, float t_min, float t_max, float& t) const {
	float a = args.a, b = args.b, c = args.c, d = args.d;
	vec3 ori = r.origin(), direct = r.direction();
	float _t = (a * ori.x + b * ori.y + c * ori.z + d) / (a * direct.x + b * direct.y + c * direct.z) * (-1.0f);
	if (_t >= t_min && _t < t_max) {
		vec3 p = r.point_at_t(_t);
		if (inTriangle(p, v1, v2, v3)) {
			t = _t;
			if (ProgramDebugger::debug) {
				std::cout << "hit facet with t = " << t << std::endl;
			}
			return true;
		}
	}
	return false;
}

void BoundingBox::setModel(Model* m) {
	model = m;
}

void BoundingBox::addIndice(int idx) {
	index.push_back(idx);
}

Sphere::Sphere(vec3& cen, float r) {
	center = cen;
	radius = r;
}

bool Sphere::isHit(const Ray& r, float t_min, float t_max, HitRecord& rec) const {
	vec3 oc = r.origin() - center;
	float a = dot(r.direction(), r.direction());
	float b = dot(oc, r.direction());
	float c = dot(oc, oc) - radius * radius;
	float discriminant = b * b - a * c;
	if (discriminant > 0) {
		float t = (-b - sqrt(discriminant)) / a;
		if (t < t_max && t > t_min) {
			rec.t = t;
			rec.p = r.point_at_t(t);
			rec.normal = (rec.p - center) / radius;
			rec.material = material;
			return true;
		}
		t = (-b + sqrt(discriminant)) / a;
		if (t < t_max && t > t_min) {
			rec.t = t;
			rec.p = r.point_at_t(t);
			rec.normal = (rec.p - center) / radius;
			rec.material = material;
			return true;
		}
	}
	return false;
}

Model::Model(vec3& c, float m) {
	center = c;
	magnification = m;
}

bool Model::isHit(const Ray& r, float t_min, float t_max, HitRecord& rec) const {
	if (!ACCELERATION) {
		rec.t = 1e9;
		HitRecord temp_rec;
		int indice_cnt = indices.size();
		vec3 ori = r.origin(), direct = r.direction();
		float closest_so_far = t_max;
		bool hit_anything = false;
		for (int i = 0; i < indice_cnt; ++i) {
			Indice idx = indices[i];
			PlaneArgs args = plane_args[i];
			unsigned int idx_a = idx.a, idx_b = idx.b, idx_c = idx.c;
			float a = args.a, b = args.b, c = args.c, d = args.d;
			float t = (a * ori.x + b * ori.y + c * ori.z + d) / (a * direct.x + b * direct.y + c * direct.z) * (-1.0f);
			if (t >= t_min && t <= closest_so_far) {
				vec3 p = r.point_at_t(t);
				if (inTriangle(p, vertices[idx_a], vertices[idx_b], vertices[idx_c])) {
					hit_anything = true;
					temp_rec.normal = normals[i];
					temp_rec.p = p;
					temp_rec.t = t;
					temp_rec.material = material;
					rec = temp_rec;
					closest_so_far = t;
				}
			}
		}
		return hit_anything;
	}
	else {
		int temp;
		bool result = box->isHit(r, t_min, t_max, temp, rec);
		if (ProgramDebugger::debug && result) {
			std::cout << "hit model with t = " << rec.t << std::endl;
		}
		return result;
	}
}

void Model::load(std::string model_path) {
	std::cout << "Loading " << model_path << std::endl;
	FILE* file = fopen(model_path.c_str(), "r");
	if (!file) {
		std::cout << "Cannot open " << model_path << std::endl;
		return;
	}
	fseek(file, 0, SEEK_SET);
	char buffer[1000];
	fgets(buffer, 300, file);

	// 读文件头
	// 获取顶点的总个数
	while (strncmp("element vertex", buffer, strlen("element vertex")) != 0) {
		fgets(buffer, 300, file);
	}
	strcpy(buffer, buffer + strlen("element vertex"));
	sscanf(buffer, "%i", &num_vertice);

	// 获取索引的个数
	fseek(file, 0, SEEK_SET);
	while (strncmp("element face", buffer, strlen("element face")) != 0) {
		fgets(buffer, 300, file);
	}
	strcpy(buffer, buffer + strlen("element face"));
	sscanf(buffer, "%i", &num_triangle);

	// 跳过文件头
	while (strncmp("end_header", buffer, strlen("end_header")) != 0) {
		fgets(buffer, 300, file);
	}
	float x, y, z;
	for (int i = 0; i < num_vertice; i++) {
		fgets(buffer, 300, file);
		sscanf(buffer, "%f %f %f", &x, &y, &z);
		vec3 tran_ver = vec3(x, y, z) * magnification + center;
		// 更新点的最大值和最小值
		max_x = tran_ver.x > max_x ? tran_ver.x : max_x;
		max_y = tran_ver.y > max_y ? tran_ver.y : max_y;
		max_z = tran_ver.z > max_z ? tran_ver.z : max_z;
		min_x = tran_ver.x < min_x ? tran_ver.x : min_x;
		min_y = tran_ver.y < min_y ? tran_ver.y : min_y;
		min_z = tran_ver.z < min_z ? tran_ver.z : min_z;

		vertices.push_back(vec3(x, y, z) * magnification + center);
	}
	unsigned int l, m, n;
	for (int i = 0; i < num_triangle; i++) {
		fgets(buffer, 300, file);
		if (buffer[0] == '3') {
			buffer[0] = ' ';
			sscanf(buffer, "%u%u%u", &l, &m, &n);
			// 计算对应的法向量
			Indice idx;
			idx.a = l;
			idx.b = m;
			idx.c = n;
			indices.push_back(idx);
			PlaneArgs args;
			getPlaneArgs(vertices[l], vertices[m], vertices[n], args);
			plane_args.push_back(args);
			glm::vec3 v1 = vertices[l] - vertices[m];
			glm::vec3 v2 = vertices[m] - vertices[n];
			glm::vec3 norm = normalize(cross(v1, v2));
			normals.push_back(norm);
		}
	}
	std::cout << "Successfully load " << model_path << std::endl;
}

void Model::getPlaneArgs(vec3& p1, vec3& p2, vec3& p3, PlaneArgs& args) const {
	float x1 = p1.x, x2 = p2.x, x3 = p3.x;
	float y1 = p1.y, y2 = p2.y, y3 = p3.y;
	float z1 = p1.z, z2 = p2.z, z3 = p3.z;
	args.a = (y2 - y1) * (z3 - z1) - (z2 - z1) * (y3 - y1);
	args.b = (z2 - z1) * (x3 - x1) - (x2 - x1) * (z3 - z1);
	args.c = (x2 - x1) * (y3 - y1) - (y2 - y1) * (x3 - x1);
	args.d = -args.a * x1 - args.b * y1 - args.c * z1;
}

Indice Model::getIndice(int index) const {
	return indices[index];
}

vec3 Model::getNormal(int index) const {
	return normals[index];
}

vec3 Model::getVertice(int index) const {
	return vertices[index];
}

PlaneArgs Model::getPlaneArgs(int index) const {
	return plane_args[index];
}

/*
*	为模型划分包围盒
*/
void Model::buildBox() {
	std::cout << "Building bounding box..." << std::endl;
	// 根包围盒的左下角节点
	box = buildBox(vec3(min_x, min_y, min_z), max_x - min_x + 0.1f, max_y - min_y + 0.1f, max_z - min_z + 0.1f);
	std::cout << "Bounding box built successfully" << std::endl;
}


BoundingBox* Model::buildBox(vec3 root, float width, float height, float length) {
	int seg_x = width < LEAF_BOUNDING_BOX_WIDTH ? 1 : 2;
	int seg_y = height < LEAF_BOUNDING_BOX_HEIGHT ? 1 : 2;
	int seg_z = height < LEAF_BOUNDING_BOX_LENGTH ? 1 : 2;
	bool is_leaf = seg_x == 1 && seg_y == 1 && seg_z == 1;
	BoundingBox* cur_box = new BoundingBox(root.x, root.y, root.z, width, height, length, is_leaf);
	cur_box->setModel(this);
	for (int i = 0; i < indices.size(); ++i) {
		Indice cur_idx = indices[i];
		bool flag = true;
		int idx1 = cur_idx.a;
		if (vertices[idx1].x >= root.x && vertices[idx1].x <= root.x + width
			&& vertices[idx1].y >= root.y && vertices[idx1].y <= root.y + height
			&& vertices[idx1].z >= root.z && vertices[idx1].z <= root.z + length) {
			
		}
		else {
			flag = false;
		}
		int idx2 = cur_idx.a;
		if (vertices[idx2].x >= root.x && vertices[idx2].x <= root.x + width
			&& vertices[idx2].y >= root.y && vertices[idx2].y <= root.y + height
			&& vertices[idx2].z >= root.z && vertices[idx2].z <= root.z + length) {
			
		}
		else {
			flag = false;
		}
		int idx3 = cur_idx.a;
		if (vertices[idx3].x >= root.x && vertices[idx3].x <= root.x + width
			&& vertices[idx3].y >= root.y && vertices[idx3].y <= root.y + height
			&& vertices[idx3].z >= root.z && vertices[idx3].z <= root.z + length) {
			
		}
		else {
			flag = false;
		}
		if (flag) {
			cur_box->addIndice(i);
		}
	}
	if(!is_leaf) {
		for (int i = 0; i < seg_x; ++i) {
			for (int j = 0; j < seg_y; ++j) {
				for (int k = 0; k < seg_z; ++k) {
					BoundingBox* sub_box = buildBox(
						vec3(
							root.x + width / 2 * i, 
							root.y + height / 2 * j, 
							root.z + length / 2 * k
						), 
						width / (float)seg_x, 
						height / (float)seg_y, 
						length / (float)seg_z
					);
					cur_box->addSubBox(sub_box);
				}
			}
		}
	}
	return cur_box;
}

ObjectList::ObjectList() {

}

bool ObjectList::isHit(const Ray& r, float t_min, float t_max, HitRecord& rec) const {
	HitRecord temp_rec;
	bool hit_anything = false;
	double closet_so_far = 1e9;
	int size = objects.size();
	for (int i = 0; i < size; ++i) {
		if (objects[i]->isHit(r, t_min, closet_so_far, temp_rec)) {
			hit_anything = true;
			closet_so_far = temp_rec.t;
			rec = temp_rec;
		}
	}
	return hit_anything;
}

void ObjectList::addModel(Object* obj) {
	objects.push_back(obj);
}
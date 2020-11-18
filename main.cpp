#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <unordered_map>
#include <string>
#include <iostream>
#include <fstream>
#include <random>
#include <cmath>
#include <windows.h>
#include <time.h>
#include "config.h"
#include "Object.h"
#include "RayTracer.h"
#include "Ray.h"
#include "camera.h"
using namespace glm;

HANDLE mutex = NULL;
std::unordered_map<int, vec3> result;
std::default_random_engine e;
std::uniform_real_distribution<float> uniform(0.0, 1.0);



void runTracer();
ObjectList* loadModel();
DWORD WINAPI parallelTracing(void* parameters);


int main(int argc, char** argv) {
	clock_t t_begin = clock();
	runTracer();
	clock_t t_end = clock();
	std::cout << "Time Running: " << t_end - t_begin << " ms\n";
	return 0;
}

void runTracer() {
	std::fstream out(RESULT_PATH, std::ios::out);
	if (!out.is_open()) {
		// 打开文件失败
		std::cout << "FAILED TO OPEN " << RESULT_PATH << std::endl;
		return;
	}
	else {
		// 写入文件头
		out << "P3\n" << WINDOW_WIDTH << ' ' << WINDOW_HEIGHT << "\n255\n";
	}
	// 加载模型
	ObjectList* world = loadModel();

	// 创建追踪器
	RayTracer *rayTracer = new RayTracer();
	rayTracer->setWorld(world);

	// 创建相机
	Camera* camera = new Camera(VIEW_POINT, LOW_LEFT_CORNER, HORIZONAL, VERTICAL);

	// 创建多个线程，进行光线追踪
	// 创建互斥量
	char szStr[] = "Map";
	WCHAR wszMutexName[256];
	memset(wszMutexName, 0, sizeof(wszMutexName));
	MultiByteToWideChar(CP_ACP, 0, szStr, strlen(szStr) + 1, wszMutexName,
		sizeof(wszMutexName) / sizeof(wszMutexName[0]));
	mutex = CreateMutex(NULL, FALSE, wszMutexName);
	// 创建线程
	int curHeight = 0, perHeight = WINDOW_HEIGHT / NUM_THREADS;
	if (NUM_THREADS > 1) {
		HANDLE *threadHandles = new HANDLE[NUM_THREADS-1];
		// 前 N - 1 个线程等分工作量
		for (int i = 0; i < NUM_THREADS - 1; ++i) {
			ParallelTracingParameter* p = new ParallelTracingParameter();
			p->rayTracer = rayTracer;
			p->camera = camera;
			p->left = 0;
			p->right = WINDOW_WIDTH - 1;
			p->bottom = curHeight;
			curHeight += perHeight;
			p->top = curHeight - 1;
			threadHandles[i] = CreateThread(NULL, 0, parallelTracing, p, 0, NULL);
		}
		// 最后一个线程承担剩下的工作
		ParallelTracingParameter* p = new ParallelTracingParameter();
		p->rayTracer = rayTracer;
		p->camera = camera;
		p->left = 0;
		p->right = WINDOW_WIDTH - 1;
		p->bottom = curHeight;
		p->top = WINDOW_HEIGHT - 1;
		parallelTracing((void*)p);
		for (int i = 0; i < NUM_THREADS - 1; ++i) {
			WaitForSingleObject(threadHandles[i], INFINITE);
		}
		for (int i = 0; i < NUM_THREADS - 1; ++i) {
			CloseHandle(threadHandles[i]);
		}
	}
	else {
		ParallelTracingParameter* p = new ParallelTracingParameter();
		p->rayTracer = rayTracer;
		p->camera = camera;
		p->left = 0;
		p->right = WINDOW_WIDTH - 1;
		p->bottom = 0;
		p->top = WINDOW_HEIGHT - 1;
		parallelTracing((void*)p);
	}
	
	// 等待多线程结束
	std::cout << "Writing result..." << std::endl;
	for (int j = WINDOW_HEIGHT - 1; j>= 0; --j) {
		for (int i = 0; i < WINDOW_WIDTH; ++i) {
			vec3& col = result[WINDOW_WIDTH * j + i]; // (i, j)
			out << int(255.99 * col[0]) << ' ' << int(255.99 * col[1]) << ' ' << int(255.99 * col[2]) << '\n';
		}
	}
}

/*
* 加载模型，并返回加载后的模型列表
*/
ObjectList* loadModel() {
	ObjectList* world = new ObjectList();

	vec3 plane_center = vec3(0.0, -5001.f, -0.5f);
	Object* plane = new Sphere(plane_center, 5000.0f);
	Material* plane_mat = new Plastic();
	plane_mat->setColor(PLANE_COLOR);
	plane->setMaterial(plane_mat);
	world->addModel(plane);
	
	vec3 center_1 = vec3(-0.5, -0.3, -0.5);
	Model* model_1 = new Model(center_1, MODEL_MAGNIFICATION);
	model_1->load(MODEL_1_PATH);
	model_1->buildBox();
	Material* mat_1 = new Plastic();
	mat_1->setColor(vec3(226.f/255.0f, 75.f/255.f, 249.f/255.f));
	model_1->setMaterial(mat_1);
	world->addModel(model_1);

	vec3 center_2 = vec3(0, -0.3, -0.5);
	Model* model_2 = new Model(center_2, MODEL_MAGNIFICATION);
	model_2->load(MODEL_2_PATH);
	model_2->buildBox();
	Material* mat_2 = new Glass(1.5);
	model_2->setMaterial(mat_2);
	world->addModel(model_2);

	vec3 center_3 = vec3(0.5, -0.3, -0.5);
	Model* model_3 = new Model(center_3, MODEL_MAGNIFICATION);
	model_3->load(MODEL_3_PATH);
	model_3->buildBox();
	Material* mat_3 = new Metal();
	mat_3->setColor(vec3(0.6, 0.1, 0.1));
	model_3->setMaterial(mat_3);
	world->addModel(model_3);

	return world;
}

/*
* 并行光线追踪，将结果存在全局变量 result 中 一个线程将负责
* 追踪 [left, right] 和 [bottom, top] 这么大范围的屏幕
* rayTracer -> 光线追踪器
* left -> 分区的左边界
* right -> 分区的右边界
* top -> 分区的上边界
* bottom -> 分区的下边界
*/
DWORD WINAPI parallelTracing(void* parameters) {
	ParallelTracingParameter* p = (ParallelTracingParameter*)parameters;
	RayTracer* rayTracer = p->rayTracer;
	Camera* camera = p->camera;
	int top = p->top;
	int bottom = p->bottom;
	int left = p->left;
	int right = p->right;
	for (int j = top; j >= bottom; --j) {
		for (int i = left; i <= right; ++i) {
			vec3 col(0.0f, 0.0f, 0.0f);
			for (int s = 0; s < SMAA_SAMPLE_POINT; ++s) {
				// 抗锯齿
				float u = float(i + uniform(e)) / float(WINDOW_WIDTH);
				float v = float(j + uniform(e)) / float(WINDOW_HEIGHT);
				Ray r = camera->generateRay(u, v);
				col += rayTracer->trace(r);
			}
			if (ProgramDebugger::debug) {
				std::cout << '\n';
			}
			col /= float(SMAA_SAMPLE_POINT);
			//col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
			int key = j * WINDOW_WIDTH + i; // (i, j)
			WaitForSingleObject(mutex, INFINITE);
			result[key] = col;
			ReleaseMutex(mutex);
		}
	}
	return 0L;
}
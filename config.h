#pragma once
#include <glm/glm.hpp>

// 屏幕宽
#define WINDOW_WIDTH		1920
// 屏幕高
#define WINDOW_HEIGHT		1080
// 线程数
#define NUM_THREADS			6
// 模型 1 的路径
#define MODEL_1_PATH		"../resources/dragon_vrip_res4.ply"
// 模型 2 的路径
#define MODEL_2_PATH		"../resources/bun_zipper_res2.ply"
// 模型 3 的路径
#define MODEL_3_PATH		"../resources/happy_vrip_res4.ply"
// 模型的放大倍数
#define MODEL_MAGNIFICATION  1.f
// 光线追踪的递归深度
#define RAY_TRACE_DEPTH_LIMIT 50
// 结果存储的图片名称
#define RESULT_PATH			"../output/result.ppm"
// 视点
#define VIEW_POINT			glm::vec3(0.0f, 0.0f, 0.0f)
#define LOW_LEFT_CORNER		glm::vec3(-2.0f, -1.0f, -1.0f)
#define HORIZONAL			glm::vec3(4.0f, 0.0f, 0.0f)
#define VERTICAL			glm::vec3(0.0f, 2.0f, 0.0f)
// 抗锯齿的强度
#define SMAA_SAMPLE_POINT	5
// 金属材料的漫反射率
#define METAL_ALBEDO 0.8
// 金属材料镜面反射的随机率
#define METAL_FUZZ 0.3f
// 塑料材料的漫反射率
#define PLASTIC_ALBEDO 0.8
// 平面的颜色
#define PLANE_COLOR vec3(0, 1, 126.0f/255.0f)
// 模型 1 的颜色
#define MODEL_1_COLOR vec3(255.0f/255.0f, 255.0f/255.0f, 255.0f/255.0f)
// 是否加速
#define ACCELERATION true
// 叶子包围盒的尺寸
#define LEAF_BOUNDING_BOX_WIDTH 0.6f
#define LEAF_BOUNDING_BOX_LENGTH 0.6f
#define LEAF_BOUNDING_BOX_HEIGHT 0.6f
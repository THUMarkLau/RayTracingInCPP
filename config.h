#pragma once
#include <glm/glm.hpp>

// ��Ļ��
#define WINDOW_WIDTH		1920
// ��Ļ��
#define WINDOW_HEIGHT		1080
// �߳���
#define NUM_THREADS			6
// ģ�� 1 ��·��
#define MODEL_1_PATH		"../resources/dragon_vrip_res4.ply"
// ģ�� 2 ��·��
#define MODEL_2_PATH		"../resources/bun_zipper_res2.ply"
// ģ�� 3 ��·��
#define MODEL_3_PATH		"../resources/happy_vrip_res4.ply"
// ģ�͵ķŴ���
#define MODEL_MAGNIFICATION  1.f
// ����׷�ٵĵݹ����
#define RAY_TRACE_DEPTH_LIMIT 50
// ����洢��ͼƬ����
#define RESULT_PATH			"../output/result.ppm"
// �ӵ�
#define VIEW_POINT			glm::vec3(0.0f, 0.0f, 0.0f)
#define LOW_LEFT_CORNER		glm::vec3(-2.0f, -1.0f, -1.0f)
#define HORIZONAL			glm::vec3(4.0f, 0.0f, 0.0f)
#define VERTICAL			glm::vec3(0.0f, 2.0f, 0.0f)
// ����ݵ�ǿ��
#define SMAA_SAMPLE_POINT	5
// �������ϵ���������
#define METAL_ALBEDO 0.8
// �������Ͼ��淴��������
#define METAL_FUZZ 0.3f
// ���ϲ��ϵ���������
#define PLASTIC_ALBEDO 0.8
// ƽ�����ɫ
#define PLANE_COLOR vec3(0, 1, 126.0f/255.0f)
// ģ�� 1 ����ɫ
#define MODEL_1_COLOR vec3(255.0f/255.0f, 255.0f/255.0f, 255.0f/255.0f)
// �Ƿ����
#define ACCELERATION true
// Ҷ�Ӱ�Χ�еĳߴ�
#define LEAF_BOUNDING_BOX_WIDTH 0.6f
#define LEAF_BOUNDING_BOX_LENGTH 0.6f
#define LEAF_BOUNDING_BOX_HEIGHT 0.6f
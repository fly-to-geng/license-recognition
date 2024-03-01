#pragma once
#include "stdafx.h"
#include "CarPlatePosition.h"
#include "CarFlowCount.h"
#include "SetPreferencesDlg.h"

#include "cv.h"
#include "cvaux.h"
#include "CvvImage.h"
#include "cxcore.h"
#include "highgui.h"
#include <iostream>

//��¼�ָ���ͨ���λ����Ϣ
typedef struct ROI
{
	int x;
	int y;
	int width;
	int height;
}ROI;

int compare(const void *a,const void *b);
//װ�ر�׼�ַ���
void loadImageLib(IplImage *model[]);
//���ƶȼ��㣬�β�Ϊ��ֵ�����ͼƬ
double imageCompare(IplImage* dst, IplImage* model);
//�ַ���⺯��
void imageRecognition(IplImage *img, int m, int n);
//��ODQ���ж��μ��
int againRecognition_ODQ(IplImage* img);
//��2Z���ж��μ��
int againRecognition_2Z(IplImage* img);
//��5S���ж��μ��
int againRecognition_5S(IplImage* img);
//��8B���ж��μ��
int againRecognition_8B(IplImage* img);
//��Ӧ���ʶ��ĺ���
void representChinese(int k);
//ʶ����
void segmentRecognition();

class CarPlateRec2
{
public:
	CarPlateRec2(void);
	~CarPlateRec2(void);
};


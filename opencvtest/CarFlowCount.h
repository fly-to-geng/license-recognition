#pragma once
#include "cv.h"
#include "cv.hpp"
#include "highgui.h"
#include "resource.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>

using namespace std;
using namespace cv;
//�ַ��ṹ��
typedef struct Hei_sta{//��ȸ߶�ͳ�ƣ���һ�д�ߣ��ڶ��д涥�����꣬�����е׶����꣬,������������꣬�������Ҳ����� ������Ϊ���ֵ
	int height;
	int width;
	int top;
	int bottom;
	int left;
	int right;
	int sign;
};
class CarFlowCount:CDialogEx
{	
public:
	CarFlowCount(void);
	~CarFlowCount(void);
	

	//IplImage *frame_use; 
	//CString strTmp2_use;//��������
	CString strTmp2;
	
	

	IplImage* count_aga();
	int countCar();
	IplImage* cutRoi(IplImage* img, CvPoint pt1, int width,int height);
	void ShowImage(IplImage *img, UINT ID);
	IplImage* CopyIplImageROI(IplImage * pImageSrc,CvRect rectROI);
	IplImage* cvCopyIplImage(IplImage*  srcImg);
	vector<Hei_sta> find_connected_domain(IplImage* src_image, IplImage* dst_image);
};


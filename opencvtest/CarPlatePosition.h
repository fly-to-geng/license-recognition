#pragma once
#include "stdafx.h"
#include "cv.h"
#include "cvaux.h"
#include "CvvImage.h"
#include "cxcore.h"
#include "highgui.h"
#include "resource.h"
#include "CarFlowCount.h"
#include<vector>
#include<iostream>

using namespace std;
using namespace cv;

typedef struct VP{    
	int initiapos_row;//��ʼ�е�����ֵ
	int initiapos_col;//��ʼ�е�����ֵ
	int end_row;//�����е���ֵ
	int end_col;//�����е���ֵ
	int count;//��������
};

//�ַ��ṹ��


class CarPlatePosition
{
public:
	CarPlatePosition(void);
	~CarPlatePosition(void);
	
	/***************���ߺ���********************************/
	//void DrawPicToHDC(IplImage *img, UINT ID);
	//void OpenPicture();
	IplImage* cut_Roi_single(IplImage* img, CvPoint pt1, int width,int height);
	IplImage* cut_Roi_three(IplImage* img, CvPoint pt1, int width,int height);
	IplImage* CopyIplImage(IplImage*  srcImg);
	IplImage* CopyIplImageROI(IplImage* m_img_src,CvRect selection);

	/******************�����㷨����*******************************/
	//IplImage* cutRoi(IplImage* pimg, CvPoint pt1, int width,int height);
	IplImage* texture(IplImage *img_src) ;
	void vp_blue_white( int istart_row, int istart_col, int iend_row, int iend_col); //��Ϊ���װ��ֳ��ƣ���ȡ��������
	void Color_Trans(IplImage *img_src);
	void projection(IplImage *img, int istart_row, int iend_row, int istart_col, int iend_col, char s); //���к���ͶӰ�����¹��������Լ����ұ�Ե
	VP vp_detect(int W_Width, int W_Height);
	VP plate_segment(IplImage *src_img);
	//vector<Hei_sta> find_connected_domain(IplImage* src_image, IplImage* dst_image);	
	bool pre_check(vector <Hei_sta> vt_Connect,IplImage* src_image, IplImage* image_sign, IplImage* dst_image, IplImage* image_sign_ref);
	void  PlateCharSegment(IplImage *pImagePlate);
	void plateSegment();//���ƶ�λ����������
	void plateSegmentOne();//���ƶ�λһ��������
};


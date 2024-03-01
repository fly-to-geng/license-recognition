#include "StdAfx.h"
#include "CarPlateRec2.h"
#include "cv.h"
#include "CvvImage.h"
#include "highgui.h"
#include <iostream>
#include <vector>
#include <iostream>
#define MINAREA 100.0

//����ʶ����
using namespace std;
using namespace cv;

CarPlateRec2::CarPlateRec2(void)
{
}

CarPlateRec2::~CarPlateRec2(void)
{
}


string result_RES2;
//extern vector <Hei_sta> vt_Connect_use;
extern IplImage* vp_images_sign_ref; //�ַ��ָ��ĳ���ͼ��
extern IplImage* roiThree;  //ԭͼ�вü��Ĳ�ɫ���ƽ�ͼ

int compare(const void *a,const void *b)//������һ��ָ��a��a����ָ���������͵�ֵ������ָ���ֵ�����ǳ���
{
	return ((ROI *)a)->x - ((ROI *)b)->x;
}

//���ƶȼ��㣬�β�Ϊ��ֵ�����ͼƬ
double imageCompare(IplImage* dst, IplImage* model)
{
	double sum = 0; //ͳ��������ص�ĸ���
	double temp = 0.0;   //����������ƶ�

	uchar* data_dst = (uchar *)dst->imageData;
	uchar* data_model = (uchar *)model->imageData;
	int step_dst = dst->widthStep;
	int step_model = model->widthStep;
	double max =0.0;
	//����һ����ƫ�������бȽ�
	for(int row = -2; row < 3; row++)
		for(int col = -2; col < 3; col++)
		{
			sum = 0;
			for(int row_dst = (row>0?row:0), row_model = (row<0?(-row):0); row_dst < dst->height && row_model < model->height; row_dst++, row_model++)
			{
				for(int col_dst = (col>0?col:0), col_model = (col<0?(-col):0); col_dst < dst->width && col_model < model->width; col_dst++, col_model++)
					if(data_dst[row_dst * step_dst + col_dst] == data_model[row_model * step_model + col_model])
						sum++;
			}
			temp = sum/((dst->height-abs(row)) * (dst->width - abs(col)));
			if(temp > max)
				max = temp;
		}
		return max;
}

//��ODQ���ж��μ��
int againRecognition_ODQ(IplImage* img)
{
	IplImage* turn_image = cvCloneImage(img);
	int key;
	cvFlip(turn_image, NULL, 1);
	if(imageCompare(turn_image, img) > 0.94)			
		key = 0;
	else
	{
		uchar* data = (uchar *)img->imageData;
		int temp = 0;
		for(int i = 0; i < 4; i++)
			for(int j = 0; j < 4; j++)
				if(data[i*img->widthStep+j] == 255)
					temp++;
		if(temp >= 8)
			key = 13;
		else if(temp < 8)
		{
			temp = 0;
			for(int i = img->height - 4; i <= img->height; i++)
				for(int j = img->width - 4; j < img->width; j++)
					if(data[i*img->widthStep+j] == 255)
						temp++;
			if(temp >= 8)
				key = 26;
			else 
				key = 0;
		}
	}
	return key;
}

//��2Z���ж��μ��
int againRecognition_2Z(IplImage* img)
{
	IplImage* turn_image = cvCloneImage(img);
	int key;
	cvFlip(turn_image, NULL, -1);
	if(imageCompare(turn_image, img) > 0.94)			
		key = 35;
	else
	{
		uchar* data = (uchar *)img->imageData;
		int temp = 0;
		for(int i = 0; i < 4; i++)
			for(int j = 0; j < 4; j++)
				if(data[i * img->widthStep + j] == 255)
					temp++;
		if(temp >= 8)
			key = 35;
		else if(temp < 8)
		{
			temp = 0;
			for(int i = 0; i < 4; i++)
				for(int j = img->width - 4; j < img->width; j++)
					if(data[i * img->widthStep + j] == 255)
						temp++;
			if(temp >= 8)
				key = 35;
			else
				key = 2;
		}
	}
	return key;
}

//��5S���ж��μ��
int againRecognition_5S(IplImage* img)
{
	IplImage* turn_image = cvCloneImage(img);
	int key;
	cvFlip(turn_image, NULL, -1);
	if(imageCompare(turn_image, img) > 0.94)			
		key = 28;
	else
	{
		uchar* data = (uchar *)img->imageData;
		int temp = 0;
		for(int i = 0; i < 4; i++)
			for(int j = 0; j < 4; j++)
				if(data[i * img->widthStep + j] == 255)
					temp++;
		if(temp >= 8)
			key = 5;
		else if(temp < 8)
		{
			temp = 0;
			for(int i = 0; i < 4; i++)
				for(int j = img->width - 4; j < img->width; j++)
					if(data[i * img->widthStep + j] == 255)
						temp++;
			if(temp >= 8)
				key = 5;
			else 
				key = 28;
		}
	}
	return key;
}

//��8B���ж��μ��
int againRecognition_8B(IplImage* img)
{
	IplImage* turn_image = cvCloneImage(img);
	int key;
	cvFlip(turn_image, NULL, 1);
	if(imageCompare(turn_image, img) > 0.94)			
		key = 8;
	else
	{
		uchar* data = (uchar *)img->imageData;
		int temp = 0;
		for(int i = 0; i < 4; i++)
			for(int j = 0; j < 4; j++)
				if(data[i * img->widthStep + j] == 255)
					temp++;
		for(int i = img->height - 4; i < img->height; i++)
			for(int j = 0; j < 4; j++)
				if(data[i * img->widthStep + j] == 255)
					temp++;
		if(temp >= 10)
			key = 11;
		else 
			key = 8;		
	}
	return key;
}

//װ�ر�׼�ַ���
void loadImageLib(IplImage *model[])
{
	//װ������ͼƬ��
	model[0] = cvLoadImage("D:\\lib\\num\\0.jpg", CV_LOAD_IMAGE_GRAYSCALE); 
	model[1] = cvLoadImage("D:\\lib\\num\\1.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	model[2] = cvLoadImage("D:\\lib\\num\\2.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	model[3] = cvLoadImage("D:\\lib\\num\\3.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	model[4] = cvLoadImage("D:\\lib\\num\\4.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	model[5] = cvLoadImage("D:\\lib\\num\\5.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	model[6] = cvLoadImage("D:\\lib\\num\\6.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	model[7] = cvLoadImage("D:\\lib\\num\\7.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	model[8] = cvLoadImage("D:\\lib\\num\\8.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	model[9] = cvLoadImage("D:\\lib\\num\\9.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	//װ����ĸͼƬ��
	model[10] = cvLoadImage("D:\\lib\\alphabet\\38.jpg", CV_LOAD_IMAGE_GRAYSCALE); 
	model[11] = cvLoadImage("D:\\lib\\alphabet\\39.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	model[12] = cvLoadImage("D:\\lib\\alphabet\\40.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	model[13] = cvLoadImage("D:\\lib\\alphabet\\41.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	model[14] = cvLoadImage("D:\\lib\\alphabet\\42.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	model[15] = cvLoadImage("D:\\lib\\alphabet\\43.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	model[16] = cvLoadImage("D:\\lib\\alphabet\\44.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	model[17] = cvLoadImage("D:\\lib\\alphabet\\45.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	model[18] = cvLoadImage("D:\\lib\\alphabet\\46.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	model[19] = cvLoadImage("D:\\lib\\alphabet\\47.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	model[20] = cvLoadImage("D:\\lib\\alphabet\\48.jpg", CV_LOAD_IMAGE_GRAYSCALE); 
	model[21] = cvLoadImage("D:\\lib\\alphabet\\49.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	model[22] = cvLoadImage("D:\\lib\\alphabet\\50.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	model[23] = cvLoadImage("D:\\lib\\alphabet\\51.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	model[24] = cvLoadImage("D:\\lib\\alphabet\\52.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	model[25] = cvLoadImage("D:\\lib\\alphabet\\53.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	model[26] = cvLoadImage("D:\\lib\\alphabet\\54.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	model[27] = cvLoadImage("D:\\lib\\alphabet\\55.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	model[28] = cvLoadImage("D:\\lib\\alphabet\\56.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	model[29] = cvLoadImage("D:\\lib\\alphabet\\57.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	model[30] = cvLoadImage("D:\\lib\\alphabet\\58.jpg", CV_LOAD_IMAGE_GRAYSCALE); 
	model[31] = cvLoadImage("D:\\lib\\alphabet\\59.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	model[32] = cvLoadImage("D:\\lib\\alphabet\\60.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	model[33] = cvLoadImage("D:\\lib\\alphabet\\61.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	model[34] = cvLoadImage("D:\\lib\\alphabet\\62.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	model[35] = cvLoadImage("D:\\lib\\alphabet\\63.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	//װ�غ���ͼƬ��
	model[36] = cvLoadImage("D:\\lib\\chinese\\1.jpg", CV_LOAD_IMAGE_GRAYSCALE); 
	model[37] = cvLoadImage("D:\\lib\\chinese\\2.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	model[38] = cvLoadImage("D:\\lib\\chinese\\3.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	model[39] = cvLoadImage("D:\\lib\\chinese\\4.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	model[40] = cvLoadImage("D:\\lib\\chinese\\5.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	model[41] = cvLoadImage("D:\\lib\\chinese\\6.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	model[42] = cvLoadImage("D:\\lib\\chinese\\7.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	model[43] = cvLoadImage("D:\\lib\\chinese\\8.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	model[44] = cvLoadImage("D:\\lib\\chinese\\9.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	model[45] = cvLoadImage("D:\\lib\\chinese\\10.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	model[46] = cvLoadImage("D:\\lib\\chinese\\11.jpg", CV_LOAD_IMAGE_GRAYSCALE); 
	model[47] = cvLoadImage("D:\\lib\\chinese\\12.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	model[48] = cvLoadImage("D:\\lib\\chinese\\13.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	model[49] = cvLoadImage("D:\\lib\\chinese\\14.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	model[50] = cvLoadImage("D:\\lib\\chinese\\15.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	model[51] = cvLoadImage("D:\\lib\\chinese\\16.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	model[52] = cvLoadImage("D:\\lib\\chinese\\17.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	model[53] = cvLoadImage("D:\\lib\\chinese\\18.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	model[54] = cvLoadImage("D:\\lib\\chinese\\19.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	model[55] = cvLoadImage("D:\\lib\\chinese\\20.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	model[56] = cvLoadImage("D:\\lib\\chinese\\21.jpg", CV_LOAD_IMAGE_GRAYSCALE); 
	model[57] = cvLoadImage("D:\\lib\\chinese\\22.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	model[58] = cvLoadImage("D:\\lib\\chinese\\23.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	model[59] = cvLoadImage("D:\\lib\\chinese\\24.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	model[60] = cvLoadImage("D:\\lib\\chinese\\25.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	model[61] = cvLoadImage("D:\\lib\\chinese\\26.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	model[62] = cvLoadImage("D:\\lib\\chinese\\27.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	model[63] = cvLoadImage("D:\\lib\\chinese\\28.jpg", CV_LOAD_IMAGE_GRAYSCALE); 
	model[64] = cvLoadImage("D:\\lib\\chinese\\29.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	model[65] = cvLoadImage("D:\\lib\\chinese\\30.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	model[66] = cvLoadImage("D:\\lib\\chinese\\31.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	model[67] = cvLoadImage("D:\\lib\\chinese\\32.jpg", CV_LOAD_IMAGE_GRAYSCALE);
}

void representChinese(int k)
{
	switch(k)
	{
	case 36:
		result_RES2 += "��";
		break;
	case 37:
		result_RES2 += "��";
		break;
	case 38:
		result_RES2 += "��";
		break;
	case 39:
		result_RES2 += "��";
		break;
	case 40:
		result_RES2 += "��";
		break;
	case 41:
		result_RES2 += "��";
		break;
	case 42:
		result_RES2 += "��";
		break;
	case 43:
		result_RES2 += "��";
		break;
	case 44:
		result_RES2 += "��";
		break;
	case 45:
		result_RES2 += "��";
		break;
	case 46:
		result_RES2 += "��";
		break;
	case 47:
		result_RES2 += "��";
		break;
	case 48:
		result_RES2 += "��";
		break;
	case 49:
		result_RES2 += "��";
		break;
	case 50:
		result_RES2 += "³";
		break;
	case 51:
		result_RES2 += "ԥ";
		break;
	case 52:
		result_RES2 += "��";
		break;
	case 53:
		result_RES2 += "��";
		break;
	case 54:
		result_RES2 += "��";
		break;
	case 55:
		result_RES2 += "��";
		break;
	case 56:
		result_RES2 += "��";
		break;
	case 57:
		result_RES2 += "��";
		break;
	case 58:
		result_RES2 += "��";
		break;
	case 59:
		result_RES2 += "��";
		break;
	case 60:
		result_RES2 += "��";
		break;
	case 61:
		result_RES2 += "��";
		break;
	case 62:
		result_RES2 += "��";
		break;
	case 63:
		result_RES2 += "��";
		break;
	case 64:
		result_RES2 += "��";
		break;
	case 65:
		result_RES2 += "��";
		break;
	case 66:
		result_RES2 += "��";
		break;
	case 67:
		result_RES2 += "��";
		break;
	}
}


void imageRecognition(IplImage *img, int m, int n)
{
	IplImage *model[68];  //����ģ��ͼƬ
	double key[68];		  //�������ģ������ƶ�
	IplImage* dst;

	//���ݷָ���ַ��Ŀ�ȣ�����ֱ���ж��Ƿ�Ϊ�ַ�1
	 	if(img->width >= 10 && img->width <= 25)
		{
			int a = 1;
			result_RES2 += (char)(a + '0');
			return ;
		}

	loadImageLib(model);  //����ģ���ַ���
	for(int i = 0; i < 68; i++)
		key[i] = 0.0;

	//����ֵ�����ͼƬ�ֱ���ÿ��ģ��Ƚϣ��ó����ƶ�
	for(int i = m; i <= n; i++)
	{
		int width = model[i]->width;
		int height = model[i]->height;

		//ֻ�Ƕ�����һ��ͼ��
		IplImage* temp = cvCreateImage(cvSize(width, height), 8, 1);
		dst = cvCreateImage(cvSize(width, height), 8, 1);
		IplImage* mod = cvCreateImage(cvSize(width, height), 8, 1);

		//���ָ�����ַ���������ģ���ַ�һ����С
		cvResize(img, temp, CV_INTER_NN);


		//void cvResize( const CvArr* src, CvArr* dst, int interpolation=CV_INTER_LINEAR );
		//����cvResize ���µ���ͼ��src��������ROI����ʹ����ȷƥ��Ŀ��dst������ROI����
		//cvNamedWindow("������С��",CV_WINDOW_AUTOSIZE);     
		//cvShowImage("������С��", temp);  
		//void cvThreshold( const CvArr* src, CvArr* dst, double threshold, double max_value, int threshold_type );
		//��tempͼ����ж�ֵ���������õ�dstͼ����ֵΪ110
		cvThreshold(temp, dst, 110, 255, CV_THRESH_OTSU);//��ֵ��

		//��ģ����ж�ֵ������,��ֵ������뵽modͼ����
		cvThreshold(model[i], mod, 128, 255, CV_THRESH_BINARY_INV);//��ֵ��


		key[i] = imageCompare(dst, mod);
	}

	//�ҳ��������ƶ�
	double max = 0;
	int k = -1;
	for(int i = m; i <= n; i++)
	{
		if(key[i] > max)
		{
			max = key[i];
			k = i;
		}
	}

	//����ж�ΪODQ֮һ������ж��μ��
	if((k == 0 || k == 13 || k == 26) && m == 0 && n == 35)
		k = againRecognition_ODQ(dst);

	//����ж�Ϊ2Z֮һ������ж��μ��
	if((k == 2 || k == 35) && m == 0 && n == 35)
		k = againRecognition_2Z(dst);

	//����ж�Ϊ5S֮һ������ж��μ��
	if((k == 5 || k == 28) && m == 0 && n == 35)
		k = againRecognition_5S(dst);

	//����ж�Ϊ8B֮һ������ж��μ��
	if((k == 8 || k == 11) && m == 0 && n == 35)
		k = againRecognition_8B(dst);

	//���ʶ���ַ�
	if(k >= 10 && k <= 35)
		result_RES2 += (char)(k+55);
	//printf("%c", k+55);
	else if(k < 10)
		result_RES2 += (char)(k+'0');
	//printf("%d", k);
	else
		representChinese(k);
	return ;
}

void segmentRecognition()
{
	IplImage* roiThree_copy = cvCreateImage(cvGetSize(roiThree),roiThree->depth,roiThree->nChannels);
	cvCopy(roiThree,roiThree_copy);

	IplImage *src = cvCreateImage(cvGetSize(roiThree_copy),roiThree_copy->depth,1);//�Ҷ�ͼ
	IplImage* img_8uc1 = cvCreateImage(cvSize(409, 90), 8, 1);
	IplImage* img_edge = cvCreateImage(cvSize(409, 90), 8, 1);
	IplImage* img_8uc3 = cvCreateImage(cvSize(409, 90), 8, 3);
	cvCvtColor(roiThree_copy, src,CV_BGR2GRAY);
	//ShowImage(src, IDC_STATIC_GRAY);  //��ʾ�Ҷ�ͼ

	cvResize(src,img_8uc1,CV_INTER_LINEAR); //���Բ�ֵ���任ͼƬΪ�����С
	//cvShowImage("�ҶȻ�", img_8uc1);

	cvSmooth(img_8uc1,img_8uc1,CV_GAUSSIAN,3,0,0); //��˹�˲���ȥ��
	//ShowImage(img_8uc1, IDC_STATIC_GUASS);  //��ʾ��˹�˲�ͼ��

	cvThreshold(img_8uc1, img_edge, 200, 255, CV_THRESH_OTSU);//��ֵ��
	//cvShowImage("ȥ��", img_8uc1);
	//cvShowImage("��ֵ��", img_edge);
	//ShowImage(img_edge, IDC_STATIC_THRESH);  //��ʾ��˹�˲�ͼ��

	//��ȡ�ַ�
	IplImage *pImgCharOne=NULL;
	IplImage *pImgCharTwo=NULL;
	IplImage *pImgCharThree=NULL;
	IplImage *pImgCharFour=NULL;
	IplImage *pImgCharFive=NULL;
	IplImage *pImgCharSix=NULL;
	IplImage *pImgCharSeven=NULL;

	//��¼�����ַ���λ����Ϣ
	ROI cROI[7]; 

	//��������һ���ڴ�洢������ͳһ������ֶ�̬������ڴ档
	//��������һ���´������ڴ�洢��ָ��
	CvMemStorage * storage = cvCreateMemStorage(0);
	CvSeq * first_contour = NULL;//CvSeq��һ�ֶ�̬�����ݽṹ

	cvCvtColor(img_8uc1, img_8uc3, CV_GRAY2BGR); //ת��Ϊ��ͼ�����ڱ����ͨ��
	//ɾ����С����ͨ��
	IplImage* img_clone=cvCloneImage(img_edge); 
	cvFindContours( img_clone, storage, &first_contour, sizeof(CvContour), CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE); 
	for(CvSeq * del = first_contour; del != NULL; del = del->h_next)
	{
		cvDrawContours(img_8uc3, del, CV_RGB(255,0,0), CV_RGB(0, 0, 255), 0, 2, 8);
		//���������С�ж�С��ͨ��
		double area = fabs(cvContourArea(del));
		printf("%f\n", area);
		CvRect delRect = cvBoundingRect(del,0);
		if(area < MINAREA)
		{
			//��img_edge�ж�Ӧ��С��ͨ���Ϊ��ɫ
			uchar *p;       
			for(int y = delRect.y; y<delRect.y+delRect.height; y++)         
				for(int x =delRect.x; x<delRect.x+delRect.width; x++)
				{
					p = (uchar*)(img_edge->imageData + img_edge->widthStep * y + x);
					p[0] = 0;
				}
				continue;
		}
	}

	 	//��ȥ����С��ͨ��Ķ�ֵͼ��img_edge�в�����ͨ�򣬷���ֵΪ��ͨ��ĸ���
	 	int Nc = cvFindContours( img_edge, storage, &first_contour, sizeof(CvContour), CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE);
	 	int n = 0;           //��Ǹ����ַ����±�
	 	int minleft = 409;	//��¼����ߵ���ͨ�������
	 
	 	for(CvSeq * c = first_contour; c != NULL; c = c->h_next)
	 	{
	 		CvRect rect;//����һ�����Ͻ�����Ϊ��x,y��������Ϊwidth�����Ϊheight�ľ���
	 		rect = cvBoundingRect(c,0); //�����ַ��߽�ľ���//����㼯������ľ��α߽�
	 		printf("\n#%d #%d *%d *%d\n\n",rect.x, rect.y, rect.width, rect.height );
	 		//�ж��Ƿ�Ϊ��߿�
	 		if(!((rect.x < 25 && rect.height > 80) || (rect.x < 25 && rect.width > 60) || (rect.x < 15 && rect.width < 8 /*11*/)))
	 		{
	 			//�ֶ�λ��һ���ַ�����߽�
	 			if(minleft > rect.x)
	 				minleft = rect.x;
	 		}
	 
	 		//����ͨ���С�����ַ�
	 		if(rect.height >= 47 && rect.width >= 8 && rect.width <= 54 && rect.x > 45 && rect.x < 398)
	 		{
	 			cROI[n].x = rect.x;
	 			cROI[n].y = rect.y;
	 			cROI[n].width = rect.width;
	 			cROI[n].height = rect.height;
	 			n++;
	 		}
	 	}
	 
	 	if(n == 6)  //˳���ָ������һ���ַ�����ĺ�6���ַ�
	 	{
	 		//���ַ�����������
	 		qsort(cROI, 6, sizeof(ROI),compare);
	 
	 		//ȷ����һ���ַ�������
	 		if(minleft + cROI[0].width + 20 < cROI[0].x)
	 		{
	 			cROI[6].x = cROI[0].x - 12 - cROI[0].width;
	 			if(cROI[6].x < 0)
	 				cROI[6].x = minleft;
	 		}
	 		else
	 			cROI[6].x = minleft;
	 
	 		//�ڶ����͵��������������ֵ��Ϊ����С��������Ӱ��
	 		cROI[6].y = (cROI[1].y + cROI[2].y + cROI[3].y + cROI[5].y)/4; 
	 		cROI[6].width = cROI[0].x - cROI[6].x - 5;
	 		cROI[6].height = (cROI[0].height + cROI[1].height + cROI[2].height + cROI[3].height + cROI[4].height + cROI[5].height)/6;
	 	}
	 	else    //δ�ֳܷ���6���ַ�ʱ�����ݷָ����ĳ���ַ���ȡ�����ַ���λ�ã��Ӷ��õ�6���ַ�
	 	{
	 		//printf("wrong = %d\n", n);
	 		int x;
	 		int y;
	 		int width;
	 		int height;
	 
	 		if(n != 0 && minleft < 40)
	 		{
	 			int sum_height = 0;
	 			int sum_y = 0;
	 			for(int i = 0; i < n; i++)
	 			{
	 				sum_height += cROI[i].height;
	 				sum_y += cROI[i].y;
	 			}
	 			x = minleft;
	 			y = sum_y/n;
	 			width = cROI[0].width;
	 			height = sum_height/n;		
	 
	 			CvRect rect_ROI[6];
	 
	 			//��һ���ַ�λ��
	 			cROI[6].x = minleft;
	 			cROI[6].y = y;
	 			cROI[6].width = width;
	 			cROI[6].height = height;
	 
	 			for(int i = 0; i < 6; i++)
	 			{
	 				if(i == 0)
	 				{
	 					rect_ROI[i].x = minleft + (i + 1) * width;
	 					rect_ROI[i].y = y;
						rect_ROI[i].width = width + 10;
	 					rect_ROI[i].height = height;
	 				}
	 				else
	 				{
	 					rect_ROI[i].x = minleft + (i + 1) * (width + 12) + 10;
	 					rect_ROI[i].y = y;
	 					rect_ROI[i].width = width + 20; /*15*/
	 					if(rect_ROI[i].x + rect_ROI[i].width > 409)
	 						rect_ROI[i].width = 409 - rect_ROI[i].x;
	 					rect_ROI[i].height = height;
	 				}
	 
	 				IplImage* temp_img = cvCreateImage(cvSize(rect_ROI[i].width, rect_ROI[i].height),IPL_DEPTH_8U,1);
	 				IplImage* temp_img3 = cvCreateImage(cvSize(rect_ROI[i].width, rect_ROI[i].height),IPL_DEPTH_8U,3);
	 
	 				cvSetImageROI(img_8uc1, rect_ROI[i]);
	 				cvCopy(img_8uc1, temp_img, NULL); 
					cvResetImageROI(img_8uc1);
	
	 				cvThreshold(temp_img, temp_img, 128, 255, CV_THRESH_OTSU);//��ֵ��
					CvMemStorage * storage_ROI = cvCreateMemStorage(0);
	 				CvSeq * first_contourROI = NULL;
	 
	 				int N = cvFindContours( temp_img, storage_ROI, &first_contourROI, sizeof(CvContour), CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE);
	 
	 				cvCvtColor(temp_img, temp_img3, CV_GRAY2BGR); //ת��Ϊ��ͼ�����ڱ����ͨ��
	 
					double contour_area = 0.0;
				CvRect MaxRect;
					for(CvSeq * contour = first_contourROI; contour != NULL; contour = contour->h_next)
					{
						cvDrawContours(temp_img3, contour, CV_RGB(255,0,0), CV_RGB(0, 0, 255), 0, 2, 8);
	 					//cvShowImage("temp_img3", temp_img3);
	
						double temp_area = fabs(cvContourArea(contour));
						if(contour_area < temp_area)
						{
 						contour_area = temp_area;
							MaxRect = cvBoundingRect(contour, 0);
						//printf("%d %d\n", MaxRect.x, MaxRect.y);
						}
	 				}
	
				//�ᡢ�������ʵ��λ��Ӧ��Ϊ�и������ͼ��ԭͼ�������������ͼ�е����λ��
	 				cROI[i].x = MaxRect.x + rect_ROI[i].x;
	 				cROI[i].y = MaxRect.y + rect_ROI[i].y;
					cROI[i].width = MaxRect.width;
	 				cROI[i].height = MaxRect.height;
	 			}
			}
	 		else
	 		{
	 			result_RES2 += "�ַ��ָ�ʧ��";
	 //			SetDlgItemText(IDC_EDIT1, result);
	 			return;
	 		}
	 	}


	pImgCharOne=cvCreateImage(cvSize(cROI[6].width, cROI[6].height),IPL_DEPTH_8U,1);
	pImgCharTwo=cvCreateImage(cvSize(cROI[0].width, cROI[0].height),IPL_DEPTH_8U,1);
	pImgCharThree=cvCreateImage(cvSize(cROI[1].width, cROI[1].height),IPL_DEPTH_8U,1);
	pImgCharFour=cvCreateImage(cvSize(cROI[2].width, cROI[2].height),IPL_DEPTH_8U,1);
	pImgCharFive=cvCreateImage(cvSize(cROI[3].width, cROI[3].height),IPL_DEPTH_8U,1);
	pImgCharSix=cvCreateImage(cvSize(cROI[4].width, cROI[4].height),IPL_DEPTH_8U,1);
	pImgCharSeven=cvCreateImage(cvSize(cROI[5].width, cROI[5].height),IPL_DEPTH_8U,1);

	//����ȷ���ַ�����
	CvRect ROI_rect1;


	//�ָ��һ���ַ�
	 	ROI_rect1.x=cROI[6].x;
	 	ROI_rect1.y=cROI[6].y;
	 	ROI_rect1.width=cROI[6].width;
	 	ROI_rect1.height=cROI[6].height;
		cvSetImageROI(img_8uc1,ROI_rect1);
	 	cvCopy(img_8uc1,pImgCharOne,NULL); 
		cvResetImageROI(img_8uc1);


	//�ָ�ڶ����ַ�
	 	ROI_rect1.x=cROI[0].x;
	 	ROI_rect1.y=cROI[0].y;
	 	ROI_rect1.width=cROI[0].width;
	 	ROI_rect1.height=cROI[0].height;
	 	cvSetImageROI(img_8uc1,ROI_rect1);
	 	cvCopy(img_8uc1,pImgCharTwo,NULL); 
	 	cvResetImageROI(img_8uc1);


	//�ָ�������ַ�
	 	ROI_rect1.x=cROI[1].x;
	 	ROI_rect1.y=cROI[1].y;
		ROI_rect1.width=cROI[1].width;
	 	ROI_rect1.height=cROI[1].height;
	 	cvSetImageROI(img_8uc1,ROI_rect1);
	 	cvCopy(img_8uc1,pImgCharThree,NULL); 
	 	cvResetImageROI(img_8uc1);


	//�ָ���ĸ��ַ�
	 	ROI_rect1.x=cROI[2].x;
	 	ROI_rect1.y=cROI[2].y;
	 	ROI_rect1.width=cROI[2].width;
	 	ROI_rect1.height=cROI[2].height;
	 	cvSetImageROI(img_8uc1,ROI_rect1);
	 	cvCopy(img_8uc1,pImgCharFour,NULL); 
	 	cvResetImageROI(img_8uc1);


	//�ָ������ַ�
		ROI_rect1.x=cROI[3].x;
	 	ROI_rect1.y=cROI[3].y;
	 	ROI_rect1.width=cROI[3].width;
	 	ROI_rect1.height=cROI[3].height;
	 	cvSetImageROI(img_8uc1,ROI_rect1);
	 	cvCopy(img_8uc1,pImgCharFive,NULL); 
	 	cvResetImageROI(img_8uc1);


	//�ָ�������ַ�
	 	ROI_rect1.x=cROI[4].x;
	 	ROI_rect1.y=cROI[4].y;
	 	ROI_rect1.width=cROI[4].width;
	 	ROI_rect1.height=cROI[4].height;
	 	cvSetImageROI(img_8uc1,ROI_rect1);
	 	cvCopy(img_8uc1,pImgCharSix,NULL); 
	 	cvResetImageROI(img_8uc1);



	//�ָ���߸��ַ�
	 	ROI_rect1.x=cROI[5].x;
	 	ROI_rect1.y=cROI[5].y;
	 	ROI_rect1.width=cROI[5].width;
	 	ROI_rect1.height=cROI[5].height;
	 	cvSetImageROI(img_8uc1,ROI_rect1);
	 	cvCopy(img_8uc1,pImgCharSeven,NULL); 
	 	cvResetImageROI(img_8uc1);


	/*----------------------�Էָ�����ַ����м��----------------------*/
	//����һ���ַ�
	imageRecognition(pImgCharOne, 36, 67);

	//���ڶ����ַ�
	imageRecognition(pImgCharTwo, 10, 35);

	//���������ַ�
	imageRecognition(pImgCharThree, 0, 35);

	//�����ĸ��ַ�
	imageRecognition(pImgCharFour, 0, 35);

	//��������ַ�
	imageRecognition(pImgCharFive, 0, 9);

	//���������ַ�
	imageRecognition(pImgCharSix, 0, 9);

	//�����߸��ַ�
	imageRecognition(pImgCharSeven, 0, 9);

	//��ʾʶ����
	//SetDlgItemText(IDC_EDIT1, result);

	//ʹ�����ͷŸ�����ʱ����
	cvReleaseImage(&pImgCharOne);
	cvReleaseImage(&pImgCharTwo);
	cvReleaseImage(&pImgCharThree);
	cvReleaseImage(&pImgCharFour);
	cvReleaseImage(&pImgCharFive);
	cvReleaseImage(&pImgCharSix);
	cvReleaseImage(&pImgCharSeven);
	cvReleaseImage(&img_8uc1);
	cvReleaseImage(&roiThree_copy);
}
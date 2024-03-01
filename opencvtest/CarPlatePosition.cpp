#include "StdAfx.h"
#include "CarPlatePosition.h"
#include "CarFlowCount.h"
#include "SetPreferencesDlg.h"
#include "cv.h"
#include "cvaux.h"
#include "CvvImage.h"
#include "cxcore.h"
#include "highgui.h"

using namespace cv;
using namespace std;

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���
struct VP m_Vp_Region;
//extern IplImage* ;
IplImage* m_img_VG;//���ڱ�������������ͼ��
IplImage* m_img_blue;//����HSVͼ��ռ�ת�������ֵ��
IplImage* cutPic_plate;
IplImage* vp_images_bin;
IplImage* m_img_vp_bluewhite;//ԭͼ���ֵ�������Ч��
IplImage* roiThree;  //ԭͼ�вü��Ĳ�ɫ���ƽ�ͼ
IplImage* vp_images_sign_ref; //�ַ��ָ��ĳ���ͼ��
//���ڳ��ƾ�ȷ��λ�Ĳ���
int ntop = 0;
int nOther = 0;
int m_iStart_col = 0;
int m_iEnd_col = 0;
int m_iStart_row = 0;
int m_iEnd_row = 0;

//����RGBͼ��HSVͼ��ɫ�ʿռ�ķ���
IplImage* m_img_H;
IplImage* m_img_S;
IplImage* m_img_V;

/*******************���ƶ�λ����ʹ���� һ������Ŀ���**************************/
//extern BOOL ONE_PART;
extern IplImage* one_img;
/********�ⲿȫ�ֱ��������ڽ��е�����λ����ͼ��**********/
extern IplImage* srcPic;//���� ����ʹ�ó��ƶ�λ�Ĵ���ͼ��
extern IplImage* pPic;//����һ������ĳ��ƶ�λ�Ĵ���ͼ��
//IplImage* srcPic_One;
//Mat image;//����һ�������Mat
CarFlowCount cfc;
SetPreferencesDlg spd;

/***************�ļ��еĲ�����ȡ��DLG�еĲ������壬����ȫ�ֱ���Ϊ���������************************/
//ComBox����
extern int inlaneInt;//�����ַ���
extern string inareaString;//���Ƶ����ַ���
extern string insearchboxString;//�ֶ�λ�������ַ���

//CStrilBar����
extern int insprpiexlInt;//���ز�
extern double inzoomFloat;//���ű���
extern int inthresholdInt;//��ֵ
extern int innocarpiexlInt;//�޳�����ֵ
extern int inplatethresholdInt;//���ƶ�ֵ����ֵ

//�Զ���·������
extern string inplatepathString;//����·��
extern string inresultpathString;//���·��
extern string incutpicpathString;//��ͼ·��

CarPlatePosition::CarPlatePosition(void)
{
}

CarPlatePosition::~CarPlatePosition(void)
{
}

/*************���ߺ���******************/
//��ȡ��ͨ��ͼ��
IplImage* CarPlatePosition::cut_Roi_single(IplImage* img, CvPoint pt1, int width,int height)
{
	//����һ����cvSize����������С��ͬ�ľ���
	IplImage* src_photo_copy = cvCreateImage(cvSize(width,height),IPL_DEPTH_8U,1);
	//����������һ������	
	//cvRectangle(img,pt1,cvPoint(pt1.x+width,pt1.y+height),Scalar(0,0,255),1,8,0);
	CvRect roi =cvRect(pt1.x, pt1.y, width, height); //�������꣬���γ�����
	cvSetImageROI(img,roi);//��src�н�ȡָ�����δ�С��ͼ��
	cvCopy(img,src_photo_copy);
	cvResetImageROI(img);
	return src_photo_copy;
}
//��ȡ��ͨ��ͼ��
IplImage* CarPlatePosition::cut_Roi_three(IplImage* img, CvPoint pt1, int width,int height)
{
	//����һ����cvSize����������С��ͬ�ľ���
	IplImage* src_photo_copy = cvCreateImage(cvSize(width,height),img->depth,img->nChannels);
	//����������һ������	
	//cvRectangle(img,pt1,cvPoint(pt1.x+width,pt1.y+height),Scalar(0,0,255),1,8,0);
	CvRect roi =cvRect(pt1.x, pt1.y, width, height); //�������꣬���γ�����
	cvSetImageROI(img,roi);//��src�н�ȡָ�����δ�С��ͼ��
	cvCopy(img,src_photo_copy);
	cvResetImageROI(img);
	return src_photo_copy;
}
//����ͼ��
IplImage* CarPlatePosition::CopyIplImage(IplImage*  srcImg)
{

	IplImage* dstImg = cvCreateImage(cvGetSize(srcImg),srcImg->depth,srcImg->nChannels);
	cvCopy(srcImg,dstImg);
	return dstImg;
}
//����ͼ��ָ������
IplImage* CarPlatePosition::CopyIplImageROI(IplImage* m_img_src,CvRect selection)
{
	cvSetImageROI(m_img_src, selection);
	IplImage* img_copy = cvCreateImage(cvGetSize(m_img_src),
		m_img_src->depth,
		m_img_src->nChannels);
	cvCopy(m_img_src, img_copy, NULL);
	cvResetImageROI(m_img_src);
	return img_copy;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
�����������̣�
��������� Դͼ���src����
����ͼ��ȫ��ͼ��m_img_VG
����ֵ����m_img_VG�ĸ�������ʱ����û�ã���ͷ��
���̣���src�������л�������Ķ�ֵ���������õ������ֵ��ͼ��m_img_VG;
*/
IplImage* CarPlatePosition::texture(IplImage *img_src)    //����������-����������ȡ
{
	IplImage* img_gray=cvCreateImage(cvGetSize(img_src),IPL_DEPTH_8U,1);	
	IplImage* p_img_VG=cvCreateImage(cvGetSize(img_gray),IPL_DEPTH_8U,1);
	cvCvtColor(img_src,img_gray,CV_BGR2GRAY);//��ɫͼ��ת��Ϊ�Ҷ�ͼ��
	int row=img_gray->height;
	int col=img_gray->width;
	int m,n;
	float ave_VG=0;
	float sum=0;
	int height=p_img_VG->height;
	int width=p_img_VG->width;

	int step= img_gray->widthStep/sizeof(uchar);   //sizeof(uchar)��1
	uchar* data=(uchar*)img_gray->imageData;

	int step_VG = p_img_VG->widthStep/sizeof(uchar);
	uchar* data_VG=(uchar*)p_img_VG->imageData;

	for(int i=1;i<row-1;i++)   //
		for(int j=1;j<col-1;j++)
		{
			int a,b,c;
			a=((uchar*)(img_gray->imageData+i*img_gray->widthStep))[j];
			b=((uchar*)(img_gray->imageData+i*img_gray->widthStep))[j-1];
			c=((uchar*)(img_gray->imageData+i*img_gray->widthStep))[j+1];
			m=abs(a-b);  //abs:�����ֵ
			n=abs(a-c);

			((uchar*)(p_img_VG->imageData+i*p_img_VG->widthStep))[j]= m + n;
		}

		for(int i1=1;i1<row-1;i1++)
			for(int j1=1;j1<col-1;j1++)
				sum=sum+data_VG[i1*step_VG+j1]; //���غͣ�Ϊ��ƽ��ֵ

		ave_VG =2*sum/(row*col);
		for(int i=1;i<row-1;i++)
			for(int j=1;j<col-1;j++)
			{
				if(data_VG[i*step_VG+j]>ave_VG)
					data_VG[i*step_VG+j]=255;
				else
					data_VG[i*step_VG+j]=0;
			}
			m_img_VG=p_img_VG;
			cvSmooth(m_img_VG,m_img_VG,CV_MEDIAN,3);
			cvReleaseImage(&img_gray);
			return p_img_VG;
}

//��ɫת��  ��һ��
//��������ͨ��........  �о���ɫת�������Ը�����   Ŀ��Ϊ�˵õ�HSVͼ�������ͨ����H,S,V��
/*
����ͼ��src����
����ͼ��ȫ��ͼ��m_img_blue
����ֵ����
���̣���src�������л���HSV���ƵĶ�ֵ���������õ���ֵ��ͼ��m_img_blue
*/
void CarPlatePosition::Color_Trans(IplImage *img_src) //RGB�ռ�ת��ΪHSV��ɫ��H,���Ͷ�S,͸����V��
{
	int row = img_src->height;
	int col = img_src->width;

	m_img_H = cvCreateImage(cvSize(img_src->width,img_src->height),IPL_DEPTH_64F,1);
	m_img_S = cvCreateImage(cvSize(img_src->width,img_src->height),IPL_DEPTH_64F,1);
	m_img_V = cvCreateImage(cvSize(img_src->width,img_src->height),IPL_DEPTH_64F,1);
	m_img_blue=cvCreateImage(cvSize(img_src->width,img_src->height),IPL_DEPTH_8U,1);
	if(img_src->nChannels!=3)
	{
		//MessageBoxA("��ѡ��Ĳ��ǲ�ɫͼ����ѡ���ɫͼ��!","");
		exit(1); //�������ʱ����1
	}
	//����ԭʼͼ�񸱱�, ��������ͷ�
	IplImage* img_src1=cvCreateImage(cvSize(img_src->width,img_src->height),IPL_DEPTH_64F,3);
	int step= img_src->widthStep/sizeof(uchar);
	int step1=img_src1->widthStep/sizeof(double);
	uchar*  data=(uchar*)img_src->imageData;
	double* dataimage=(double*)img_src1->imageData;

	for(int i=0;i<row;i++)//��ԭͼ���ֵ��ֵ���½���ͼ�񣬸�ֵ����Ϊʲô����cvCopy��
	{
		for(int j=0;j<col;j++)
		{
			dataimage[i*step1+j*3+0]=data[i*step+j*3+0];
			dataimage[i*step1+j*3+1]=data[i*step+j*3+1];
			dataimage[i*step1+j*3+2]=data[i*step+j*3+2];
		}
	}
	for(int i=0;i<row;i++)
	{
		for(int j=0;j<col;j++)
		{
			double x,y,t,t1;

			y=std::max((double)dataimage[i*step1+j*3+0],(double)dataimage[i*step1+j*3+1]);
			y=std::max((double)y,(double)dataimage[i*step1+j*3+2]);
			x=std::min((double)dataimage[i*step1+j*3+0],(double)dataimage[i*step1+j*3+1]);
			x=std::min((double)x,(double)dataimage[i*step1+j*3+2]);
			((double*)(m_img_V->imageData+i*m_img_V->widthStep))[j]=y/255;
			if(y==0)
				((double*)(m_img_S->imageData+i*m_img_S->widthStep))[j] = 0;
			else
				((double*)(m_img_S->imageData+i*m_img_S->widthStep))[j] = (y-x)/(0.0001+y);//�˴���������ڴ��ȡʧ�ܵ���y=0�����
			t=y-x;

			if(t!=0)
			{
				if((((double*)(m_img_S->imageData+i*m_img_S->widthStep))[j]!=0)&&(y == dataimage[i*step1+j*3+0]))
					//double *p = (double*)(S->imageData+i*S->widthStep);
					// if((p[j]!=0)&&(y==dataimage[i*step+j*3+0]))
				{
					t1=dataimage[i*step1+j*3+2]-dataimage[i*step1+j*3+1];
					((double*)(m_img_H->imageData+i*m_img_H->widthStep))[j] = 60*(4+t1/t);
				}
				else if((((double*)(m_img_S->imageData+i*m_img_S->widthStep))[j]!=0)&&(y==dataimage[i*step1+j*3+1]))
				{
					t1=dataimage[i*step1+j*3+0]-dataimage[i*step1+j*3+2];
					((double*)(m_img_H->imageData+i*m_img_H->widthStep))[j] = 60*(2+t1/t);

				}
				else if((((double*)(m_img_S->imageData+i*m_img_S->widthStep))[j]!=0)&&(y==dataimage[i*step1+j*3+2]))
				{
					t1=dataimage[i*step1+j*3+1]-dataimage[i*step1+j*3+0];
					((double*)(m_img_H->imageData+i*m_img_H->widthStep))[j] = 60*(t1/t);
				}
				else
					((double*)(m_img_H->imageData+i*m_img_H->widthStep))[j] = 0;
			}
		}
	}

	for(int i=0;i<row;i++)
		for(int j=0;j<col;j++)
		{
			//m_img_H��m_img_S��m_img_V������ȫ�ֱ���������ɫ�ʿռ�ת����õ��ĵ�ͨ��ͼ��
			double value_H = ((double*)(m_img_H->imageData+i*m_img_H->widthStep))[j];
			double value_S = ((double*)(m_img_S->imageData+i*m_img_S->widthStep))[j];
			double value_V = ((double*)(m_img_V->imageData+i*m_img_V->widthStep))[j];

			/*
			            ��HSV�ռ��ͼ�񣬰���Hͨ������ɫ���ж�ֵ������
			*/
			if((value_S>0.1)&&(value_H>200)&&(value_H<250))  //��ɫ          ������׳��Ƶ��ص㶨��Ĳ��� 0.1  200   250
				((uchar*)(m_img_blue->imageData+i*m_img_blue->widthStep))[j] = 255; //
			else
				((uchar*)(m_img_blue->imageData+i*m_img_blue->widthStep))[j] = 0;
		}
	cvSmooth(m_img_blue,m_img_blue,CV_MEDIAN,3);
//	DrawPicToHDC(m_img_blue,IDC_HSVERZHIHUA);
	cvReleaseImage(&img_src1);
}

//��������vp_detect�����������Ӻ��������Բ���ֱ�Ӳ��ԣ���Ӳ��Լ��ɣ�Ҫ�������������֮��Ľӿ�����ù�ϵ������ֲ�������ȫ�ֱ����ȹ�ϵ
//��װ�Ķ�һЩ��ͼ��������ȡ�Ĳ�����������Ϊ��vp_detect������һ���֣����Դ��β��ֶ���vp_detect���������ˣ���ɫ���ղ�����������ͼ�����������
//����ͼ����ȡ    ������
/*
����ͼ��ȫ�ֶ�ֵͼ��m_img_VG   ȫ�ֶ�ֵͼ��m_img_blue
����ͼ��ȫ�ֶ�ֵͼ��m_img_vp_bluewhite
����ֵ����
���̣��Դ��������ͼ���Ӧλ������ֵ��������������õ����ն�ֵ���Ľ��m_img_vp_bluewhite
*/

void CarPlatePosition::vp_blue_white( int istart_row, int istart_col, int iend_row, int iend_col) //��Ϊ���װ��ֳ��ƣ�����������ȡ
{
	//int m_iCount_bluewhite=0;
	int height = iend_row-istart_row+1;
	int width  = iend_col-istart_col+1;
	IplImage* m_img_b=cvCreateImage(cvSize(width,height),IPL_DEPTH_8U,1);
	IplImage* m_img_wl=cvCreateImage(cvSize(width,height),IPL_DEPTH_8U,1);

	cvCopy(m_img_blue,m_img_b);
	cvCopy(m_img_VG,m_img_wl);
	m_img_vp_bluewhite=cvCreateImage(cvSize(width,height),IPL_DEPTH_8U,1);//8λ��ͨ��ͼ��
	//	IplImage* m_img_white=cvCreateImage(cvSize(width,height),IPL_DEPTH_8U,1);//�������������ݽṹ������û�����ݣ����ǿյ�
	//	IplImage* m_img_blue=cvCreateImage(cvSize(width,height),IPL_DEPTH_8U,1);

	int step_vp= m_img_vp_bluewhite->widthStep;
	//int step_w = m_img_white->widthStep;  //��ɫ����
	//int step_b = m_img_blue->widthStep;   //��ɫ����                  ͼ��һ�����ص��ֽ���

	uchar* data_vp=(uchar*)m_img_vp_bluewhite->imageData;
	//uchar* data_w=(uchar*)m_img_white->imageData;
	//uchar* data_b=(uchar*)m_img_blue->imageData;

	for(int i=istart_row;i<=iend_row;i++)
		for(int j=istart_col;j< iend_col;j++)
		{
			if((((uchar*)(m_img_b->imageData+i*m_img_b->widthStep))[j]==255)&&(((uchar*)(m_img_wl->imageData+i*m_img_wl->widthStep))[j]==255))  //������ͼ������ɫ�������룬ȥ����������ȡ�����������ڴ˴�������ʱ�򣬺����Ѿ�ִ����ɫ�ʿռ�ת���Ȳ�����
			{
				data_vp[i*step_vp+j]=255;
				//	m_iCount_bluewhite++;
			}
			else
				data_vp[i*step_vp+j]=0;//��������˵��������������ģ�����0�Ŷԣ���Ȼ�޸ĵ���data_vp������m_img_vp_bluewhiteָ����ֵҲ���Ӧ�ı䣬��Ϊָ�����ͬһ����ַ��
		}
}
//ͶӰ������vp_dect�������Ӻ��������崫�봫�������ں���vp_detect�ж��壩
void CarPlatePosition::projection(IplImage *img, int istart_row, int iend_row, int istart_col, int iend_col, char s) //���к���ͶӰ�����¹��������Լ����ұ�Ե
{
	IplImage *img1=cvCreateImage(cvGetSize(img),IPL_DEPTH_64F,1);
	int row=img->height;
	int col=img->width;
	//������ص㿽��
	for(int i=0;i<row;i++)
		for(int j=0;j<col;j++)
		{
			((double*)(img1->imageData+i*img1->widthStep))[j]=((uchar*)(img->imageData+i*img->widthStep))[j];
		}

		//cvCopy(img,img1);

		if(s=='h')  //��ֱ����
		{
			vector<double> y(row,0.000000);
			//ˮƽ����ͶӰ
			for(int i=0;i<row;i++)
			{
				for(int j=istart_col;j<=iend_col;j++)
				{
					if (((double*)(img1->imageData+i*img1->widthStep))[j]==255)
					{
						y.at(i)=y.at(i)+1;
					}
				}
			}

			int tt_limit=iend_row-istart_row+1;
			int t_limit = 0;
			tt_limit=cvRound(tt_limit/3);  //����֮һ�ĸ߶ȣ�cvRound��һ��double�͵���������������,������һ��������
			//�ϱ�Ե�����������ϱ�Ե����������������������������Ͳ����������ˣ������ظ��������ì��

			bool bret1 = false;//�����־��������������Ͳ���������������
			if((istart_row-tt_limit)<=0)
				t_limit = 0;
			else
				t_limit = istart_row - tt_limit; //��������֮һλ�ô�
			int nNum=0;
			for(int i=istart_row;i>=t_limit;i--)
			{
				if(y.at(i)>ntop)  
				{
					istart_row = i;
					bret1  = true;
				}
			}
			m_iStart_row = istart_row;
			if(bret1==false)
				//�ϱ�Ե��������
			{
				if((istart_row+tt_limit)>row-1)//�ϱ߽紦�����⳵��ͼ��λ�ڵ�һ��
					t_limit = row-1;
				else
					t_limit = istart_row+tt_limit;

				for(int i=istart_row;i<=t_limit;i++)
				{
					if(y.at(i)>nOther)  
						istart_row = i;

				}
				m_iStart_row = istart_row;//�����ϱ�Ե
			}
			// �±�Ե��������

			bool bret2 = false;//�����־��������������Ͳ���������������
			if((iend_row+tt_limit)>row-1)
				t_limit = row-1;
			else
				t_limit = iend_row + tt_limit;
			for(int i=iend_row;i<=t_limit;i++)
			{
				if(y.at(i)>nOther)
				{
					iend_row = i;
					bret2 = true;
				}
			}
			m_iEnd_row = iend_row;

			//�±�Ե��������
			if(bret2==false)
			{
				if((iend_row - tt_limit)<0)
					t_limit = 0;
				else
					t_limit = iend_row - tt_limit;

				for(int i = iend_row;i>=t_limit;i--)
				{
					if(y.at(i)>nOther)
						iend_row = i;
				}
				m_iEnd_row = iend_row;
			}
		}
		if(s=='v')
		{
			vector<double>y(col,0.000000);
			for(int j=0;j<col;j++)
			{
				for(int i=istart_row;i<=iend_row;i++)
				{
					if (((double*)(img1->imageData+i*img1->widthStep))[j]==255) 
					{
						y.at(j)=y.at(j)+1;//��Ȼ����0����1����ֵ���Ľ��డ
					}
				}
			}

			int tt_limit = iend_col - istart_col + 1;
			int t_limit = 0;
			tt_limit = cvRound(tt_limit/10);

			//���Ե��������
			bool bret1=false;
			if((istart_col-tt_limit)<0)
				t_limit = 0;
			else
				t_limit = istart_col-tt_limit;

			for(int i=istart_col;i>=t_limit;i--)
			{
				if(y.at(i)>nOther)
				{
					istart_col = i;
					bret1 = true;
				}
			}
			m_iStart_col = istart_col;

			if(bret1==false)
				//���Ե��������
			{
				if((istart_col+tt_limit)>col-1)
					t_limit = col-1;
				else
					t_limit = istart_col + tt_limit;

				for(int i = istart_col;i<=t_limit;i++)
				{
					if(y.at(i)>nOther)
					{
						istart_col = i;
					}
				}
				m_iStart_col = istart_col;
			}

			//�ұ�Ե��������
			bool bret2 = false;
			if((iend_col+tt_limit)>col-1)
				t_limit = col-1;
			else
				t_limit = iend_col + tt_limit;

			for(int i=iend_col;i<=t_limit;i++)
			{
				if(y.at(i)>nOther)
				{
					iend_col = i;
					bret2  = true;
				}
			}
			m_iEnd_col = iend_col;

			//�ұ�Ե��������
			if(bret2==false)
			{
				if((iend_col-tt_limit)<0)
					t_limit=0;
				else
					t_limit=iend_col-tt_limit;
				int t=0;
				for(int i=iend_col;i>=t_limit;i--)
				{
					if(y.at(i)>nOther)
						iend_col = i;
				}
				m_iEnd_col = iend_col;
			}

		}
		cvReleaseImage(&img1);		
}

//��λ�����������ض�λ�ֽ�� 
//��Ҫ���ڷ��ض�λ���....
VP CarPlatePosition::vp_detect(int W_Width, int W_Height)
{
	VP m_PlateResult;
	//	int vp_class = 0;
	VP m_Vp_Region_bluewhite;

	int row=m_img_VG->height;//row������ͼ��ĸ߶ȣ������ǳ��Ʋ��ֳ��Ƶĸ߶�
	int col=m_img_VG->width;

	m_Vp_Region_bluewhite.initiapos_row = 0;
	m_Vp_Region_bluewhite.initiapos_col = 0;
	m_Vp_Region_bluewhite.end_row = 0;
	m_Vp_Region_bluewhite.end_col = 0;
	m_Vp_Region_bluewhite.count = 0;

	for(int i=0;i<(row - W_Height);i+=4)
		for(int j=0;j<(col - W_Width);j+=8)
		{
			int count1 = 0;//��ɫͼ������������������
			for(int m=0;m<=W_Height;m++)
			{
				for(int n=0;n<=W_Width;n++)
				{
					if(((uchar*)(m_img_vp_bluewhite->imageData+(i+m)*m_img_vp_bluewhite->widthStep))[j+n]==255)
						count1=count1+1;
				}
			}
			//ȷ���˴��ֲڽ�ͼ�����ͼ��ߴ�
			if(count1>m_Vp_Region_bluewhite.count)//�����������򣬴˴����޸ģ���Ϊʵ���г����Ҵ������Ϊ�ô������������������Ĵ������崦��ɸѡ��Ч����úܶ࣬���ٶȻ����
			{
				m_Vp_Region_bluewhite.count = count1;
				m_Vp_Region_bluewhite.initiapos_row = i;
				m_Vp_Region_bluewhite.initiapos_col = j;
				m_Vp_Region_bluewhite.end_row = i+W_Height;
				m_Vp_Region_bluewhite.end_col = j+W_Width;
			}
		}
		//�����������
		m_Vp_Region.initiapos_row = m_Vp_Region_bluewhite.initiapos_row;
		m_Vp_Region.initiapos_col = m_Vp_Region_bluewhite.initiapos_col;
		m_Vp_Region.end_row = m_Vp_Region_bluewhite.end_row;
		m_Vp_Region.end_col = m_Vp_Region_bluewhite.end_col;
		m_Vp_Region.count = m_Vp_Region_bluewhite.count;

		//����ȫ�ֶ�ֵͼ��m_img_vp_bluewhite�ĸ���pic
		IplImage* m_pic = cvCreateImage(cvGetSize(m_img_vp_bluewhite),IPL_DEPTH_8U,1);
		cvCopy(m_img_vp_bluewhite,m_pic);

		//////////////////////////////////////////////�����Ǿ�ȷ��λ����////////////////////////////////////////////
		int start_row_ref = 0;
		int start_col_ref = 0;
		int end_row_ref = 0;
		int end_col_ref = 0;
		bool bret = true;
		int intertimes = 30;//�޶�����������Ϊ30�Σ�����������ѭ��
		while((bret)&&(intertimes>0))
		{
			intertimes--;
			intertimes = intertimes - 1 ;
			start_row_ref = m_Vp_Region.initiapos_row ;
			end_row_ref = m_Vp_Region.end_row ;
			start_col_ref = m_Vp_Region.initiapos_col ;
			end_col_ref = m_Vp_Region.end_col ;

			projection(m_pic,m_Vp_Region.initiapos_row,m_Vp_Region.end_row,m_Vp_Region.initiapos_col,m_Vp_Region.end_col,'v');//����ͶӰ�����¹���������ֱ��Ե
			m_Vp_Region.initiapos_col = m_iStart_col;
			m_Vp_Region.end_col = m_iEnd_col;

			projection(m_pic,m_Vp_Region.initiapos_row,m_Vp_Region.end_row,m_Vp_Region.initiapos_col,m_Vp_Region.end_col,'h');//����ͶӰ�����¹�������ˮƽ��Ե����ʱʹ�õĴ�ֱ��ԵΪ���º�ı�Ե
			m_Vp_Region.initiapos_row = m_iStart_row;
			m_Vp_Region.end_row = m_iEnd_row;

			bret =((m_Vp_Region.initiapos_row!=start_row_ref)||(m_Vp_Region.end_row!=end_row_ref)||(m_Vp_Region.initiapos_col!=start_col_ref)||(m_Vp_Region.end_col!=end_col_ref));//�ж�ͶӰǰ���Ե�Ƿ�仯������仯����ѭ����������ֹ
		}

		CvPoint p1,p2,p3,p4; //��λ��ϸ���
		p1.x=m_Vp_Region.initiapos_col;
		p1.y=m_Vp_Region.initiapos_row;
		p2.x=m_Vp_Region.end_col;
		p2.y=m_Vp_Region.initiapos_row;
		p3.x=m_Vp_Region.end_col;
		p3.y=m_Vp_Region.end_row;
		p4.x=m_Vp_Region.initiapos_col;
		p4.y=m_Vp_Region.end_row;    

		//һ���Խ��д���   ���ô�������ֱ�Ӱ�ԭͼ��ͷ����ˣ�Ӧ���ø�switch����
		//if (ONE_PART==TRUE)
		//{
			cvRectangle(srcPic,p1,p3,Scalar(0,0,255),1,8,0);  //srcPicû��ֵ
			roiThree = cut_Roi_three(srcPic,p1,p3.x-p1.x,p3.y-p1.y);
		//}
		//else
		//{
			//IplImage* src = cvCreateImage(cvGetSize(one_img),8,3);
		//	cvCopy(one_img,src);
		//	Color_Trans(src);
			//�˴�����һ������ĳ��ƶ�λ��ش���
		//	cvRectangle(one_img,p1,p3,Scalar(0,0,255),1,8,0);
		//	roiThree = cut_Roi_three(one_img,p1,p3.x-p1.x,p3.y-p1.y);
		//}

		m_PlateResult=m_Vp_Region;
		return m_PlateResult;
}
VP CarPlatePosition::plate_segment(IplImage *src_img) //��λ����
{
	int width_big;
	int height_big;
	int width_small;
	int height_small;
	spd.InitText();
	if (insearchboxString=="40*10")
	{
		width_small =40;
		height_small=10;
		width_big=52;
		height_big=13;
		
	} 
	else if(insearchboxString=="60*15")
	{
		width_small =60;
		height_small=15;
		width_big=77;
		height_big=19;
	}
	else if(insearchboxString=="80*20")
	{
		width_small =80;
		height_small=20;
		width_big=102;
		height_big=26;
	}
	else if(insearchboxString=="100*25")
	{
		width_small =100;
		height_small=25;
		width_big=128;
		height_big=32;
	}
	else if(insearchboxString=="120*30")
	{
		width_small =120;
		height_small=30;
		width_big=154;
		height_big=38;
	}
	else if(insearchboxString=="140*35")
	{
		width_small =140;
		height_small=35;
		width_big=180;
		height_big=45;
	}

	m_Vp_Region.initiapos_row = 0;
	m_Vp_Region.initiapos_col = 0;
	m_Vp_Region.count = 0;
	//�����������С
	int W_Width;
	int W_Height;
	if (src_img->height<600)
	{
		W_Width = width_small;
		W_Height = height_small;
		ntop=2;
		nOther=2;
	}
	else
	{
		W_Width =width_big;
		W_Height = height_big;
		ntop=30;
		nOther=5;
	}
	return vp_detect(W_Width,W_Height);   
}

//��ͨ��ɸѡ
bool CarPlatePosition::pre_check(vector <Hei_sta> vt_Connect,IplImage* src_image, IplImage* image_sign, IplImage* dst_image, IplImage* image_sign_ref)
{
	//
	//src_imageΪ��ֵͼ��image_signΪ��ͨ��ͼ��
	//������ͨ����λ�����е��ַ�ʵ�ֶԳ��Ƶķ���λ
	//1��ȥ��С��ƽ����ȴ���1/7����ͨ��
	//1��ͳ��ͨ�����ĵ�ÿ����ͨ��ĸ߶�
	//2��ȥ��С��ƽ���߶�1/3����ͨ��

	if (src_image->nChannels!=1)
		exit(1);

	//���г�ʼ��
	int row = src_image->height;
	int col = src_image->width;
	int a=0;
	///////////////////// /*ȥ���߶ȴ���1/7����ͨ��*/
	for(vector<Hei_sta>::iterator iter=vt_Connect.begin(); iter!=vt_Connect.end(); )
	{
		if( iter->width>col/7.0||iter->height<row/3)
		{
			for (int m=0;m<row;m++)
			{
				for (int n=0;n<col;n++)
				{
					if (((double*)(image_sign->imageData+m*image_sign->widthStep))[n]==iter->sign)
						((double*)(image_sign->imageData+m*image_sign->widthStep))[n]=0;
				}
			}
			iter = vt_Connect.erase(iter);
		}
		else
			iter ++ ;
	}	
	int stastic_count = 7;
	int height_thre = 5;
	int max_top_bot_thre = 30;
	///////////////////// /*ȥ��δͨ�����ĵ���ͨ��*/
	int row_center = 0;
	int col_center = 0;
	int pos_count = 0;

	for (int i=0;i<row;i++)
	{
		for (int j=0;j<col;j++)
		{
			if (((double*)(image_sign->imageData+i*image_sign->widthStep))[j]>0)
			{
				row_center = row_center + i;
				col_center = col_center + j;
				pos_count = pos_count + 1;
			}
		}
	}

	if ((row_center==0)||(pos_count==0))
	{
		dst_image = NULL;
		image_sign_ref = NULL;
		return false;
	}
	row_center = row_center/pos_count;  //�е�����
	col_center = col_center/pos_count;  //�е�����
	//��ͼ����0
	cvZero(dst_image);
	cvZero(image_sign_ref);
	for (int j=0;j<col;j++)
	{
		if (((double*)(image_sign->imageData+row_center*image_sign->widthStep))[j]>0)
		{
			int tmp_sign = ((double*)(image_sign->imageData+row_center*image_sign->widthStep))[j];  //�������ϵ�����
			for (int m = 0;m<row;m++)
			{
				for (int n=0;n<col;n++)
				{
					if (((double*)(image_sign->imageData+m*image_sign->widthStep))[n]==tmp_sign)//���������ĵ����ͨ��
					{
						((uchar*)(dst_image->imageData+m*dst_image->widthStep))[n]=((uchar*)(src_image->imageData+m*src_image->widthStep))[n];
						((double*)(image_sign_ref->imageData+m*image_sign_ref->widthStep))[n]=((double*)(image_sign->imageData+m*image_sign->widthStep))[n];
					}
				}
			}
		}
	}
	return true;
}

//�ַ��ָ��
//�˴��Ĵ�������ǳ���ͼ��  ���ƵĲ�ɫͼ�� roiThressͼ��
void  CarPlatePosition::PlateCharSegment(IplImage *pImagePlate)
{
	IplImage* m_img_plate = cvCreateImage(cvGetSize(pImagePlate),pImagePlate->depth,pImagePlate->nChannels);
	int thresthold=inplatethresholdInt*2.5;//���ļ����복�ƶ�ֵ������ֵ
	//�õ���������m_Vp_Region
	if (pImagePlate->width<500)//����ͼ��
		m_img_plate=CopyIplImage(pImagePlate);
	else
	{
		CvPoint p1,p2,p3,p4;
		p1.x=m_Vp_Region.initiapos_col;
		p1.y=m_Vp_Region.initiapos_row;
		p2.x=m_Vp_Region.end_col;
		p2.y=m_Vp_Region.initiapos_row;
		p3.x=m_Vp_Region.end_col;
		p3.y=m_Vp_Region.end_row;
		p4.x=m_Vp_Region.initiapos_col;
		p4.y=m_Vp_Region.end_row;
		int vp_height = p4.y-p1.y;
		int vp_width = p3.x-p4.x;	
		CvRect selection;
		selection.x = p1.x;
		selection.y = p1.y;
		selection.width = p2.x-p1.x;
		selection.height = p4.y-p1.y;
		//m_img_plate= CopyIplImageROI(src,selection);	//�˴���m_img_srcӦ��Ϊ��ԭʼ��ͼ��ͼ��
		//if (ONE_PART==TRUE)
		//{
			m_img_plate= CopyIplImageROI(srcPic,selection);
		//} 
// 		else
// 		{
// 			IplImage* src = cvCreateImage(cvGetSize(one_img),8,3);
// 			cvCopy(one_img,src);
// 			//�˴�����һ���������ش���
// 			m_img_plate= CopyIplImageROI(src,selection);
// 		}		
	} 

	IplImage* vp_images = m_img_plate;
	IplImage* vp_images_gray = cvCreateImage(cvGetSize(vp_images),IPL_DEPTH_8U,1);
	cvCvtColor(vp_images,vp_images_gray,CV_BGR2GRAY);//vp_imagesΪԴͼ��vp_images_grayΪĿ��ͼ��

	vp_images_bin = cvCreateImage(cvGetSize(vp_images),IPL_DEPTH_8U,1);
	cvThreshold(vp_images_gray,vp_images_bin,thresthold,255,CV_THRESH_BINARY);

	//������ֵͼ��
	//m_PlateResult.pImagePlateBin=CopyIplImage(vp_images_bin);

	IplImage* vp_images_precise = cvCreateImage(cvGetSize(vp_images_bin),IPL_DEPTH_8U,1);
	IplImage* vp_images_sign = cvCreateImage(cvGetSize(vp_images_bin),IPL_DEPTH_64F,1);      //��ͨ����ͼ��        
    vp_images_sign_ref = cvCreateImage(cvGetSize(vp_images_bin),IPL_DEPTH_64F,1);
	//��ͨ������ ������ͨ��ṹ
	vector <Hei_sta> vt_ConnectDom=cfc.find_connected_domain( vp_images_bin,vp_images_sign); 

	pre_check(vt_ConnectDom,vp_images_bin,vp_images_sign,vp_images_precise,vp_images_sign_ref);
}

void CarPlatePosition::plateSegment()
{
	//ͼ�����ɫ�ʿռ�ת��
	IplImage* src_color_trans = cvCreateImage(cvGetSize(srcPic),8,3);
	cvCopy(srcPic,src_color_trans);
	Color_Trans(src_color_trans);

	//ͼ�����������
	IplImage* src_texture = cvCreateImage(cvGetSize(srcPic),8,3);
	cvCopy(srcPic,src_texture);
	texture(src_texture);

	//���׳���ʶ��������
	int row=m_img_VG->height;//row������ͼ��ĸ߶ȣ������ǳ��Ʋ��ֳ��Ƶĸ߶�
	int col=m_img_VG->width;
	vp_blue_white(0,0,row-1,col-1); 

	plate_segment(srcPic);

	PlateCharSegment(roiThree);
}

void CarPlatePosition::plateSegmentOne()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//����ͼƬ
// 	string dir_path = "D:\cutpic\\";
// 	Directory dir;
// 	vector<string> filename = dir.GetListFiles(dir_path,"*.jpg",false);
// 
// 	for(int i = 0;i<filename.size();i++)
// 	{
// 		string fileName = filename[i];
// 		string fileFullName = dir_path+fileName;
// 		image = imread(fileFullName,1);
		//Matת����IplImage

		//srcPic_One = &IplImage(image);

		IplImage* src_color_trans = cvCreateImage(cvSize(pPic->width,pPic->height),pPic->depth,pPic->nChannels);
		cvCopy(pPic,src_color_trans);
		cvShowImage("xxx",src_color_trans);
		Color_Trans(src_color_trans);

		IplImage* src_texture = cvCreateImage(cvGetSize(pPic),8,3);
		cvCopy(pPic,src_texture);
		texture(src_texture);

		//���ƾ�ȷ��λ
		int row=m_img_VG->height;//row������ͼ��ĸ߶ȣ������ǳ��Ʋ��ֳ��Ƶĸ߶�
		int col=m_img_VG->width;
		vp_blue_white(0,0,row-1,col-1); 

		plate_segment(pPic);

		PlateCharSegment(roiThree);

// 		waitKey(1000);
// 	}
}
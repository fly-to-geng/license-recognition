#include "StdAfx.h"
#include "CarFlowCount.h"
#include "cv.h" //  OpenCV �Ļ�������ͷ�ļ�  
#include"highgui.h" //OpenCV ��ͼ����ʾ����ͷ�ļ�  
#include "CvvImage.h" 
#include "vector"
#include "CarPlatePosition.h"
#include <math.h>
#include<fstream>
#include<iostream>
#include<cmath>
#include "SetPreferencesDlg.h"
#include "opencvtestDlg.h"
#include <vector>
#include "resource.h"
using namespace cv;
using namespace std;

extern int nnPosNum;
extern int thickness_scale2;
//extern BOOL ONE_FLOW;
CarPlatePosition Cpp;
string dir_path;
Directory dir;
CString strTmp2;
char img_name[100];  

int flag=0;
/************************�ļ��еĲ�����ȡ��DLG�еĲ������壬����ȫ�ֱ���*************************/
//ComBox����
string inlaneText2;//�����ַ���
string inareaText2;//���Ƶ����ַ���
string insearchboxText2;//�ֶ�λ�������ַ���

//CStrilBar����
string insprpiexlText2;//���ز�
string inzoomText2;//���ű���
string inthresholdText2;//��ֵ
string innocarpiexlText2;//�޳�����ֵ
string inplatethresholdText2;//���ƶ�ֵ����ֵ

//�Զ���·������
string inplatepathText2;//����·��
string inresultpathText2;//���·��
string incutpicpathText2;//��ͼ·��


//--------------------------------------��������----------------------------------------
float scale = 0.0;  //���ű���Ϊ0.5��  
int scale_scale;
int threshold_num=0;//��ֵ��ʱ�����ֵ
int diff=0;//������֮֡������ز�ֵ��
int noCar=0;//ƽ��ֵ�������ֵʱ����Ϊû�г���ͨ��
float height_scale=0.5;//���Ƽ���ߵ�λ��
CvPoint lefttop;//�����������Ͻ�
int cardoor=2;//������Ŀ
int s0=0,s1=0,s2=0 ;//������¼ƽ���������ڵ�����ֵ��
int s3=0,s4=0,s5=0;
int s6=0,s7=0,s8=0;
int s9=0,s10=0,s11=0;
//------------------------------------------------------------------------------------------
//CvCapture* capture = NULL;
//��ǰ֡
IplImage* pFrImg = NULL;  
IplImage* pBkImg = NULL;
CvMat* pFrameMat = NULL;  
CvMat* pFrMat = NULL;  
CvMat* pBkMat = NULL;
int nFrmNum = 0;  //��ǰ��֡��Ŀ
int vfps=0;
IplImage *frame; //��ǰ֡
IplImage* big_frame;
CvCapture *capture ;
static int car_num=0;
IplImage* pPic;
IplImage* exPic;
IplImage* zonePic;//��Ż��߲��ֽ�ȡ������ͼƬ
CarPlatePosition cpl;
//CarPlatePosition cpl;
extern CvCapture* pCapture;
//struct Hei_sta car[4]={{0,0,0,0,0,0,0},{0,0,0,0,0,0,0},{0,0,0,0,0,0,0},{0,0,0,0,0,0,0}};
//struct Hei_sta car_temp[4] = {{0,0,0,0,0,0,0},{0,0,0,0,0,0,0},{0,0,0,0,0,0,0},{0,0,0,0,0,0,0}};
		 
CarFlowCount::CarFlowCount(void)
{
	   
}

CarFlowCount::~CarFlowCount(void)
{
}

//��ʼ������
void InitText()
{
	//�ļ��в�������
	ifstream inf("./out.txt");
	inf>>inlaneText2>>inareaText2>>insearchboxText2>>insprpiexlText2>>inzoomText2>>inthresholdText2>>innocarpiexlText2>>inplatethresholdText2
		>>inplatepathText2>>inresultpathText2>>incutpicpathText2;

	cardoor = atoi(inlaneText2.c_str());
	diff=atoi(insprpiexlText2.c_str());
	scale_scale=atof(inzoomText2.c_str());
	scale = scale_scale/100.0;
	threshold_num=atoi(inthresholdText2.c_str());
	noCar=atoi(innocarpiexlText2.c_str());
}
//����ͳ�ƺ���
void carDoor1(){
	CarFlowCount cfc;
	lefttop.x = 0;
	height_scale = pFrImg->height*(nnPosNum/100.0);
	lefttop.y = height_scale;
    int thickness=(int)(pFrImg->height*(thickness_scale2/400.0) );//ϸ����ϸ
	int sum = 0;//�������غ�
	int count = 0;//�������ظ���

	int avg = 0;//��������ƽ��ֵ
	
	uchar tmp=0;

	IplImage* img=cvCreateImage(cvSize(pFrImg->width,pFrImg->height),IPL_DEPTH_8U,1);
	uchar* data=(uchar *)pFrImg->imageData;
	int step = pFrImg->widthStep/sizeof(uchar);

	for(int i=lefttop.y;i<(lefttop.y+thickness);i++)
	{
		for(int j=0;j<pFrImg->width;j++)
		{ 
			tmp=data[i*step+j];
			sum = sum + tmp;
			count++;
		}
	}
	if(count!=0){	
		avg = sum /count;
	}

	s2 = avg;
	if(s2>diff){//�����г�����
		if(s2-s1>diff&&s1<20&&s2>diff&&s0<noCar){
			strTmp2.Format(_T("������Ŀ:  %d"),++car_num);
			IplImage* temPic;
			temPic = cvCreateImage(cvSize(big_frame->width, big_frame->height),big_frame->depth,big_frame->nChannels); 
			cvCopy(big_frame,temPic);
			pPic = cfc.cutRoi(temPic,cvPoint(0,lefttop.y),temPic->width,temPic->height*0.4);

			CopencvtestDlg cpd;
			cpd.searchDirectry();
			//��ȡ���̵�ǰ·��
			//char strExePath[_MAX_PATH];
			//GetModuleFileName(NULL,strExePath,_MAX_PATH);
			//PathRemoveFileSpec(strExePath);

			//char subfilename[50]="\\cutPic";					
			//char* filename= strcat(strExePath,subfilename);
			//���泵��ͼ�񵽱����ļ���
			CreateDirectory("../Picture/", NULL);
			
			sprintf(img_name,"../Picture/%d.jpg",car_num); 
			cvSaveImage(img_name,pPic);

			///if (ONE_FLOW==TRUE)
			//{
				//Cpp.plateSegmentOne();
				cvNamedWindow("������ͼ",CV_WINDOW_AUTOSIZE);     
				cvShowImage("������ͼ", pPic);   
				cvReleaseImage(&temPic); 
			//} 

			//Cpp.plateSegmentOne();
		
		}
	}
	s0=s1;
	s1=s2;
}
void carDoor2(){
	CarFlowCount cfc;
	lefttop.x = 0;
	lefttop.y = (int)(pFrImg->height*height_scale);
	int thickness=(int)(pFrImg->height*0.02 );//ϸ����ϸ
	int sum = 0;//�������غ�
	int sum2=0;
	int count = 0;//�������ظ���
	int count2=0;
	int avg = 0;//��������ƽ��ֵ
	int avg2=0;
	uchar tmp=0;
	uchar tmp2 = 0;
	IplImage* img=cvCreateImage(cvSize(frame->width,frame->height),IPL_DEPTH_8U,1);
	uchar* data=(uchar *)pFrImg->imageData;
	int step = pFrImg->widthStep/sizeof(uchar);

	for(int i=lefttop.y;i<(lefttop.y+thickness);i++)
	{
		for(int j=0;j<frame->width/2;j++)
		{ 
			tmp=data[i*step+j];
			sum = sum + tmp;
			count++;
		}
	}
	if(count!=0){	
		avg = sum /count;
	}

	s2 = avg;
	if(s2>diff){//�����г�����
		if(s2-s1>diff&&s1<20&&s2>diff&&s0<noCar){
			strTmp2.Format(_T("������Ŀ:  %d"),++car_num);
			IplImage* temPic;
			temPic = cvCreateImage(cvSize(big_frame->width, big_frame->height),big_frame->depth,big_frame->nChannels); 
			cvCopy(big_frame,temPic);
			pPic = cfc.cutRoi(temPic,cvPoint(0,lefttop.y),temPic->width/2,temPic->height*0.4);


			 //dir_path = "../Picture/";
			//Directory dir;
			//CopencvtestDlg cpd;
			//cpd.searchDirectry();

			CreateDirectory("../Picture/", NULL);

			sprintf(img_name,"../Picture/%d.jpg",car_num); 
			cvSaveImage(img_name,pPic);


			//if (ONE_FLOW==TRUE)
		//	{
				//Cpp.plateSegmentOne();
				cvNamedWindow("��೵����ͼ",CV_WINDOW_AUTOSIZE);     
				cvShowImage("��೵����ͼ", pPic);   
				cvReleaseImage(&temPic); 

		}
	}
	s0=s1;
	s1=s2;




	for(int i=lefttop.y;i<lefttop.y+thickness;i++)
	{
		for(int j=frame->width/2;j<frame->width;j++)
		{
			tmp2=data[i*step+j];
			sum2 = sum2 + tmp2;
			count2++;
		}
	}
	if(count2!=0){avg2 = sum2/count2;}
	s4 = avg2;
	if(s4>diff){//�����г�����
		if(s4-s3>diff&&s3<20&&s4>diff&&s5<noCar){
			strTmp2.Format(_T("������Ŀ:  %d"),++car_num);
			IplImage* temPic;
			temPic = cvCreateImage(cvSize(big_frame->width, big_frame->height),big_frame->depth,big_frame->nChannels); 
			cvCopy(big_frame,temPic);
			pPic = cfc.cutRoi(temPic,cvPoint(temPic->width/2,lefttop.y),temPic->width/2,temPic->height*0.4);

			//CopencvtestDlg cpd;
			//cpd.searchDirectry();

			CreateDirectory("../Picture/", NULL);

			sprintf(img_name,"../Picture/%d.jpg",car_num); 
			cvSaveImage(img_name,pPic);
			////if (ONE_FLOW==TRUE)
			//{
				//Cpp.plateSegmentOne();
				cvNamedWindow("�Ҳ೵����ͼ",CV_WINDOW_AUTOSIZE);     
				cvShowImage("�Ҳ೵����ͼ", pPic);   
				cvReleaseImage(&temPic); 
		//	} 
		}
	}
	s5=s3;
	s3=s4;
}
void carDoor3(){
	CarFlowCount cfc;
	lefttop.x = 0;
	lefttop.y = (int)(pFrImg->height*height_scale);
	int thickness=(int)(pFrImg->height*0.02 );//ϸ����ϸ
	int sum = 0;//�������غ�
	int sum2=0;
	int sum3=0;
	int count = 0;//�������ظ���
	int count2=0;
	int count3=0;
	int avg = 0;//��������ƽ��ֵ
	int avg2=0;
	int avg3=0;
	uchar tmp=0;
	uchar tmp2 = 0;
	int tmp3 = 0;
	IplImage* img=cvCreateImage(cvSize(frame->width,frame->height),IPL_DEPTH_8U,1);
	uchar* data=(uchar *)pFrImg->imageData;
	int step = pFrImg->widthStep/sizeof(uchar);

	for(int i=lefttop.y;i<(lefttop.y+thickness);i++)
	{
		for(int j=0;j<frame->width/3;j++)
		{ 
			tmp=data[i*step+j];
			sum = sum + tmp;
			count++;
		}
	}
	if(count!=0){	
		avg = sum /count;
	}
	s2 = avg;
	if(s2>diff){//�����г�����
		if(s2-s1>diff&&s1<20&&s2>diff&&s0<noCar){
			strTmp2.Format(_T("������Ŀ:  %d"),++car_num);
			IplImage* temPic;
			temPic = cvCreateImage(cvSize(big_frame->width, big_frame->height),big_frame->depth,big_frame->nChannels); 
			cvCopy(big_frame,temPic);
			pPic = cfc.cutRoi(temPic,cvPoint(0,lefttop.y),temPic->width/3,temPic->height*0.4);

		//	CopencvtestDlg cpd;
		//	cpd.searchDirectry();

			CreateDirectory("../Picture/", NULL);

			sprintf(img_name,"../Picture/%d.jpg",car_num); 
			cvSaveImage(img_name,pPic);
			//Cpp.plateSegmentOne();
			//if (ONE_FLOW==TRUE)
			//{
				//Cpp.plateSegmentOne();
				cvNamedWindow("��೵����ͼ",CV_WINDOW_AUTOSIZE);     
				cvShowImage("��೵����ͼ", pPic);   
				cvReleaseImage(&temPic); 
			//} 
		}
	}
	s0=s1;
	s1=s2;




	for(int i=lefttop.y;i<lefttop.y+thickness;i++)
	{
		for(int j=frame->width/3;j<(2*frame->width)/3;j++)
		{
			tmp2=data[i*step+j];
			sum2 = sum2 + tmp2;
			count2++;
		}
	}
	if(count2!=0){avg2 = sum2/count2;}
	s4 = avg2;
	if(s4>diff){//�����г�����
		if(s4-s3>diff&&s3<20&&s4>diff&&s5<noCar){
			strTmp2.Format(_T("������Ŀ:  %d"),++car_num);
			IplImage* temPic;
			temPic = cvCreateImage(cvSize(big_frame->width, big_frame->height),big_frame->depth,big_frame->nChannels); 
			cvCopy(big_frame,temPic);
			pPic = cfc.cutRoi(temPic,cvPoint(temPic->width/3,lefttop.y),(temPic->width)/3,temPic->height*0.4);

		//	CopencvtestDlg cpd;
		//	cpd.searchDirectry();

			CreateDirectory("../Picture/", NULL);

			sprintf(img_name,"../Picture/%d.jpg",car_num); 
			cvSaveImage(img_name,pPic);

			//if (ONE_FLOW==TRUE)
			//{
				//Cpp.plateSegmentOne();
				cvNamedWindow("�м䳵����ͼ",CV_WINDOW_AUTOSIZE);     
				cvShowImage("�м䳵����ͼ", pPic);   
				cvReleaseImage(&temPic); 
		}
	}
	s5=s3;
	s3=s4;



	for(int i=lefttop.y;i<lefttop.y+thickness;i++)
	{
		for(int j=(2*frame->width)/3;j<frame->width;j++)
		{
			tmp3=data[i*step+j];
			sum3 = sum3 + tmp3;
			count3++;
		}
	}
	if(count3!=0){avg3 = sum3/count3;}
	s7 = avg3;
	if(s7>diff){//�����г�����
		if(s7-s6>diff&&s6<20&&s7>diff&&s8<noCar){
			strTmp2.Format(_T("������Ŀ:  %d"),++car_num);
			IplImage* temPic;
			temPic = cvCreateImage(cvSize(big_frame->width, big_frame->height),big_frame->depth,big_frame->nChannels); 
			cvCopy(big_frame,temPic);
			pPic = cfc.cutRoi(temPic,cvPoint((2*temPic->width)/3,lefttop.y),temPic->width/3,temPic->height*0.4);

		//	CopencvtestDlg cpd;
		//	cpd.searchDirectry();

			CreateDirectory("../Picture/", NULL);

			sprintf(img_name,"../Picture/%d.jpg",car_num); 
			cvSaveImage(img_name,pPic);


			//if (ONE_FLOW==TRUE)
			//{
				//Cpp.plateSegmentOne();
				cvNamedWindow("�Ҳ೵����ͼ",CV_WINDOW_AUTOSIZE);     
				cvShowImage("�Ҳ೵����ͼ", pPic);   
				cvReleaseImage(&temPic); 

		}
	}
	s8=s6;
	s6=s7;
}
void carDoor4(){
	CarFlowCount cfc;
	lefttop.x = 0;
	lefttop.y = (int)(pFrImg->height*height_scale);
	int thickness=(int)(pFrImg->height*0.02 );//ϸ����ϸ
	int sum = 0;//�������غ�
	int sum2=0;
	int sum3=0;
	int sum4=0;
	int count = 0;//�������ظ���
	int count2=0;
	int count3=0;
	int count4=0;
	int avg = 0;//��������ƽ��ֵ
	int avg2=0;
	int avg3=0;
	int avg4=0;
	uchar tmp=0;
	uchar tmp2 = 0;
	uchar tmp3 = 0;
	uchar tmp4 = 0;
	IplImage* img=cvCreateImage(cvSize(frame->width,frame->height),IPL_DEPTH_8U,1);
	uchar* data=(uchar *)pFrImg->imageData;
	int step = pFrImg->widthStep/sizeof(uchar);

	for(int i=lefttop.y;i<(lefttop.y+thickness);i++)
	{
		for(int j=0;j<frame->width/4;j++)
		{ 
			tmp=data[i*step+j];
			sum = sum + tmp;
			count++;
		}
	}
	if(count!=0){	
		avg = sum /count;
	}
	s2 = avg;
	if(s2>diff){//�����г�����
		if(s2-s1>diff&&s1<20&&s2>diff&&s0<noCar){
			strTmp2.Format(_T("������Ŀ:  %d"),++car_num);
			IplImage* temPic;
			temPic = cvCreateImage(cvSize(big_frame->width, big_frame->height),big_frame->depth,big_frame->nChannels); 
			cvCopy(big_frame,temPic);
			pPic = cfc.cutRoi(temPic,cvPoint(0,lefttop.y),temPic->width/4,temPic->height*0.4);

			CreateDirectory("../Picture/", NULL);

			sprintf(img_name,"../Picture/%d.jpg",car_num); 
			cvSaveImage(img_name,pPic);
			//Cpp.plateSegmentOne();
			//if (ONE_FLOW==TRUE)
		//	{
				//Cpp.plateSegmentOne();
				cvNamedWindow("��೵����ͼ",CV_WINDOW_AUTOSIZE);     
				cvShowImage("��೵����ͼ", pPic);   
				cvReleaseImage(&temPic); 
			//} 
		}
	}
	s0=s1;
	s1=s2;




	for(int i=lefttop.y;i<lefttop.y+thickness;i++)
	{
		for(int j=frame->width/4;j<(2*frame->width)/4;j++)
		{
			tmp2=data[i*step+j];
			sum2 = sum2 + tmp2;
			count2++;
		}
	}
	if(count2!=0){avg2 = sum2/count2;}
	s4 = avg2;
	if(s4>diff){//�����г�����
		if(s4-s3>diff&&s3<20&&s4>diff&&s5<noCar){
			strTmp2.Format(_T("������Ŀ:  %d"),++car_num);
			IplImage* temPic;
			temPic = cvCreateImage(cvSize(big_frame->width, big_frame->height),big_frame->depth,big_frame->nChannels); 
			cvCopy(big_frame,temPic);
			pPic = cfc.cutRoi(temPic,cvPoint(temPic->width/4,lefttop.y),(temPic->width)/4,temPic->height*0.4);


			CreateDirectory("../Picture/", NULL);

			sprintf(img_name,"../Picture/%d.jpg",car_num); 
			cvSaveImage(img_name,pPic);


			//if (ONE_FLOW==TRUE)
			//{
				//Cpp.plateSegmentOne();
				cvNamedWindow("���г�����ͼ",CV_WINDOW_AUTOSIZE);     
				cvShowImage("���г�����ͼ", pPic);   
				cvReleaseImage(&temPic); 
			//} 

		}
	}
	s5=s3;
	s3=s4;



	for(int i=lefttop.y;i<lefttop.y+thickness;i++)
	{
		for(int j=(2*frame->width)/4;j<(3*frame->width)/4;j++)
		{
			tmp3=data[i*step+j];
			sum3 = sum3 + tmp3;
			count3++;
		}
	}
	if(count3!=0){avg3 = sum3/count3;}
	s7 = avg3;
	if(s7>diff){//�����г�����
		if(s7-s6>diff&&s6<20&&s7>diff&&s8<noCar){
			strTmp2.Format(_T("������Ŀ:  %d"),++car_num);
			IplImage* temPic;
			temPic = cvCreateImage(cvSize(big_frame->width, big_frame->height),big_frame->depth,big_frame->nChannels); 
			cvCopy(big_frame,temPic);
			pPic = cfc.cutRoi(temPic,cvPoint((2*temPic->width)/4,lefttop.y),temPic->width/4,temPic->height*0.4);


			CreateDirectory("../Picture/", NULL);

			sprintf(img_name,"../Picture/%d.jpg",car_num); 
			cvSaveImage(img_name,pPic);

		//	if (ONE_FLOW==TRUE)
			//{
				//Cpp.plateSegmentOne();
				cvNamedWindow("���г�����ͼ",CV_WINDOW_AUTOSIZE);     
				cvShowImage("���г�����ͼ", pPic);   
				cvReleaseImage(&temPic); 
			//} 
		}
	}
	s8=s6;
	s6=s7;



	for(int i=lefttop.y;i<lefttop.y+thickness;i++)
	{
		for(int j=(2*frame->width)/4;j<(3*frame->width)/4;j++)
		{
			tmp4=data[i*step+j];
			sum4 = sum4 + tmp4;
			count4++;
		}
	}
	if(count4!=0){avg4 = sum4/count4;}
	s10 = avg4;
	if(s10>diff){//�����г�����
		if(s10-s9>diff&&s9<20&&s10>diff&&s11<noCar){
			strTmp2.Format(_T("������Ŀ:  %d"),++car_num);
			IplImage* temPic;
			temPic = cvCreateImage(cvSize(big_frame->width, big_frame->height),big_frame->depth,big_frame->nChannels); 
			cvCopy(big_frame,temPic);
			pPic = cfc.cutRoi(temPic,cvPoint((3*temPic->width)/4,lefttop.y),temPic->width/4,temPic->height*0.4);

			CreateDirectory("../Picture/", NULL);

			sprintf(img_name,"../Picture/%d.jpg",car_num); 
			cvSaveImage(img_name,pPic);


			//if (ONE_FLOW==TRUE)
		//	{
				//Cpp.plateSegmentOne();
				cvNamedWindow("�Ҳ೵����ͼ",CV_WINDOW_AUTOSIZE);     
				cvShowImage("�Ҳ೵����ͼ", pPic);   
				cvReleaseImage(&temPic); 
		//	} 
		}
	}
	s11=s9;
	s9=s10;
}
int CarFlowCount::countCar()
{
	//const char* path = (LPSTR)(LPCTSTR)m_path;
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//capture = cvCreateFileCapture (path);  //��ȡ��Ƶ
	if(pCapture==NULL) {
		//printf("NO capture"); 
		AfxMessageBox("NO capture");//��ȡ���ɹ������ʶ
	};    
	double fps=cvGetCaptureProperty(pCapture, CV_CAP_PROP_FPS );   //cvGetCaptureProperty����ȡ��Ƶ�ļ���һЩ����  ���еڶ��������û�ȷ�����Ե�����
	vfps = 1000 / fps;    
	return vfps;//����ÿ֡���ŵ�ʱ��  1000ms = 1s                           	
}
// ��ĳ���ؼ�����ʾͼ��
void CarFlowCount::ShowImage(IplImage *img, UINT ID)
{
	CDC *pDC=GetDlgItem(ID)->GetDC();
	HDC hDC=pDC->GetSafeHdc();
	CRect rect;
	GetDlgItem(ID)->GetClientRect(&rect);
	SetRect(rect,rect.left,rect.top,rect.right,rect.bottom);
	CvvImage cimg;
	cimg.CopyOf(img);
	cimg.DrawToHDC(hDC,&rect);
	ReleaseDC(pDC);
}
//��ͼ����ȡָ������λ�õľ���ͼƬ
IplImage* CarFlowCount::cutRoi(IplImage* img, CvPoint pt1, int width,int height)
{
	//����һ����cvSize����������С��ͬ�ľ���
	IplImage* src_photo_copy = cvCreateImage(cvSize(width,height),img->depth,img->nChannels);
	//����������һ������	
	//cvRectangle(img,pt1,cvPoint(pt1.x+width,pt1.y+height),Scalar(0,0,255),1,8,0);
	CvRect roi = cvRect(pt1.x, pt1.y, width, height); //�������꣬���γ�����
	cvSetImageROI(img,roi);//��src�н�ȡָ�����δ�С��ͼ��
	cvCopyImage(img,src_photo_copy);
	return src_photo_copy;
}
IplImage* CarFlowCount::cvCopyIplImage(IplImage*  srcImg){

	IplImage* dstImg = cvCreateImage(cvGetSize(srcImg),srcImg->depth,1);
    cvCopy(srcImg,dstImg);
	return dstImg;
}
//��ͨ���� �������м�⵽����ͨ��ṹvector
vector<Hei_sta> CarFlowCount::find_connected_domain(IplImage* src_image, IplImage* dst_image)  //��ͨ��
{
	int row = src_image->height;
	int col = src_image->width;
	int nConnectNum=0;//��ͨ����Ŀ
	if (src_image->nChannels!=1)
	{
		//MessageBox(_T("���Ƕ�ֵͼ��"));
	}

	//��ס���ܱ߽�
	for (int i=0;i<col;i++)
	{
		((uchar*)(src_image->imageData+0*src_image->widthStep))[i]=0;  //�ϱ߽縳ֵ0
		((uchar*)(src_image->imageData+(row-1)*src_image->widthStep))[i]=0;   //�±߽縳ֵ0
	}
	for (int i=0;i<row;i++)
	{
		((uchar*)(src_image->imageData+i*src_image->widthStep))[0]=0;   //��߽縳ֵ0
		((uchar*)(src_image->imageData+i*src_image->widthStep))[col-1]=0;  //�ұ߽縳ֵ0
	}

    //����ͼ��Ľ���
	for (int i = 0 ;i<row;i++)
	{
		for (int j = 0 ;j<col;j++)
		{
			((double*)(dst_image->imageData+i*dst_image->widthStep))[j]=((uchar*)(src_image->imageData+i*src_image->widthStep))[j];
			//��src_image����ÿ������һһ��ֵ�ķ�ʽ���Ƶ�dst_image��ȥ
		}
	}

	//��ͨ���⣬ÿ������һ���
	int sign_con = 300;//�˴��������⸳ֵ�������ǵ���һͼ������ͨ�����ǡ���б��Ϊ255�ĵ㣬������ֵ����255
	//sign_con ��⵽����ͨ�򱻸�ֵ��������
	bool con_suc = false;
	for (int i=1;i<row-1;i++)//ȥ���������һ���ߺ��������һ����
	{
		for (int j=1;j<col-1;j++)//�������������һ���ߺ��������һ����
		{
			if (((double*)(dst_image->imageData+i*dst_image->widthStep))[j]==255)
			{
				((double*)(dst_image->imageData+i*dst_image->widthStep))[j]=sign_con;
				con_suc = true;//con_suc �Ƿ�����ͨ��ı�־��true ��ʾ��ǰ����������ͨ��
				while(con_suc)
				{
					con_suc = false;
					for (int i_s=1;i_s<row-1;i_s++)
					{
						for (int j_s=1;j_s<col-1;j_s++)
						{
							if (((double*)(dst_image->imageData+i_s*dst_image->widthStep))[j_s]==255)
							{
								for (int m=-1;m<=1;m++)//��������Ϊ255�����������������ֵ�Ƿ�Ϊ0����255
								{

									bool flag_bre = false;
									for (int n=-1;n<=1;n++)
									{
										int temp1=((double*)(dst_image->imageData+(i_s+m)*dst_image->widthStep))[j_s + n];
										if ((temp1!=0)&&(temp1!=255))
										{
											((double*)(dst_image->imageData+i_s*dst_image->widthStep))[j_s] = sign_con;
											con_suc = true;
											flag_bre = true;
											break;
										}
									}
									if (flag_bre)
									{
										break;
									}
								}
							}
						}
					}
				}
				sign_con = sign_con + 1;
			}
		}
	}

	//��ͨ������
	Hei_sta st_Connect;//��ͨ��
	vector <Hei_sta>  vt_Connect;//����ָ��
	vt_Connect.clear();
	IplImage* pImageTemp=cvCopyIplImage(dst_image);
	for (int i=0;i<row;i++)
	{
		for (int j=0;j<col;j++)
		{
			int tmp_value = ((double*)(pImageTemp->imageData+i*pImageTemp->widthStep))[j];//ȡ��ͼ���е�ÿ�����ص����
			//if (((double*)(dst_image->imageData+i*dst_image->widthStep))[j]>0)
			if(tmp_value>0)
			{
				int left_col = col-1;
				int right_col = 0;
				int nTop_row=row-1;
				int nBottom_row=0;
				for (int m=0;m<row;m++)
				{
					for (int n=0;n<col;n++)
					{
						if (((double*)(pImageTemp->imageData+m*pImageTemp->widthStep))[n]==tmp_value)
						{
							if (n<left_col)
								left_col = n;
							if (n>right_col)
								right_col = n;
							if(m<nTop_row)
								nTop_row=m;
							if(n>nBottom_row)
								nBottom_row=m;
                            ((double*)(pImageTemp->imageData+m*pImageTemp->widthStep))[n]=0;
						}
					}
				}
				st_Connect.width = right_col - left_col+1;  //������ͨ����
				st_Connect.height=nBottom_row-nTop_row+1;
				st_Connect.top=nTop_row;
				st_Connect.bottom=nBottom_row;
				st_Connect.left=left_col;
				st_Connect.right=right_col;
				st_Connect.sign=tmp_value;
				vt_Connect.push_back(st_Connect);

			}
		}
	}
	cvReleaseImage(&pImageTemp);
     return vt_Connect;
}
IplImage* CarFlowCount::count_aga()
{
	InitText();	
    big_frame = cvQueryFrame( pCapture );
	
 	if (big_frame==NULL)
 	{
 		flag = 1;
 	}
	else{
	CvSize dst_cvsize;   //Ŀ��ͼ��ߴ�   
	dst_cvsize.width = big_frame->width * scale;  //Ŀ��ͼ��Ŀ�ΪԴͼ����scale��  
	dst_cvsize.height = big_frame->height * scale; //Ŀ��ͼ��ĸ�ΪԴͼ��ߵ�scale��  
	frame = cvCreateImage( dst_cvsize, big_frame->depth,big_frame->nChannels); //����Ŀ��ͼ��  
	cvResize(big_frame, frame, CV_INTER_LINEAR); //����Դͼ��Ŀ��ͼ��  

	nFrmNum++;//ȫ�ֱ���  ֡����
	if(nFrmNum == 1)  
	{  
		pBkImg = cvCreateImage(cvSize(frame->width, frame->height),IPL_DEPTH_8U,1);  
		pFrImg = cvCreateImage(cvSize(frame->width, frame->height), IPL_DEPTH_8U,1);  
		pBkMat = cvCreateMat(frame->height, frame->width, CV_32FC1);  
		pFrMat = cvCreateMat(frame->height, frame->width, CV_32FC1);  
		pFrameMat = cvCreateMat(frame->height, frame->width, CV_32FC1);  //���پ���ռ�
		//ת���ɵ�ͨ��ͼ���ٴ���  
		cvCvtColor(frame, pBkImg, CV_BGR2GRAY); //����ͼ�����ͼ��ת�ɻҶ�ͼ�� 
		cvCvtColor(frame, pFrImg, CV_BGR2GRAY);  
		cvConvert(pFrImg, pFrameMat);  //
		cvConvert(pFrImg, pFrMat);  //ͼ������ת���ɾ������ʹ��
		cvConvert(pFrImg, pBkMat);  
	}  
	else  
	{  
		//ÿһ֡ͼ��frame      ��ת���ɻҶ�ͼ��pFrImg
		cvCvtColor(frame, pFrImg, CV_BGR2GRAY);  
		//#define cvConvert( src, dst )  cvConvertScale( (src), (dst), 1, 0 )
		cvConvert(pFrImg, pFrameMat); //ͼ������ת���ɾ������ʹ�� ****��Ƶ����ʱ���ڴ˾�����쳣
		//������˹�˲�����ƽ��ͼ��  
		cvSmooth(pFrameMat, pFrameMat, CV_GAUSSIAN, 3, 0, 0);  
		//��ǰ֡������ͼ���  opencvtest.rc
		cvAbsDiff(pFrameMat, pBkMat, pFrMat);  
		//��ֵ��ǰ��ͼ 
		cvThreshold(pFrMat, pFrImg, threshold_num, 255.0,CV_THRESH_BINARY);  //28���ֵ���Ե��ڶ�ֵ�����ͼ������
		//���±���  
		cvRunningAvg(pFrameMat, pBkMat, 0.1, 0);  
		//������ת��Ϊͼ���ʽ��������ʾ  
		cvConvert(pBkMat, pBkImg);  
		//��ǰ��ͼ�н�ȡ���ߵ����򣬱�������ͼ���У�ͳ����ͨ��ĸ���


		switch(cardoor){
		case 1:
			{
				carDoor1();
			}
			break;
		case  2:
			{
				carDoor2();

			}

			break;
		case 3:
			{
				carDoor3();
			}
			break;
		case 4:
			{
				carDoor4();
			}
			break;
			
		}
		return pPic;
	}	
	}
}
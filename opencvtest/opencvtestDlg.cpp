#include "stdafx.h"
#include "opencvtest.h"
#include "opencvtestDlg.h"
#include "afxdialogex.h"
#include "cv.h" //  OpenCV �Ļ�������ͷ�ļ�  
#include"highgui.h" //OpenCV ��ͼ����ʾ����ͷ�ļ�  
#include "CvvImage.h" 
#include "CarFlowCount.h"
#include "CarPlatePosition.h"
#include "SetPreferencesDlg.h"
#include "CarPlateRec2.h"
#include <atltime.h>
#include <opencv2/opencv.hpp>
#include<vector>      //ʹ��Vector����Ӧ������ͷ�ļ�
#include <iostream>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
using namespace std;

//-----------------------------------------����������----------------------------------------------
extern CString strTmp2;
extern int flag;
int nnPosNum;
float height_xian;
int thickness_scale2;
float thickness=0.0;
int car_num_num;

IplImage* srcPic;//����   �������ƶ�λ   �Ĵ���ͼ��
extern IplImage* srcPic_One;//����һ�������ԭʼͼ��
extern IplImage* pPic;
extern IplImage* m_img_VG;
IplImage* one_img;

Mat image;//���ڱ����ļ���
CString pathVideo;
CString pathPic;
CvCapture* pCapture;
int g_m_run;
IplImage* g_pFrame;
int g_m_int;
bool IsPlaying;
bool IsAviFile;
CRect rect;
CDC* pDC;
HDC hDC;
CWnd* pwnd;
IplImage* cutPic =NULL;//��Ƶ��ͼ��1��������ʾ��ͼ 2�����ڳ��ƶ�λ������
CarFlowCount CFC;
CarPlatePosition CPP;
/***********************************����ͳ�Ʋ��ִ���Ĳ���********/
extern IplImage* exPic;//������ͳ�Ʋ��ֹ����Ľ�ͼ���ƻ��ǲ�����
extern string result_RES2;
extern string dir_path;
extern Directory dir;

/*****************c����ʶ�𲿷ִ�����ⲿ����******************************/
extern IplImage* m_img_vp_bluewhite;//���ڶ�λ�Ķ�ֵͼ��
extern IplImage* roiThree;  //ԭͼ�вü��Ĳ�ɫ���ƽ�ͼ
extern IplImage* vp_images_sign_ref; //�ַ��ָ��ĳ���ͼ��

/*******************���ƶ�λ����ʹ���� һ������Ŀ���**************************/
//BOOL ONE_PART;
//BOOL ONE_FLOW;
/***************������������λ�û�õ�ȫ����ֵ**************************/
int detctlinePos;
int thicknesslinePos;

//-------------------------------------------------------------------�����Զ����ɵĴ���---------------------------------
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
public:
	//afx_msg void OnTimer(UINT_PTR nIDEvent);
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}
BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_WM_TIMER()
END_MESSAGE_MAP()
// CopencvtestDlg �Ի���
CopencvtestDlg::CopencvtestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CopencvtestDlg::IDD, pParent)
	, car_num(_T(""))
	, columns(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}
void CopencvtestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ORIGINAL, Picture1);
	DDX_Control(pDX, IDC_LICENSE, frontimg);
	DDV_MaxChars(pDX, car_num, 100);
	DDX_Control(pDX, IDC_LICENSENUM, mMyPicture);
	DDX_Control(pDX, IDC_SLIDER2, m_hor_detectline);
	DDX_Control(pDX, IDC_SLIDER1, m_hor_thicknessline);
	DDX_Control(pDX, IDC_LIST, m_listbox);
}
BEGIN_MESSAGE_MAP(CopencvtestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
//	ON_BN_CLICKED(IDC_OPENVIDEO, &CopencvtestDlg::OnBnClickedOpenvideo)
	ON_BN_CLICKED(IDC_STARTVIDEO, &CopencvtestDlg::OnBnClickedStartvideo)
	ON_BN_CLICKED(IDC_PAUSE, &CopencvtestDlg::OnBnClickedPause)
	ON_BN_CLICKED(IDC_STOP, &CopencvtestDlg::OnBnClickedStop)
//	ON_BN_CLICKED(IDC_EXIT, &CopencvtestDlg::OnBnClickedExit)
	ON_BN_CLICKED(IDC_FSTART, &CopencvtestDlg::OnBnClickedFstart)
	ON_COMMAND(ID_32771, OnC1)         // ��һ������Ϊ�˵���ID,�ڶ�������Ϊ�¼����ƣ���������ΪOnC1;
	ON_COMMAND(ID_32781, OnC2) 
	ON_COMMAND(ID_32772, OnOpenVideo) 
	ON_COMMAND(ID_32782, OnOpenPic) 
	ON_COMMAND(ID_32779, OnHelpDlg) 
	ON_COMMAND(ID_32780, OnAboutDlg) 

//	ON_WM_TIMER()
ON_WM_TIMER()
ON_BN_CLICKED(IDC_FSTOP, &CopencvtestDlg::OnBnClickedFstop)
ON_LBN_SELCHANGE(IDC_LIST, &CopencvtestDlg::OnLbnSelchangeList)
ON_BN_CLICKED(IDC_CUTSCREEN, &CopencvtestDlg::OnBnClickedCutscreen)
ON_BN_CLICKED(IDC_LSTART, &CopencvtestDlg::OnBnClickedLstart)
ON_STN_CLICKED(IDC_CARNUM, &CopencvtestDlg::OnStnClickedCarnum)
ON_WM_HSCROLL()
ON_BN_CLICKED(IDC_LRECNOGIZE, &CopencvtestDlg::OnBnClickedLrecnogize)
ON_EN_CHANGE(IDC_EDIT1, &CopencvtestDlg::OnEnChangeEdit1)
END_MESSAGE_MAP()
// CopencvtestDlg ��Ϣ�������
BOOL CopencvtestDlg::OnInitDialog()
{
	HMENU m_hMenu1;
	CDialogEx::OnInitDialog();
	//�����ʾ
	ShowWindow(SW_SHOWMAXIMIZED);
	//��Ӳ˵���
	//m_hMenu1 =LoadMenu(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MENU1));//
	// ::SetMenu(this->GetSafeHwnd(),m_hMenu1);//��ӵ��Ի���
	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��
	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	//GetDlgItem( IDC_OPENVIDEO )->EnableWindow( TRUE );
	GetDlgItem( IDC_STARTVIDEO )->EnableWindow( FALSE );
	GetDlgItem( IDC_PAUSE )->EnableWindow( FALSE );
	GetDlgItem( IDC_STOP )->EnableWindow( FALSE );
	GetDlgItem( IDC_FSTART )->EnableWindow( TRUE );//һ����ʼ
	GetDlgItem( IDC_LSTART )->EnableWindow( FALSE );//����ͳ��
	GetDlgItem( IDC_LRECNOGIZE )->EnableWindow( FALSE );//����ʶ��
	
	
	pwnd = GetDlgItem(IDC_VIDEO);
	pwnd->MoveWindow(30,30,624,436);
	pDC =pwnd->GetDC();
	hDC= pDC->GetSafeHdc();
	pwnd->GetClientRect(&rect);
	Invalidate();
//	m_slider.SetRange(0,100);
//	m_slider.SetTicFreq(10);
	CvSize ImgSize;
	ImgSize.height = 436;
	ImgSize.width = 624;
	g_pFrame = cvCreateImage( ImgSize, IPL_DEPTH_8U, 3 );
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE


	/***********************�Լ����λ�ù��������г�ʼ��**********************************/
	m_hor_detectline.SetRange(1,100);
	//����ˮƽ�������ĳ�ʼλ��Ϊ20
	m_hor_detectline.SetPos(50);

	/***********************�Լ���ߴ�ϸ���������г�ʼ��**********************************/
	m_hor_detectline.SetRange(1,100);
	//����ˮƽ�������ĳ�ʼλ��Ϊ20
	m_hor_detectline.SetPos(5);



}

void CopencvtestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}
// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CopencvtestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}
//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CopencvtestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
void CopencvtestDlg::OnStnClickedShowimage()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
//-------------------------------------------------------�������Զ����ɴ���-------------------------------------
void CopencvtestDlg::cleanArea()
{
	CStatic* pStatic = (CStatic*)GetDlgItem(IDC_VIDEO);
	CRect lRect;
	pStatic->GetClientRect(&lRect);
	pStatic->GetDC()->FillSolidRect(lRect.left ,lRect.top ,lRect.Width(),lRect.Height(), RGB(192, 192, 192));

	pStatic = (CStatic*)GetDlgItem(IDC_PLATE_POS);
	pStatic->GetClientRect(&lRect);	
	pStatic->GetDC()->FillSolidRect(lRect.left ,lRect.top ,lRect.Width(),lRect.Height(), RGB(192, 192, 192));

	pStatic = (CStatic*)GetDlgItem(IDC_PLATE_THRES);
	pStatic->GetClientRect(&lRect);	
	pStatic->GetDC()->FillSolidRect(lRect.left ,lRect.top ,lRect.Width(),lRect.Height(), RGB(192, 192, 192));

	pStatic = (CStatic*)GetDlgItem(IDC_PLATE_THRES_RESULT);
	pStatic->GetClientRect(&lRect);	
	pStatic->GetDC()->FillSolidRect(lRect.left ,lRect.top ,lRect.Width(),lRect.Height(), RGB(192, 192, 192));

	pStatic = (CStatic*)GetDlgItem(IDC_PLATE_CHAR_SEG);
	pStatic->GetClientRect(&lRect);	
	pStatic->GetDC()->FillSolidRect(lRect.left ,lRect.top ,lRect.Width(),lRect.Height(), RGB(192, 192, 192));


	//����ͼ���ʱ��ͽ�һ�����ַ����ӵ�edit����
	result_RES2=" ";
	LPCTSTR result2= result_RES2.c_str(); 
	SetDlgItemText(IDC_EDIT3,result2);
}
void CopencvtestDlg::ShowImage(IplImage* img, UINT ID)
{
	CDC* pDC = GetDlgItem( ID ) ->GetDC();		
	HDC hDC = pDC ->GetSafeHdc();	
	CRect rect;                                
	GetDlgItem(ID) ->GetClientRect( &rect );   
	int rw = rect.right - rect.left;
	int rh = rect.bottom - rect.top;
	int iw = img->width;
	int ih = img->height;
	int tx = (int)(rw - iw)/2;	
	int ty = (int)(rh - ih)/2;
	SetRect( rect, tx, ty, tx+iw, ty+ih );
	CvvImage cimg;
	cimg.CopyOf( img );				
	cimg.DrawToHDC( hDC, &rect );
	ReleaseDC( pDC );
}

void CopencvtestDlg::DrawPicToHDC(IplImage *img, UINT ID)
{
	CDC *pDC = GetDlgItem(ID)->GetDC();
	HDC hDC = pDC->GetSafeHdc();
	CRect rect;
	GetDlgItem(ID)->GetClientRect(&rect);
	CvvImage cimg;
	cimg.CopyOf(img,3);
	cimg.DrawToHDC(hDC,&rect);
	ReleaseDC(pDC);
}


void CopencvtestDlg::ResizeImage(IplImage* img)
{
	int w = img->width;
	int h = img->height;
	int max = (w > h)? w: h;
	float scale = (float) ( (float) max / 624.0f );
	int nw = (int)( w/scale );
	int nh = (int)( h/scale );
	//int tlx = (nw > nh)? 0: (int)(320-nw)/2;
	//int tly = (nw > nh)? (int)(320-nh)/2: 0;
	int tlx = 0;
	int tly = 0;
	cvSetImageROI( g_pFrame, cvRect( tlx, tly, nw, nh) );
	cvResize( img, g_pFrame );//����һ���ĳߴ����¹���������  //����һ��ʱ�����������
	cvResetImageROI( g_pFrame );
}

//�����Ƶ�ļ�
void CopencvtestDlg::OnaddVideo()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	strTmp2=" ";
	SetDlgItemText(IDC_EDIT1,strTmp2);

	CFileDialog dlg(
		TRUE,
		_T("*.avi"), 
		NULL,
		OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,
		_T("*.avi|*.avi|*.rmvb|*.rmvb|*.mp4|*.mp4|*.wmv|*.wmv| All Files (*.*) |*.*||"), 
		NULL
		);	
	dlg.m_ofn.lpstrTitle = _T("ѡȡ��Ƶ�ļ�");	
	if( dlg.DoModal() != IDOK )		
		return;

	pathVideo = dlg.GetPathName();
	cleanArea();

	GetDlgItem( IDC_STARTVIDEO )->EnableWindow( TRUE );
	GetDlgItem( IDC_PAUSE )->EnableWindow( TRUE );
	GetDlgItem( IDC_STOP )->EnableWindow( TRUE );
	GetDlgItem( IDC_LRECNOGIZE )->EnableWindow( FALSE );
	//GetDlgItem( IDC_SLIDER1 )->EnableWindow( TRUE );
}

//���ͼ���ļ�
void CopencvtestDlg::OnaddPic()
{
	CFileDialog dlg(TRUE, _T("*.bmp"),"",OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_HIDEREADONLY,"image files (*.bmp; *.jpg) |*.bmp;*.jpg|All Files (*.*)|*.*||",NULL);
	dlg.m_ofn.lpstrTitle = _T("ѡȡͼ���ļ�");	
	if( dlg.DoModal() != IDOK )		
		return;

	pathPic = dlg.GetPathName();

	cleanArea();
	//srcPic=cvLoadImage("D:\18.bmp",1);
	srcPic = cvLoadImage((LPSTR)(LPCTSTR)pathPic);
	DrawPicToHDC(srcPic,IDC_VIDEO);



	GetDlgItem( IDC_STARTVIDEO )->EnableWindow( FALSE );
	GetDlgItem( IDC_PAUSE )->EnableWindow( FALSE );
	GetDlgItem( IDC_STOP )->EnableWindow( FALSE );
	//GetDlgItem( IDC_SLIDER1 )->EnableWindow( TRUE );
	GetDlgItem( IDC_FSTART )->EnableWindow( FALSE );
	GetDlgItem( IDC_LSTART )->EnableWindow( FALSE );
	GetDlgItem( IDC_LRECNOGIZE )->EnableWindow( TRUE );
}


void CopencvtestDlg::OnBnClickedStartvideo()
{
	BOOL SWITCH=FALSE;
	nnPosNum=50;
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if( pathVideo == "" ) 
	{
		MessageBox("����ѡ����Ƶ�ļ�!");
		return;
	}
	else
	{
		if(!(pCapture = cvCreateFileCapture( pathVideo )))
		{
			MessageBox("����Ƶ�ļ�ʧ��!");
			return;
		}
	}
	int frames = (int) cvGetCaptureProperty( pCapture, CV_CAP_PROP_FRAME_COUNT );
	//GetDlgItem( IDC_OPENVIDEO )->EnableWindow( FALSE );
	GetDlgItem( IDC_STARTVIDEO )->EnableWindow( FALSE );
	GetDlgItem( IDC_PAUSE )->EnableWindow( TRUE );
	GetDlgItem( IDC_STOP )->EnableWindow( TRUE );
	GetDlgItem( IDC_FSTART )->EnableWindow( FALSE );
	GetDlgItem( IDC_LSTART )->EnableWindow( TRUE );
	
	IplImage *pFrame = NULL;
	cvNamedWindow("video");
	cvResizeWindow("video",1,1);
	HWND hWnd = (HWND) cvGetWindowHandle("video");
	HWND hParent = ::GetParent(hWnd);
	HWND hwnd1=::FindWindow("CVideoPlayerDlg","VideoPlayer");
	::SetParent(hWnd, hwnd1);
	::ShowWindow(hParent, SW_HIDE);
	if( g_pFrame )								
		cvZero( g_pFrame );
	g_m_run = 1;
	if(pCapture)
	{
		cvSetCaptureProperty(pCapture,CV_CAP_PROP_POS_FRAMES,g_m_int);
	}
	while( (g_m_int < frames) & (g_m_run == 1) )
	{ 
		pFrame = cvQueryFrame( pCapture );
		if (pFrame==NULL)
		{
			INT_PTR nRes;
			nRes=MessageBox(_T("��Ƶ��������ɣ�����������"), _T("����ʶ��ϵͳV1.0"), MB_OK | MB_ICONEXCLAMATION );
			if (IDOK == nRes)   
			     // return;   
				// CopencvtestDlg dlg;
				 //dlg.DoModal();
				 //break;
				 KillTimer(1);
				break;

		} 
		else
		{
		
		IplImage* newframe = cvCloneImage(pFrame);  //��Ƶ������������
		ResizeImage( newframe );
		//ShowImage( pFrame , IDC_VIDEO);
		

		height_xian=(nnPosNum/100.0)*g_pFrame->height;
		thickness = (thickness_scale2/400.0)*g_pFrame->height;
		cvLine(g_pFrame,cvPoint(0,height_xian),cvPoint(650,height_xian),cvScalar(255,0,0),thickness); 
		ShowImage( g_pFrame , IDC_VIDEO);

		if (cvWaitKey(20) == 27) 
			break;
		g_m_int++;
		cvReleaseImage( &newframe );

		}
	}
	if(g_m_int==frames)
	{
		g_m_int=0;
		//m_slider.SetPos(g_m_int);
		g_m_run = 0;
		if( g_pFrame )	
		{
			cvZero( g_pFrame  );
			//cvLine(g_pFrame,cvPoint(0,350),cvPoint(650,350),cvScalar(255,0,0),20); 
			ShowImage( g_pFrame , IDC_VIDEO );
		}
	//	GetDlgItem( IDC_OPENVIDEO )->EnableWindow( FALSE );
		GetDlgItem( IDC_STARTVIDEO )->EnableWindow( FALSE );
		GetDlgItem( IDC_PAUSE )->EnableWindow( TRUE );
		GetDlgItem( IDC_STOP )->EnableWindow( TRUE );
	}	
	cvReleaseCapture(&pCapture);
	cvDestroyWindow("video");
}

void CopencvtestDlg::OnBnClickedPause()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	g_m_run=0;

//	GetDlgItem( IDC_OPENVIDEO )->EnableWindow( FALSE );
	GetDlgItem( IDC_STARTVIDEO )->EnableWindow( TRUE );
	GetDlgItem( IDC_PAUSE )->EnableWindow( FALSE );
	GetDlgItem( IDC_STOP )->EnableWindow( FALSE );
	GetDlgItem( IDC_FSTART )->EnableWindow( FALSE );
	GetDlgItem( IDC_LSTART )->EnableWindow( FALSE );
	KillTimer(1);
}

void CopencvtestDlg::OnBnClickedStop()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDC MemDC; 
	CBitmap m_Bitmap1;
	//m_Bitmap1.LoadBitmap(129); 
	MemDC.CreateCompatibleDC(NULL);
	MemDC.SelectObject(&m_Bitmap1);
	pDC->StretchBlt(rect.left,rect.top,rect.Width(),rect.Height(),&MemDC,0,0,48,48,SRCCOPY); 
	m_Bitmap1.DeleteObject();
	MemDC.DeleteDC();

//	GetDlgItem( IDC_OPENVIDEO )->EnableWindow( TRUE );
	GetDlgItem( IDC_STARTVIDEO )->EnableWindow( TRUE );
	GetDlgItem( IDC_PAUSE )->EnableWindow( FALSE );
	GetDlgItem( IDC_STOP )->EnableWindow( FALSE );
	GetDlgItem( IDC_FSTART )->EnableWindow( FALSE );
	GetDlgItem( IDC_LSTART )->EnableWindow( FALSE );

	g_m_run=0;
	g_m_int=0;
	KillTimer(1);
	return;
}


void CopencvtestDlg::OnBnClickedFstart()
{
	//ONE_FLOW=TRUE;
	//ONE_PART=FALSE;//����һ������Ŀ���

	//int vfps = CFC.countCar();
	//AfxMessageBox("����");

	//SetTimer(1,vfps, NULL);  //��ʱ��ID����ʱʱ�䣬������Ϣ������Ӧ��ʱ����
	searchDirectry();
//	AfxMessageBox("����");
}

void CopencvtestDlg::cutToOrignal()
{
	//AfxMessageBox("����");
	DrawPicToHDC(exPic,IDC_LICENSE);
}

void CopencvtestDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//CarFlowCount cfc;
    cutPic = CFC.count_aga();
	DrawPicToHDC(pPic,IDC_PLATE_POS);
	cutToOrignal();
	//CWnd *pWnd2=GetDlgItem(IDC_CARNUM);//��öԻ�����
	//pWnd2->SetWindowText(CFC.strTmp2);
	SetDlgItemText(IDC_EDIT1,strTmp2);
	if(flag==1)
	{
		KillTimer(1);
	}
	

	CDialogEx::OnTimer(nIDEvent);
}


void CopencvtestDlg::searchDirectry()
{
	//UpdateData(TRUE);
// 	char szDir[150];//����һ���ַ������������ļ���
// 	CString strFilePath1;
// 	BROWSEINFO bi;//����һ��BROWSEIFO����
// 	ITEMIDLIST* pidl;//����һ���ṹָ��
// 	memset(&bi,0,sizeof BROWSEINFO);
// 	bi.hwndOwner=m_hWnd;
// 	bi.hwndOwner=this->m_hWnd;//ָ�������ڣ��ڶԻ�����ʾ�ڼ䣬�����ڽ�������
// 	bi.pszDisplayName=szDir;//pszDisplayNmae�����������û�ѡ�е�Ŀ¼�ַ������ڴ��ַ����������
// 	bi.pidlRoot=NULL;//���ָ��ΪNULL�����ԡ����桱Ϊ����pidRoot��ITEMIDLIST�ṹ�ĵ�ַ�������ʱ�ĳ�ʼ��Ŀ¼
// 	bi.lpszTitle="��ѡ����������·��";//��һ�н���ʾ�ڶԻ���Ķ���
// 
// 	//�������ڵ�ѡ�����״̬�ı�����ʾĿ¼���ļ���ֻ�����ļ�ϵͳĿ¼������ϵͳһ���ڵ�
// 	bi.ulFlags=BIF_STATUSTEXT|BIF_BROWSEINCLUDEFILES|BIF_RETURNONLYFSDIRS|BIF_RETURNFSANCESTORS;
// 
// 	bi.lpfn=NULL;//lpfn��Ӧ�ó������������Ի���ص������ĵ�ַ�����Ի����е��¼�����ʱ�������ûص��������ò�������ΪNULL��
// 
// 	bi.iImage=0;//�Ի��򴫵ݸ��ص�������һ������ָ��
// 	bi.lParam=0;//��ѡ��Ŀ¼��ص�ͼ�񡣸�ͼ�񽫱�ָ��Ϊϵͳͼ���б��е�����ֵ
// 	pidl=SHBrowseForFolder(&bi);
// 
// 
//  	if (SHGetPathFromIDList(pidl,szDir))
//  	{
//  		strFilePath1=szDir;
// // 		SetDlgItemText(IDC_EDIT_PLATE, strFilePath1);
//  		//return;
// 		 } 
	//UpdateData(FALSE);


	string dir_path = "C:\\Picture\\";
	//LPCTSTR strFilePath= atoi(strFilePath1); 
	//string strFilePath  = LPCSTR(strFilePath1);
	vector<string> filename = dir.GetListFiles(dir_path,"*.jpg",false);

	for(int i = 0;i<filename.size();i++)
	{
		string fileName = filename[i];
		string fileFullName = dir_path+fileName;
		image = imread(fileFullName,1);

		srcPic = &IplImage(image);


		CPP.plateSegment();

	}

	waitKey(1000);
}

void CopencvtestDlg::OnBnClickedFstop()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	KillTimer(1);
}


void CopencvtestDlg::OnLbnSelchangeList()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CopencvtestDlg::OnBnClickedCutscreen()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CopencvtestDlg::OnBnClickedLstart()
{
	//ONE_FLOW = FALSE;
	int vfps = CFC.countCar();
	//AfxMessageBox("����");
	
	SetTimer(1,vfps, NULL);  //��ʱ��ID����ʱʱ�䣬������Ϣ������Ӧ��ʱ����
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
void CopencvtestDlg::OnC1()
{
	OnaddVideo();
}
void CopencvtestDlg::OnC2()
{
	SetPreferencesDlg spd;
	spd.DoModal();
}
void CopencvtestDlg::OnOpenVideo()
{
	KillTimer(1);
	exit(0);
	
}

void CopencvtestDlg::OnHelpDlg()
{
   ShellExecute(NULL,"open","d:\\������ͳ�Ƽ�����ʶ��ϵͳʹ��˵����.htm",NULL,NULL,SW_SHOWNORMAL);

}

void CopencvtestDlg::OnAboutDlg()
{
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();

}

void CopencvtestDlg::OnOpenPic()
{
	//PostMessage(WM_QUIT,0,0);
	OnaddPic();
}

void CopencvtestDlg::OnStnClickedCarnum()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CopencvtestDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	switch(pScrollBar->GetDlgCtrlID()){

	case IDC_SLIDER2:
		{
			int pos = m_hor_detectline.GetPos();
			switch(nSBCode)
			{
				//����������һ�У���pos��1
			case  SB_LINEUP:
				pos-=1;
				break;
				//������ҹ���һ�У���pos��1
			case SB_LINEDOWN:
				pos+=1;
				break;
				//����������һҳ����pos��10
			case SB_PAGEUP:
				pos-=10;
				break;
				//������ҹ���һҳ����pos��10
			case SB_PAGEDOWN:
				pos+=10;
				break;
				//�������������ˣ���posΪ1
			case SB_TOP:
				pos = 1;
				break;
				//������������Ҷˣ���posΪ100
			case SB_BOTTOM:
				pos = 100;
				break;
				//����϶������������ָ��λ�ã���pos��ֵΪnpos��ֵ
			case SB_THUMBPOSITION:
				pos = nPos;

				nnPosNum=nPos;
				break;
				//�����m_horiScrollbar.SetScrollPos(pos);ִ��ʱ��ڶ��ν���˺���������ȷ��������λ�ã����һ�ֱ�ӵ�default��֧��
				//���Դ˴����ñ༭������ʾ��ֵ
			default:
				//SetDlgItemInt(IDC_HSCROLL_EDIT,pos);
				return;
			}
			//���ù�������λ��
			//m_horiScrollbar.SetScrollPos(pos);
			m_hor_detectline.SetPos(pos);
			detctlinePos = m_hor_detectline.GetPos();
			break;
		}
	case IDC_SLIDER1:
		/***************************���ű���ˮƽ����������*******************************************/
		{
			//��ȡˮƽ��������ǰλ��
			//int pos = m_horiScrollbar.GetScrollPos();
			int pos_thickness = m_hor_thicknessline.GetPos();
			switch(nSBCode)
			{
				//����������һ�У���pos��1
			case  SB_LINEUP:
				pos_thickness-=1;
				break;
				//������ҹ���һ�У���pos��1
			case SB_LINEDOWN:
				pos_thickness+=1;
				break;
				//����������һҳ����pos��10
			case SB_PAGEUP:
				pos_thickness-=10;
				break;
				//������ҹ���һҳ����pos��10
			case SB_PAGEDOWN:
				pos_thickness+=10;
				break;
				//�������������ˣ���posΪ1
			case SB_TOP:
				pos_thickness = 1;
				break;
				//������������Ҷˣ���posΪ100
			case SB_BOTTOM:
				pos_thickness = 100;
				break;
				//����϶������������ָ��λ�ã���pos��ֵΪnpos��ֵ
			case SB_THUMBPOSITION:
				pos_thickness = nPos;

				thickness_scale2=nPos;
				break;
				//�����m_horiScrollbar.SetScrollPos(pos);ִ��ʱ��ڶ��ν���˺���������ȷ��������λ�ã����һ�ֱ�ӵ�default��֧��
				//���Դ˴����ñ༭������ʾ��ֵ
			default:
				//SetDlgItemInt(IDC_HSCROLL_EDIT4,pos_zoom);
				return;
			}
			//���ù�������λ��
			//m_horiScrollbar.SetScrollPos(pos);
			m_hor_thicknessline.SetPos(pos_thickness);
			thicknesslinePos = m_hor_thicknessline.GetPos();
			break;
		}
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}
}

void CopencvtestDlg::OnBnClickedLrecnogize()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

		//����ΪTRUE���зֱ���
		//ONE_PART=TRUE;
		CPP.plateSegment();
		segmentRecognition();

		LPCTSTR result2= result_RES2.c_str(); 
		SetDlgItemText(IDC_EDIT3,result2);

		

		CTime curTime;
		curTime = CTime::GetCurrentTime();
		CString strCurTime,showContent;   
		char carNum[40];
		CString spaces="     ";
		strCurTime.Format(_T("%04d/%02d/%02d %02d:%02d:%02d"), curTime.GetYear(), curTime.GetMonth(), curTime.GetDay(), curTime.GetHour(), curTime.GetMinute(), curTime.GetSecond());
		car_num_num++;
		sprintf(carNum,"%d",car_num_num); 
		showContent = carNum+spaces+strCurTime+spaces+result2;
		m_listbox.InsertString(0,showContent);

		result_RES2="";
		DrawPicToHDC(srcPic,IDC_PLATE_POS);
		DrawPicToHDC(m_img_vp_bluewhite,IDC_PLATE_THRES);
		DrawPicToHDC(roiThree,IDC_PLATE_THRES_RESULT);
		DrawPicToHDC(vp_images_sign_ref,IDC_PLATE_CHAR_SEG);

		GetDlgItem( IDC_LRECNOGIZE )->EnableWindow( FALSE );
		//searchDirectry();
	
}
//��Ȼ�����ˣ����Ǵ������⣬��δʹ�ã���һ�ֽ�ͼֱ��ʹ�õķ�ʽ����ĳ��ƴ�������������
void CopencvtestDlg::plateSegment_ALL()
{
	//����ΪTRUE���зֱ���
	//ONE_PART=TRUE;
	CPP.plateSegmentOne();
	segmentRecognition();

	LPCTSTR result2= result_RES2.c_str(); 
	//LPCTSTR result2=(LPCTSTR)result.c_str();
	SetDlgItemText(IDC_EDIT3,result2);
	DrawPicToHDC(srcPic,IDC_PLATE_POS);
	DrawPicToHDC(m_img_vp_bluewhite,IDC_PLATE_THRES);
	DrawPicToHDC(roiThree,IDC_PLATE_THRES_RESULT);
	DrawPicToHDC(vp_images_sign_ref,IDC_PLATE_CHAR_SEG);	
}

void CopencvtestDlg::OnEnChangeEdit1()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}

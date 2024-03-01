
// opencvtestDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "cv.h" //  OpenCV �Ļ�������ͷ�ļ�  
#include"highgui.h" //OpenCV ��ͼ����ʾ����ͷ�ļ�  
#include "mypicture.h"

// CopencvtestDlg �Ի���
class CopencvtestDlg : public CDialogEx
{
// ����
public:
	CopencvtestDlg(CWnd* pParent = NULL);	// ��׼���캯��
	void cleanArea();
	void OnaddVideo();
	void OnaddPic();
	void ResizeImage(IplImage* img);
	void ShowImage(IplImage* img, UINT ID);
	void cutToOrignal();
	void DrawPicToHDC(IplImage *img, UINT ID);
	void plateSegment_ALL();
	void searchDirectry();

// �Ի�������
	enum { IDD = IDD_OPENCVTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListBox m_listbox;
	CStatic Picture1;
	CSliderCtrl m_hor_detectline;
	CSliderCtrl m_hor_thicknessline;


	afx_msg void OnBnClickedOk();
	afx_msg void OnStnClickedShowimage();
	afx_msg void OnBnClickedButton1();
	// ��ĳ���ؼ�����ʾͼ��
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	//�˵���Ӧ�¼�1
	 afx_msg void OnC1();  
	 afx_msg void OnC2();  
	 afx_msg void OnOpenVideo();  
	 afx_msg void OnOpenPic(); 
	 afx_msg void OnHelpDlg(); 
	 afx_msg void OnAboutDlg(); 
	
	CStatic frontimg;
	// ��������
	CString car_num;
	// ��ȡͼ��
	void GetImageRect(IplImage* orgImage, IplImage* imgRect, CvRect rectInImage);
	//
	IplImage* CopyIplImageROI(IplImage* pImageSrc,CvRect rectROI);
	int columns;
	afx_msg void OnStnClickedcolumn();
	afx_msg void OnBnClickedChoosefile();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	CMyPicture mMyPicture;
	afx_msg void OnStnClickedMypicture();
//	afx_msg void OnBnClickedOpenvideo();
	afx_msg void OnBnClickedStartvideo();
	afx_msg void OnBnClickedPause();
	afx_msg void OnBnClickedStop();
	afx_msg void OnBnClickedExit();
	afx_msg void OnBnClickedFstart();

//	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedFstop();
	afx_msg void OnLbnSelchangeList();
	// �˵���
	HMENU SetMenu;
	afx_msg void OnBnClickedCutscreen();
	afx_msg void OnBnClickedLstart();
	afx_msg void OnStnClickedCarnum();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedLrecnogize();
	afx_msg void OnEnChangeEdit1();
};

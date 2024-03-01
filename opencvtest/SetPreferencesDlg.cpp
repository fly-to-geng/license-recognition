// SetPreferencesDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "opencvtest.h"
#include "SetPreferencesDlg.h"
#include "afxdialogex.h"
#include "iostream"
#include "sstream"
#include <fstream>
#include <stdlib.h>
#include <math.h>


using namespace std;
// SetPreferencesDlg �Ի���
CString strFilePath1;  //���Ʊ���λ��
CString strFilePath2;  //�������λ��
CString strFilePath3;  //��ͼ����λ��

CString pathVideo2;//��������ļ���·��
/************************DLG�еĲ���д�뵽�ļ��еĲ������壬����ȫ�ֱ���*************************/
//ComBox����
CString laneText;//�����ַ���
CString areaText;//���Ƶ����ַ���
CString searchboxText;//�ֶ�λ�������ַ���

//CStrilBar����
CString sprpiexlText;//���ز�
CString zoomText;//���ű���
CString thresholdText;//��ֵ
CString nocarpiexlText;//�޳�����ֵ
CString platethresholdText;//���ƶ�ֵ����ֵ

//�Զ���·������
CString platepathText;//����·��
CString resultpathText;//���·��
CString cutpicpathText;//��ͼ·��

/************************�ļ��еĲ�����ȡ��DLG�еĲ������壬����ȫ�ֱ���*************************/
//ComBox����
string inlaneText;//�����ַ���
string inareaText;//���Ƶ����ַ���
string insearchboxText;//�ֶ�λ�������ַ���

//CStrilBar����
string insprpiexlText;//���ز�
string inzoomText;//���ű���
string inthresholdText;//��ֵ
string innocarpiexlText;//�޳�����ֵ
string inplatethresholdText;//���ƶ�ֵ����ֵ

//�Զ���·������
string inplatepathText;//����·��
string inresultpathText;//���·��
string incutpicpathText;//��ͼ·��

/***************�ļ��еĲ�����ȡ��DLG�еĲ������壬����ȫ�ֱ���Ϊ���������************************/
//ComBox����
int inlaneInt;//�����ַ���
string inareaString;//���Ƶ����ַ���
string insearchboxString;//�ֶ�λ�������ַ���

//CStrilBar����
int insprpiexlInt;//���ز�
double inzoomFloat;//���ű���
int inthresholdInt;//��ֵ
int innocarpiexlInt;//�޳�����ֵ
int inplatethresholdInt;//���ƶ�ֵ����ֵ

//�Զ���·������
string inplatepathString;//����·��
string inresultpathString;//���·��
string incutpicpathString;//��ͼ·��



IMPLEMENT_DYNAMIC(SetPreferencesDlg, CDialog)

SetPreferencesDlg::SetPreferencesDlg(CWnd* pParent /*=NULL*/)
	: CDialog(SetPreferencesDlg::IDD, pParent)
{

}

SetPreferencesDlg::~SetPreferencesDlg()
{
}

void SetPreferencesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER2, m_horiScrollbar);
	DDX_Control(pDX, IDC_SLIDER4, m_horiScrollbar_zoom);
	DDX_Control(pDX, IDC_SLIDER06, m_horiScrollbar_threshold);
	DDX_Control(pDX, IDC_SLIDER8, m_horiScrollbar_pixel);
	DDX_Control(pDX, IDC_SLIDER7, m_horiScrollbar_platethros);
	DDX_Control(pDX, IDC_COMBO_LANE, m_combo_lane);
	DDX_Control(pDX, IDC_COMBO_AREA, m_combox_area);
	DDX_Control(pDX, IDC_COMBO_SEARCHBOX, m_combox_searchbox);
	DDX_Control(pDX, IDC_HSCROLL_EDIT, m_sprpiexledit);
	DDX_Control(pDX, IDC_HSCROLL_EDIT4, m_zoomedit);
	DDX_Control(pDX, IDC_HSCROLL_EDIT06, m_thresholdedit);
	DDX_Control(pDX, IDC_HSCROLL_EDIT8, m_nocaredit);
	DDX_Control(pDX, IDC_HSCROLL_EDIT7, m_paltethrestholdedit);
	DDX_Control(pDX, IDC_EDIT_PLATE, m_platepathedit);
	DDX_Control(pDX, IDC_EDIT_RESULT, m_resultpathedit);
	DDX_Control(pDX, IDC_EDIT_CUTPIC, m_cutpicedit);
}




BEGIN_MESSAGE_MAP(SetPreferencesDlg, CDialog)
	ON_COMMAND(ID_32781, &SetPreferencesDlg::On32781)
	ON_COMMAND(ID_32771, &SetPreferencesDlg::On32771)
	
	ON_WM_HSCROLL()
	ON_CBN_SELCHANGE(IDC_COMBO_LANE, &SetPreferencesDlg::OnCbnSelchangeComboLane)
	ON_CBN_SELCHANGE(IDC_COMBO_AREA, &SetPreferencesDlg::OnCbnSelchangeComboArea)
	ON_CBN_SELCHANGE(IDC_COMBO_SEARCHBOX, &SetPreferencesDlg::OnCbnSelchangeComboSearchbox)
	ON_BN_CLICKED(IDC_BUTTON2, &SetPreferencesDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &SetPreferencesDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &SetPreferencesDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDOK, &SetPreferencesDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON5, &SetPreferencesDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON1, &SetPreferencesDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// SetPreferencesDlg ��Ϣ�������


void SetPreferencesDlg::On32781()
{
	SetPreferencesDlg spd;
	spd.DoModal();
	// TODO: �ڴ���������������
}


void SetPreferencesDlg::On32771()
{
	exit(0);
	// TODO: �ڴ���������������
}


void SetPreferencesDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}


void SetPreferencesDlg::OnCbnSelchangeCombo4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void SetPreferencesDlg::InitText()
{
	ifstream inf;
	//�ļ��в�������
	if(pathVideo2==""){
		inf.open("./out.txt");
			
	}
	else{
		inf.open("./out.txt");
	//inf.open(pathVideo2);
	}
	inf>>inlaneText>>inareaText>>insearchboxText>>insprpiexlText>>inzoomText>>inthresholdText>>innocarpiexlText>>inplatethresholdText
		>>inplatepathText>>inresultpathText>>incutpicpathText;

	inlaneInt = atoi(inlaneText.c_str());
	inareaString=inareaText;
	insearchboxString=insearchboxText;
	insprpiexlInt=atoi(insprpiexlText.c_str());
	inzoomFloat=atof(inzoomText.c_str());
	inthresholdInt=atoi(inthresholdText.c_str());
	innocarpiexlInt=atoi(innocarpiexlText.c_str());
	inplatethresholdInt=atoi(inplatethresholdText.c_str());
	inplatepathString=inplatethresholdText;
	inresultpathString=inresultpathText;
	incutpicpathString=incutpicpathText;
	////����Ĭ��·��
	//strFilePath1 = "D://word";
	//strFilePath2 = "D://word";
	//strFilePath3 = "D://word";

	// SetDlgItemText(IDC_EDIT_PLATE, strFilePath1);
	// SetDlgItemText(IDC_EDIT_RESULT, strFilePath2);
	//  SetDlgItemText(IDC_EDIT_CUTPIC, strFilePath3);
	
}


BOOL SetPreferencesDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	InitText();

		/**************************���ز�ˮƽ��������ʼ������****************************************/
	//����ˮƽ��������ΧΪ1-100
	m_horiScrollbar.SetRange(1,100);
	//����ˮƽ�������ĳ�ʼλ��Ϊ20
	m_horiScrollbar.SetPos(insprpiexlInt);
	//�ڱ༭������ʾ20
	SetDlgItemInt(IDC_HSCROLL_EDIT,insprpiexlInt);

	/**************************���ű���ˮƽ��������ʼ������****************************************/
	//����ˮƽ��������ΧΪ1-100
	m_horiScrollbar_zoom.SetRange(1,100);
	//����ˮƽ�������ĳ�ʼλ��Ϊ20
	m_horiScrollbar_zoom.SetPos(inzoomFloat);
	//�ڱ༭������ʾ20
	SetDlgItemInt(IDC_HSCROLL_EDIT4,inzoomFloat);

	/**************************��ֵ����ֵ��������ʼ������****************************************/
	//����ˮƽ��������ΧΪ1-100
	m_horiScrollbar_threshold.SetRange(1,100);
	//����ˮƽ�������ĳ�ʼλ��Ϊ20
	m_horiScrollbar_threshold.SetPos(inthresholdInt);
	//�ڱ༭������ʾ20
	SetDlgItemInt(IDC_HSCROLL_EDIT06,inthresholdInt);

	/**************************�޳�����ֵ��������ʼ������****************************************/
	//����ˮƽ��������ΧΪ1-100
	m_horiScrollbar_pixel.SetRange(1,100);
	//����ˮƽ�������ĳ�ʼλ��Ϊ20
	m_horiScrollbar_pixel.SetPos(innocarpiexlInt);
	//�ڱ༭������ʾ20
	SetDlgItemInt(IDC_HSCROLL_EDIT8,innocarpiexlInt);

	/**************************���ƶ�ֵ����ֵ��������ʼ������****************************************/
	//����ˮƽ��������ΧΪ1-100
	m_horiScrollbar_platethros.SetRange(1,100);
	//����ˮƽ�������ĳ�ʼλ��Ϊ20
	m_horiScrollbar_platethros.SetPos(inplatethresholdInt);
	//�ڱ༭������ʾ20
	SetDlgItemInt(IDC_HSCROLL_EDIT7,inplatethresholdInt);

	/******************************����ѡ��COMBOX��ʼ������***************************************/


 	m_combo_lane.InsertString(0,"1");
 	m_combo_lane.InsertString(1,"2");
 	m_combo_lane.InsertString(2,"3");
 	m_combo_lane.InsertString(3,"4");

	LPCSTR inlaneTextLpc = inlaneText.c_str();
	int inlaneIndex = m_combo_lane.FindString(-1,inlaneTextLpc);
	m_combo_lane.SetCurSel(inlaneIndex);
	//m_combo_lane.Text

	/******************************����ѡ��ComBox��ʼ������***************************************/
	m_combox_area.InsertString(0,"��");
	m_combox_area.InsertString(1,"��");
	m_combox_area.InsertString(2,"��");
	m_combox_area.InsertString(3,"��");
	m_combox_area.InsertString(4,"��");
	m_combox_area.InsertString(5,"��");
	m_combox_area.InsertString(6,"��");
	m_combox_area.InsertString(7,"��");

	LPCSTR inareaTextLpc = inareaText.c_str();
	int inareaIndex = m_combox_area.FindString(-1,inareaTextLpc);
	m_combox_area.SetCurSel(inareaIndex);

	/******************************�ֶ�λ�������СCOMBOX��ʼ������***************************************/
	m_combox_searchbox.InsertString(0,"40*10");
	m_combox_searchbox.InsertString(1,"60*15");
	m_combox_searchbox.InsertString(2,"80*20");
	m_combox_searchbox.InsertString(3,"100*25");
	m_combox_searchbox.InsertString(4,"120*30");
	m_combox_searchbox.InsertString(5,"140*35");

	LPCSTR insearchboxTextLpc = insearchboxText.c_str();
	int insearchboxIndex = m_combox_searchbox.FindString(-1,insearchboxTextLpc);
	m_combox_searchbox.SetCurSel(insearchboxIndex);



	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

//ͨ����������Կ�����Ӷ�Ӧ�¼��õ�
void SetPreferencesDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	/***************************���ز�ˮƽ����������*******************************************/
	switch(pScrollBar->GetDlgCtrlID()){
	
	case IDC_SLIDER2:
		{
			int pos = m_horiScrollbar.GetPos();
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
				break;
				//�����m_horiScrollbar.SetScrollPos(pos);ִ��ʱ��ڶ��ν���˺���������ȷ��������λ�ã����һ�ֱ�ӵ�default��֧��
				//���Դ˴����ñ༭������ʾ��ֵ
			default:
				SetDlgItemInt(IDC_HSCROLL_EDIT,pos);
				return;
			}
			//���ù�������λ��
			//m_horiScrollbar.SetScrollPos(pos);
			m_horiScrollbar.SetPos(pos);
			break;
			}
	case IDC_SLIDER4:
		/***************************���ű���ˮƽ����������*******************************************/
		{
			//��ȡˮƽ��������ǰλ��
		//int pos = m_horiScrollbar.GetScrollPos();
		int pos_zoom = m_horiScrollbar_zoom.GetPos();
		switch(nSBCode)
		{
			//����������һ�У���pos��1
		case  SB_LINEUP:
			pos_zoom-=1;
			break;
			//������ҹ���һ�У���pos��1
		case SB_LINEDOWN:
			pos_zoom+=1;
			break;
			//����������һҳ����pos��10
		case SB_PAGEUP:
			pos_zoom-=10;
			break;
			//������ҹ���һҳ����pos��10
		case SB_PAGEDOWN:
			pos_zoom+=10;
			break;
			//�������������ˣ���posΪ1
		case SB_TOP:
			pos_zoom = 1;
			break;
			//������������Ҷˣ���posΪ100
		case SB_BOTTOM:
			pos_zoom = 100;
			break;
			//����϶������������ָ��λ�ã���pos��ֵΪnpos��ֵ
		case SB_THUMBPOSITION:
			pos_zoom = nPos;
			break;
			//�����m_horiScrollbar.SetScrollPos(pos);ִ��ʱ��ڶ��ν���˺���������ȷ��������λ�ã����һ�ֱ�ӵ�default��֧��
			//���Դ˴����ñ༭������ʾ��ֵ
		default:
			SetDlgItemInt(IDC_HSCROLL_EDIT4,pos_zoom);
			return;
		}
		//���ù�������λ��
		//m_horiScrollbar.SetScrollPos(pos);
		m_horiScrollbar_zoom.SetPos(pos_zoom);
		break;
		}
	case IDC_SLIDER06:
		/***************************���ű���ˮƽ����������*******************************************/
		{
			//��ȡˮƽ��������ǰλ��
			//int pos = m_horiScrollbar.GetScrollPos();
			int pos_threshold = m_horiScrollbar_threshold.GetPos();
			switch(nSBCode)
			{
				//����������һ�У���pos��1
			case  SB_LINEUP:
				pos_threshold-=1;
				break;
				//������ҹ���һ�У���pos��1
			case SB_LINEDOWN:
				pos_threshold+=1;
				break;
				//����������һҳ����pos��10
			case SB_PAGEUP:
				pos_threshold-=10;
				break;
				//������ҹ���һҳ����pos��10
			case SB_PAGEDOWN:
				pos_threshold+=10;
				break;
				//�������������ˣ���posΪ1
			case SB_TOP:
				pos_threshold = 1;
				break;
				//������������Ҷˣ���posΪ100
			case SB_BOTTOM:
				pos_threshold = 100;
				break;
				//����϶������������ָ��λ�ã���pos��ֵΪnpos��ֵ
			case SB_THUMBPOSITION:
				pos_threshold = nPos;
				break;
				//�����m_horiScrollbar.SetScrollPos(pos);ִ��ʱ��ڶ��ν���˺���������ȷ��������λ�ã����һ�ֱ�ӵ�default��֧��
				//���Դ˴����ñ༭������ʾ��ֵ
			default:
				SetDlgItemInt(IDC_HSCROLL_EDIT06,pos_threshold);
				return;
			}
			//���ù�������λ��
			//m_horiScrollbar.SetScrollPos(pos);
			m_horiScrollbar_threshold.SetPos(pos_threshold);
			break;
		}
	case IDC_SLIDER8:
		/***************************���ű���ˮƽ����������*******************************************/
		{
			//��ȡˮƽ��������ǰλ��
			//int pos = m_horiScrollbar.GetScrollPos();
			int pos_pixel = m_horiScrollbar_pixel.GetPos();
			switch(nSBCode)
			{
				//����������һ�У���pos��1
			case  SB_LINEUP:
				pos_pixel-=1;
				break;
				//������ҹ���һ�У���pos��1
			case SB_LINEDOWN:
				pos_pixel+=1;
				break;
				//����������һҳ����pos��10
			case SB_PAGEUP:
				pos_pixel-=10;
				break;
				//������ҹ���һҳ����pos��10
			case SB_PAGEDOWN:
				pos_pixel+=10;
				break;
				//�������������ˣ���posΪ1
			case SB_TOP:
				pos_pixel = 1;
				break;
				//������������Ҷˣ���posΪ100
			case SB_BOTTOM:
				pos_pixel = 100;
				break;
				//����϶������������ָ��λ�ã���pos��ֵΪnpos��ֵ
			case SB_THUMBPOSITION:
				pos_pixel = nPos;
				break;
				//�����m_horiScrollbar.SetScrollPos(pos);ִ��ʱ��ڶ��ν���˺���������ȷ��������λ�ã����һ�ֱ�ӵ�default��֧��
				//���Դ˴����ñ༭������ʾ��ֵ
			default:
				SetDlgItemInt(IDC_HSCROLL_EDIT8,pos_pixel);
				return;
			}
			//���ù�������λ��
			//m_horiScrollbar.SetScrollPos(pos);
			m_horiScrollbar_pixel.SetPos(pos_pixel);
			break;
		}

	case IDC_SLIDER7:
		/***************************���ű���ˮƽ����������*******************************************/
		{
			//��ȡˮƽ��������ǰλ��
			//int pos = m_horiScrollbar.GetScrollPos();
			int pos_platethros = m_horiScrollbar_platethros.GetPos();
			switch(nSBCode)
			{
				//����������һ�У���pos��1
			case  SB_LINEUP:
				pos_platethros-=1;
				break;
				//������ҹ���һ�У���pos��1
			case SB_LINEDOWN:
				pos_platethros+=1;
				break;
				//����������һҳ����pos��10
			case SB_PAGEUP:
				pos_platethros-=10;
				break;
				//������ҹ���һҳ����pos��10
			case SB_PAGEDOWN:
				pos_platethros+=10;
				break;
				//�������������ˣ���posΪ1
			case SB_TOP:
				pos_platethros = 1;
				break;
				//������������Ҷˣ���posΪ100
			case SB_BOTTOM:
				pos_platethros = 100;
				break;
				//����϶������������ָ��λ�ã���pos��ֵΪnpos��ֵ
			case SB_THUMBPOSITION:
				pos_platethros = nPos;
				break;
				//�����m_horiScrollbar.SetScrollPos(pos);ִ��ʱ��ڶ��ν���˺���������ȷ��������λ�ã����һ�ֱ�ӵ�default��֧��
				//���Դ˴����ñ༭������ʾ��ֵ
			default:
				SetDlgItemInt(IDC_HSCROLL_EDIT7,pos_platethros);
				return;
			}
			//���ù�������λ��
			//m_horiScrollbar.SetScrollPos(pos);
			m_horiScrollbar_platethros.SetPos(pos_platethros);
			break;
		}


	}
	//��ȡˮƽ��������ǰλ��
	//int pos = m_horiScrollbar.GetScrollPos();
	
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}


//ͨ����Dlg���Ҽ�������Ӧ�Ŀؼ�������¼��������õ�
void SetPreferencesDlg::OnCbnSelchangeComboLane()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strlane;
	int nSel;

	//��ȡ��Ͽ�ؼ����б�����������
	nSel=m_combo_lane.GetCurSel();
	//����ѡ����������ȡ�����ַ���
	m_combo_lane.GetLBText(nSel,strlane);

}


void SetPreferencesDlg::OnCbnSelchangeComboArea()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strarea;
	int nSel;

	//��ȡ��Ͽ�ؼ����б�����������
	nSel=m_combox_area.GetCurSel();
	//����ѡ����������ȡ�����ַ���
	m_combox_area.GetLBText(nSel,strarea);
}


void SetPreferencesDlg::OnCbnSelchangeComboSearchbox()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strsearchbox;
	int nSel;

	//��ȡ��Ͽ�ؼ����б�����������
	nSel=m_combox_searchbox.GetCurSel();
	//����ѡ����������ȡ�����ַ���
	m_combox_searchbox.GetLBText(nSel,strsearchbox);
}

//ͨ��Button�������б��еġ��¼��ؼ������
void SetPreferencesDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	char szDir[150];//����һ���ַ������������ļ���

	BROWSEINFO bi;//����һ��BROWSEIFO����
	ITEMIDLIST* pidl;//����һ���ṹָ��
	memset(&bi,0,sizeof BROWSEINFO);
	bi.hwndOwner=m_hWnd;
	bi.hwndOwner=this->m_hWnd;//ָ�������ڣ��ڶԻ�����ʾ�ڼ䣬�����ڽ�������
	 bi.pszDisplayName=szDir;//pszDisplayNmae�����������û�ѡ�е�Ŀ¼�ַ������ڴ��ַ����������
	bi.pidlRoot=NULL;//���ָ��ΪNULL�����ԡ����桱Ϊ����pidRoot��ITEMIDLIST�ṹ�ĵ�ַ�������ʱ�ĳ�ʼ��Ŀ¼
	bi.lpszTitle="��ѡ���ļ�";//��һ�н���ʾ�ڶԻ���Ķ���

	//�������ڵ�ѡ�����״̬�ı�����ʾĿ¼���ļ���ֻ�����ļ�ϵͳĿ¼������ϵͳһ���ڵ�
	bi.ulFlags=BIF_STATUSTEXT|BIF_BROWSEINCLUDEFILES|BIF_RETURNONLYFSDIRS|BIF_RETURNFSANCESTORS;

	bi.lpfn=NULL;//lpfn��Ӧ�ó������������Ի���ص������ĵ�ַ�����Ի����е��¼�����ʱ�������ûص��������ò�������ΪNULL��

	bi.iImage=0;//�Ի��򴫵ݸ��ص�������һ������ָ��
	bi.lParam=0;//��ѡ��Ŀ¼��ص�ͼ�񡣸�ͼ�񽫱�ָ��Ϊϵͳͼ���б��е�����ֵ
	pidl=SHBrowseForFolder(&bi);


	if (SHGetPathFromIDList(pidl,szDir))
	{
		 strFilePath1=szDir;
		 SetDlgItemText(IDC_EDIT_PLATE, strFilePath1);
		//return;
	} 
	//else
//	{
//		SHGetPathFromIDList(pidl,szDir);
//	}
//	m_strFileName=szDir;
	UpdateData(FALSE);

}



void SetPreferencesDlg::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	char szDir[150];//����һ���ַ������������ļ���

	BROWSEINFO bi;//����һ��BROWSEIFO����
	ITEMIDLIST* pidl;//����һ���ṹָ��
	memset(&bi,0,sizeof BROWSEINFO);
	bi.hwndOwner=m_hWnd;
	bi.hwndOwner=this->m_hWnd;//ָ�������ڣ��ڶԻ�����ʾ�ڼ䣬�����ڽ�������
	bi.pszDisplayName=szDir;//pszDisplayNmae�����������û�ѡ�е�Ŀ¼�ַ������ڴ��ַ����������
	bi.pidlRoot=NULL;//���ָ��ΪNULL�����ԡ����桱Ϊ����pidRoot��ITEMIDLIST�ṹ�ĵ�ַ�������ʱ�ĳ�ʼ��Ŀ¼
	bi.lpszTitle="��ѡ���ļ�";//��һ�н���ʾ�ڶԻ���Ķ���

	//�������ڵ�ѡ�����״̬�ı�����ʾĿ¼���ļ���ֻ�����ļ�ϵͳĿ¼������ϵͳһ���ڵ�
	bi.ulFlags=BIF_STATUSTEXT|BIF_BROWSEINCLUDEFILES|BIF_RETURNONLYFSDIRS|BIF_RETURNFSANCESTORS;

	bi.lpfn=NULL;//lpfn��Ӧ�ó������������Ի���ص������ĵ�ַ�����Ի����е��¼�����ʱ�������ûص��������ò�������ΪNULL��

	bi.iImage=0;//�Ի��򴫵ݸ��ص�������һ������ָ��
	bi.lParam=0;//��ѡ��Ŀ¼��ص�ͼ�񡣸�ͼ�񽫱�ָ��Ϊϵͳͼ���б��е�����ֵ
	pidl=SHBrowseForFolder(&bi);


	if (SHGetPathFromIDList(pidl,szDir))
	{
		strFilePath2=szDir;
		SetDlgItemText(IDC_EDIT_RESULT, strFilePath2);
		//return;
	} 
	//else
	//	{
	//		SHGetPathFromIDList(pidl,szDir);
	//	}
	//	m_strFileName=szDir;
	UpdateData(FALSE);
}


void SetPreferencesDlg::OnBnClickedButton4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	char szDir[150];//����һ���ַ������������ļ���

	BROWSEINFO bi;//����һ��BROWSEIFO����
	ITEMIDLIST* pidl;//����һ���ṹָ��
	memset(&bi,0,sizeof BROWSEINFO);
	bi.hwndOwner=m_hWnd;
	bi.hwndOwner=this->m_hWnd;//ָ�������ڣ��ڶԻ�����ʾ�ڼ䣬�����ڽ�������
	bi.pszDisplayName=szDir;//pszDisplayNmae�����������û�ѡ�е�Ŀ¼�ַ������ڴ��ַ����������
	bi.pidlRoot=NULL;//���ָ��ΪNULL�����ԡ����桱Ϊ����pidRoot��ITEMIDLIST�ṹ�ĵ�ַ�������ʱ�ĳ�ʼ��Ŀ¼
	bi.lpszTitle="��ѡ���ļ�";//��һ�н���ʾ�ڶԻ���Ķ���

	//�������ڵ�ѡ�����״̬�ı�����ʾĿ¼���ļ���ֻ�����ļ�ϵͳĿ¼������ϵͳһ���ڵ�
	bi.ulFlags=BIF_STATUSTEXT|BIF_BROWSEINCLUDEFILES|BIF_RETURNONLYFSDIRS|BIF_RETURNFSANCESTORS;

	bi.lpfn=NULL;//lpfn��Ӧ�ó������������Ի���ص������ĵ�ַ�����Ի����е��¼�����ʱ�������ûص��������ò�������ΪNULL��

	bi.iImage=0;//�Ի��򴫵ݸ��ص�������һ������ָ��
	bi.lParam=0;//��ѡ��Ŀ¼��ص�ͼ�񡣸�ͼ�񽫱�ָ��Ϊϵͳͼ���б��е�����ֵ
	pidl=SHBrowseForFolder(&bi);


	if (SHGetPathFromIDList(pidl,szDir))
	{
		strFilePath3=szDir;
		SetDlgItemText(IDC_EDIT_CUTPIC, strFilePath3);
		//return;
	} 
	//else
	//	{
	//		SHGetPathFromIDList(pidl,szDir);
	//	}
	//	m_strFileName=szDir;
	UpdateData(FALSE);
}


void SetPreferencesDlg::OnBnClickedOk()
{

	//ComBox����д��
	int laneIndex = m_combo_lane.GetCurSel();
	m_combo_lane.GetLBText(laneIndex,laneText);

	int areaIndex=m_combox_area.GetCurSel();	
	m_combox_area.GetLBText(areaIndex,areaText);

	int searchboxIndex =m_combox_searchbox.GetCurSel();
	m_combox_searchbox.GetLBText(searchboxIndex,searchboxText);

	//CStrilBar����д��
	m_sprpiexledit.GetWindowText(sprpiexlText);
	m_zoomedit.GetWindowText(zoomText);
	m_thresholdedit.GetWindowText(thresholdText);
	m_nocaredit.GetWindowText(nocarpiexlText);
	m_paltethrestholdedit.GetWindowText(platethresholdText);

	//�Զ���·������
	m_platepathedit.GetWindowText(platepathText);
	m_resultpathedit.GetWindowText(resultpathText);
	m_cutpicedit.GetWindowText(cutpicpathText);

	
	ofstream ouf2;
 	ouf2.open("./out.txt",ios::out);
 	ouf2<<laneText<<"\r\n"<<areaText<<"\r\n"<<searchboxText<<"\r\n"<<sprpiexlText<<"\r\n"<<zoomText<<"\r\n"<<thresholdText<<"\r\n"<<nocarpiexlText<<"\r\n"
		<<platethresholdText<<"\r\n"<<platepathText<<"\r\n"<<resultpathText<<"\r\n"<<cutpicpathText;
 	//ouf2<<"\r\n";

	/*WritePrivateProfileSection(_T("windows"),_T("load=c:\\winnt\\notepad.exe"),_T("d://cfc.ini"));
	GetPrivateProfileIntW("system","type",0,"./config.ini");  */
	CDialog::OnOK();
}


void SetPreferencesDlg::OnBnClickedButton5()
{
	// TODO: ����ǰ���ñ���Ϊ�����ļ�
	CFileDialog fileDlg(FALSE);              //����һ���ļ����Ϊ�Ի���
	fileDlg.m_ofn.lpstrTitle="����";
	fileDlg.m_ofn.lpstrFilter="Text Files(*.txt)\0*.txt\0ALL Files(*.*)\0*.*\0\0";
	fileDlg.m_ofn.lpstrDefExt="txt";		  //����ȱʡ�ļ���չ��
	if(IDOK==fileDlg.DoModal())              //���ļ����Ϊ�Ի���
	{
		////CFile file(fileDlg.GetFileName(),CFile::modeCreate | CFile::modeWrite);           //��д�ķ�ʽ�������ļ�
		//CFile file(fileDlg.GetFileName(),CFile::modeCreate | CFile::modeWrite);           //��д�ķ�ʽ������

		//ComBox����д��
		int laneIndex = m_combo_lane.GetCurSel();
		m_combo_lane.GetLBText(laneIndex,laneText);

		int areaIndex=m_combox_area.GetCurSel();	
		m_combox_area.GetLBText(areaIndex,areaText);

		int searchboxIndex =m_combox_searchbox.GetCurSel();
		m_combox_searchbox.GetLBText(searchboxIndex,searchboxText);

		//CStrilBar����д��
		m_sprpiexledit.GetWindowText(sprpiexlText);
		m_zoomedit.GetWindowText(zoomText);
		m_thresholdedit.GetWindowText(thresholdText);
		m_nocaredit.GetWindowText(nocarpiexlText);
		m_paltethrestholdedit.GetWindowText(platethresholdText);

		//�Զ���·������
		m_platepathedit.GetWindowText(platepathText);
		m_resultpathedit.GetWindowText(resultpathText);
		m_cutpicedit.GetWindowText(cutpicpathText);



		ofstream ouf2;
		string path = fileDlg.GetFolderPath()+"/"+fileDlg.GetFileName();
		ouf2.open(path,ios::out);
		ouf2<<laneText<<"\r\n"<<areaText<<"\r\n"<<searchboxText<<"\r\n"<<sprpiexlText<<"\r\n"<<zoomText<<"\r\n"<<thresholdText<<"\r\n"<<nocarpiexlText<<"\r\n"
			<<platethresholdText<<"\r\n"<<platepathText<<"\r\n"<<resultpathText<<"\r\n"<<cutpicpathText;
		ouf2.close();
		CDialog::OnOK();
	}
	
}


void SetPreferencesDlg::OnBnClickedButton1()
{
	// TODO:
	CFileDialog dlg(
		TRUE,
		_T("*.txt"), 
		NULL,
		OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,
		_T(" All Files (*.*) |*.*||"), 
		NULL
		);	
	dlg.m_ofn.lpstrTitle = _T("ѡȡ�����ļ�");	
	if( dlg.DoModal() != IDOK ){
		 	return;
	}		
	pathVideo2 = dlg.GetPathName();
	SetDlgItemText(IDC_EDIT_PLATE2, pathVideo2);	
	ifstream inf(pathVideo2);

	inf>>inlaneText>>inareaText>>insearchboxText>>insprpiexlText>>inzoomText>>inthresholdText>>innocarpiexlText>>inplatethresholdText
		>>inplatepathText>>inresultpathText>>incutpicpathText;

	inlaneInt = atoi(inlaneText.c_str());
	inareaString=inareaText;
	insearchboxString=insearchboxText;
	insprpiexlInt=atoi(insprpiexlText.c_str());
	inzoomFloat=atof(inzoomText.c_str());
	inthresholdInt=atoi(inthresholdText.c_str());
	innocarpiexlInt=atoi(innocarpiexlText.c_str());
	inplatethresholdInt=atoi(inplatethresholdText.c_str());
	inplatepathString=inplatethresholdText;
	inresultpathString=inresultpathText;
	incutpicpathString=incutpicpathText;



	/**************************���ز�ˮƽ��������ʼ������****************************************/
	//����ˮƽ��������ΧΪ1-100
	m_horiScrollbar.SetRange(1,100);
	//����ˮƽ�������ĳ�ʼλ��Ϊ20
	m_horiScrollbar.SetPos(insprpiexlInt);
	//�ڱ༭������ʾ20
	SetDlgItemInt(IDC_HSCROLL_EDIT,insprpiexlInt);

	/**************************���ű���ˮƽ��������ʼ������****************************************/
	//����ˮƽ��������ΧΪ1-100
	m_horiScrollbar_zoom.SetRange(1,100);
	//����ˮƽ�������ĳ�ʼλ��Ϊ20
	m_horiScrollbar_zoom.SetPos(inzoomFloat);
	//�ڱ༭������ʾ20
	SetDlgItemInt(IDC_HSCROLL_EDIT4,inzoomFloat);

	/**************************��ֵ����ֵ��������ʼ������****************************************/
	//����ˮƽ��������ΧΪ1-100
	m_horiScrollbar_threshold.SetRange(1,100);
	//����ˮƽ�������ĳ�ʼλ��Ϊ20
	m_horiScrollbar_threshold.SetPos(inthresholdInt);
	//�ڱ༭������ʾ20
	SetDlgItemInt(IDC_HSCROLL_EDIT06,inthresholdInt);

	/**************************�޳�����ֵ��������ʼ������****************************************/
	//����ˮƽ��������ΧΪ1-100
	m_horiScrollbar_pixel.SetRange(1,100);
	//����ˮƽ�������ĳ�ʼλ��Ϊ20
	m_horiScrollbar_pixel.SetPos(innocarpiexlInt);
	//�ڱ༭������ʾ20
	SetDlgItemInt(IDC_HSCROLL_EDIT8,innocarpiexlInt);

	/**************************���ƶ�ֵ����ֵ��������ʼ������****************************************/
	//����ˮƽ��������ΧΪ1-100
	m_horiScrollbar_platethros.SetRange(1,100);
	//����ˮƽ�������ĳ�ʼλ��Ϊ20
	m_horiScrollbar_platethros.SetPos(inplatethresholdInt);
	//�ڱ༭������ʾ20
	SetDlgItemInt(IDC_HSCROLL_EDIT7,inplatethresholdInt);

	/******************************����ѡ��COMBOX��ʼ������***************************************/


	m_combo_lane.InsertString(0,"1");
	m_combo_lane.InsertString(1,"2");
	m_combo_lane.InsertString(2,"3");
	m_combo_lane.InsertString(3,"4");

	LPCSTR inlaneTextLpc = inlaneText.c_str();
	int inlaneIndex = m_combo_lane.FindString(-1,inlaneTextLpc);
	m_combo_lane.SetCurSel(inlaneIndex);
	//m_combo_lane.Text

	/******************************����ѡ��ComBox��ʼ������***************************************/
	m_combox_area.InsertString(0,"��");
	m_combox_area.InsertString(1,"��");
	m_combox_area.InsertString(2,"��");
	m_combox_area.InsertString(3,"��");
	m_combox_area.InsertString(4,"��");
	m_combox_area.InsertString(5,"��");
	m_combox_area.InsertString(6,"��");
	m_combox_area.InsertString(7,"��");

	LPCSTR inareaTextLpc = inareaText.c_str();
	int inareaIndex = m_combox_area.FindString(-1,inareaTextLpc);
	m_combox_area.SetCurSel(inareaIndex);

	/******************************�ֶ�λ�������СCOMBOX��ʼ������***************************************/
	m_combox_searchbox.InsertString(0,"40*10");
	m_combox_searchbox.InsertString(1,"60*15");
	m_combox_searchbox.InsertString(2,"80*20");
	m_combox_searchbox.InsertString(3,"100*25");
	m_combox_searchbox.InsertString(4,"120*30");
	m_combox_searchbox.InsertString(5,"140*35");

	LPCSTR insearchboxTextLpc = insearchboxText.c_str();
	int insearchboxIndex = m_combox_searchbox.FindString(-1,insearchboxTextLpc);
	m_combox_searchbox.SetCurSel(insearchboxIndex);
}

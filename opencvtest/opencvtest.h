
// opencvtest.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CopencvtestApp:
// �йش����ʵ�֣������ opencvtest.cpp
//

class CopencvtestApp : public CWinApp
{
public:
	CopencvtestApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CopencvtestApp theApp;
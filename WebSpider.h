
// WebSpider.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CWebSpiderApp:
// �йش����ʵ�֣������ WebSpider.cpp
//

class CWebSpiderApp : public CWinApp
{
public:
	CWebSpiderApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CWebSpiderApp theApp;
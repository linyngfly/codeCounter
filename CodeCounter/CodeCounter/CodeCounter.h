
// CodeCounter.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CCodeCounterApp:
// �йش����ʵ�֣������ CodeCounter.cpp
//

class CCodeCounterApp : public CWinApp
{
public:
	CCodeCounterApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CCodeCounterApp theApp;
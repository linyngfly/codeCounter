/*******************************************************************
** �ļ���:	TreadManage.cpp
** ������:	�Ľ���
** ��  ��:	2015-06-27  18:55
** ��  ��:	1.0
** ��  ��:	�򵥽��̹�����
** Ӧ  ��: 
**************************** �޸ļ�¼ ******************************
** �޸���: 
** ��  ��: 
** ��  ��: 
********************************************************************/

#include "StdAfx.h"
#include "ThreadManage.h"
#include "../FileCounter.h"

#include "Thread.h"


ThreadManage::ThreadManage(void)
{
}

ThreadManage::~ThreadManage(void)
{
}

//////////////////////////////////////////////
//		��ȡ�̳߳�
//////////////////////////////////////////////
rkt::ThreadPool* ThreadManage::getThreadPool()
{
	return &m_threadPool;
}

//////////////////////////////////////////////
//		�����߳�
//////////////////////////////////////////////
void ThreadManage::ThreadStart()
{
	m_threadPool.start();
}

//////////////////////////////////////////////
//		��ֹ�߳�
//////////////////////////////////////////////
void ThreadManage::ThreadTerminate()
{
	m_threadPool.clear();
}

//////////////////////////////////////////////
//		�̴߳���
//////////////////////////////////////////////
void ThreadManage::CounterThreadCreate(int nNumCPU, CCodeCounterMana * pMana, bool (*ConuterEndCallBack)(CCodeCounterMana * pMana, 
	stFileMsg sFileMg, string &strPath, stCounterConfig &stConfig))
{	
	for (int i=0; i<nNumCPU; ++i)
	{
		CFileCounter * p = new CFileCounter(pMana, ConuterEndCallBack);
		getThreadPool()->add(p);
		m_vctCounterThread.push_back(p);		
	}
}

//////////////////////////////////////////////
//		���������߳�
//////////////////////////////////////////////
void ThreadManage::CounterThreadClose()
{
	for (IRUNNABLEVECTOR::iterator iter = m_vctCounterThread.begin(); iter != m_vctCounterThread.end(); ++iter)
	{ 
		CFileCounter* p = (CFileCounter*)(*iter);
		if (p)
		{
			p->Close();
		}
	}
	m_vctCounterThread.clear();
}

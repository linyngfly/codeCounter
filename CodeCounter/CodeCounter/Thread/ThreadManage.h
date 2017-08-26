/*******************************************************************
** �ļ���:	TreadManage.h
** ������:	�Ľ���
** ��  ��:	2015-06-27  18:55
** ��  ��:	1.0
** ��  ��:	���̹߳�����
** Ӧ  ��: 
**************************** �޸ļ�¼ ******************************
** �޸���: 
** ��  ��: 
** ��  ��: 
********************************************************************/

#pragma once

#include <vector>
#include "Thread.h"
#include "../CounterDef.h"

class CFileCounter;
class CCodeCounterMana;

// �̹߳�����
class ThreadManage
{
public:
	typedef std::vector<::rkt::IRunnable*>	IRUNNABLEVECTOR;

public:
	ThreadManage(void);
	~ThreadManage(void);

	// ��ȡ�̳߳�
	rkt::ThreadPool* getThreadPool();
	// �����߳�
	void ThreadStart();
	// ��ֹ�߳�
	void ThreadTerminate();

	// �̴߳���
	void CounterThreadCreate(int nNumCPU, CCodeCounterMana * pMana, bool (*ConuterEndCallBack)(CCodeCounterMana * pMana, 
		stFileMsg sFileMg, string &strPath, stCounterConfig &stConfig));
	// ���������߳�
	void CounterThreadClose();	

private:
	// �̳߳�
	rkt::ThreadPool m_threadPool;
	// �߳̾���б�
	IRUNNABLEVECTOR m_vctCounterThread;
};


/*******************************************************************
** �ļ���:	CodeCounterMana.h
** ������:	�Ľ���
** ��  ��:	2015-06-27  11:02
** ��  ��:	1.0
** ��  ��:	����ͳ�ƹ����ࣨ����ģʽ��
** Ӧ  ��:  	
**************************** �޸ļ�¼ ******************************
** �޸���: 
** ��  ��: 
** ��  ��: 
********************************************************************/
#pragma once

#include "CounterDef.h"
#include "Singleton.h"
#include "Thread/ringQueue.h"
#include "Thread/ThreadManage.h"

using namespace rkt;

/*	ͳ�ƹ����� ԭ��˵����
1������������������̰߳�ȫ���У��ȴ��������WaitQueue��������������DoneQueue
2���ⲿ����AddTask������������Ϣ��WaitQueue����
3���ⲿ����Start������ݵ�ǰ����CPU�����������������߳�������ʼ���߳�����
4��ÿ���̵߳���ConuterEndCallBack�����ϴ�����������Ϣ�������䱣�浽DoneQueue��
   ͬʱ�����һ�������ݡ��Դ�ѭ����ֱ�������������Ϊֹ
5���ⲿ��ʱ����GetDoneTaskData����������ȡ����DoneQueue���Ѵ��������
6���������ݴ�����ɺ��ⲿ����End��ֹ�����½��߳�

ps: ���̰߳�ȫ�����ǿ��Զ����ݼ��ڴ���յĶ���
///////////////////////////////////////////////////////////////////////////////*/

class CCodeCounterMana : public SingletonEx<CCodeCounterMana>
{
public:
	CCodeCounterMana() :m_nLeftNum(0)
	{}
	~CCodeCounterMana()
	{}
	// ����������������
	void	AddTask(string filePath, stCounterConfig stConfig);
	// ��ʼ���̴߳�������
	bool	Start();
	// ��ֹ���߳�����
	void	End();
	// �����������Ƿ���ȫ���������
	bool	bFinished();
	// ��ȡ������ϵ���������
	void	GetDoneTaskData(vector<stTaskDone> &vctTaskDone);
	// �������ݻص��ӿ�
	static bool	ConuterEndCallBack(CCodeCounterMana * pMana, stFileMsg sFileMg, 
		string &strPath, stCounterConfig &stConfig);

public:
	ThreadManage			m_CTreadManage;			// ���̹߳�����
	RingQueue<stTask>		m_queueListWait;		// �ȴ���ɵ������б�
	RingQueue<stTaskDone>	m_queueListDone;		// ����ɵ������б�
	atomic_m<int>			m_nLeftNum;				// ʣ���������
};

#include "stdafx.h"
#include "CodeCounterMana.h"

#define MIN_PROCESSOR_NUM		1
#define MAX_PROCESSOR_NUM		32

//////////////////////////////////////////////
//		���������������ݵ����������
//////////////////////////////////////////////
void CCodeCounterMana::AddTask(string filePath, stCounterConfig stConfig)
{	
	if (!stConfig.bCheakConfig() || filePath.size() == 0)
	{
		return;
	}
	bool bAdd = m_queueListWait.spin_push(stTask(filePath, stConfig));
	if (bAdd)
	{
		m_nLeftNum.AddValue(1);
	}	
}

//////////////////////////////////////////////
//		��ʼ���̴߳�������
//////////////////////////////////////////////
bool CCodeCounterMana::Start()
{
	if (m_queueListWait.sizes() > 0)
	{
		// ��ȡCPU������n���������n�������̲߳�������
		SYSTEM_INFO info;
		GetSystemInfo(&info);
		int nNewPro = min(MAX_PROCESSOR_NUM, max(MIN_PROCESSOR_NUM, info.dwNumberOfProcessors));
		m_CTreadManage.CounterThreadCreate(nNewPro, this, ConuterEndCallBack);
		m_CTreadManage.ThreadStart();
		return true;
	}
	return false;
}

//////////////////////////////////////////////
//		��ֹ���߳�����
//////////////////////////////////////////////
void CCodeCounterMana::End()
{
	m_CTreadManage.CounterThreadClose();
	m_CTreadManage.ThreadTerminate();
}

//////////////////////////////////////////////
//		�����������Ƿ���ȫ���������
//////////////////////////////////////////////
bool CCodeCounterMana::bFinished()
{
	return m_nLeftNum.GetValue() == 0 && m_queueListDone.sizes() == 0;
}

//////////////////////////////////////////////
//		��ȡ������ϵ���������
//////////////////////////////////////////////
void CCodeCounterMana::GetDoneTaskData(vector<stTaskDone> &vctTaskDone)
{
	vctTaskDone.clear();
	stTaskDone stDoneTemp;
	while (m_queueListDone.spin_pop(stDoneTemp))
	{
		vctTaskDone.push_back(stDoneTemp);
	}
}

//////////////////////////////////////////////
// �������ݻص��ӿ�
//param@ pMana��	����ָ��
//param@ sFileMg��	�ļ��������������ϴδ�����ļ������
//param@ strPath��	�ļ�·���������ϴδ����ļ�·�����Ҵ�����һ����Ҫ������ļ�·����
//param@ stConfig��	�ļ���Ӧ���ã�strPath��Ӧ�ļ���ͳ�����ã�
//return bool		�Ƿ�ص��ɹ�
//////////////////////////////////////////////
bool CCodeCounterMana::ConuterEndCallBack(CCodeCounterMana * pMana, stFileMsg sFileMg, 
	string &strPath, stCounterConfig &stConfig)
{
	if (pMana == NULL)
	{
		return false;
	}
	if (strPath.size() > 0)
	{
		stTaskDone stDoneTemp(strPath, sFileMg);
		pMana->m_queueListDone.spin_push(stDoneTemp);
	}	
	stTask stTaskTemp;
	if (pMana->m_queueListWait.spin_pop(stTaskTemp))
	{
		strPath = stTaskTemp.strFilePath;
		stConfig = stTaskTemp.stConfig;	
		pMana->m_nLeftNum.AddValue(-1);
		return true;
	}
	return false;
}
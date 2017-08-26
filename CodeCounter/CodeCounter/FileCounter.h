/*******************************************************************
** �ļ���:	FileCounter.h
** ������:	�Ľ���
** ��  ��:	2015-06-27  11:02
** ��  ��:	1.0
** ��  ��:	�ļ�����ͳ��ͷ�ļ�
** Ӧ  ��:  	
**************************** �޸ļ�¼ ******************************
** �޸���: 
** ��  ��: 
** ��  ��: 
********************************************************************/

/*
�㷨˵����
����ע�ͷ���a
����ע�Ϳ�����: b
����ע����ֹ����e
��Ҫ�������ڸ����裺
1����ȡ�ļ�һ�����ݣ�ȥ��β�ո�TAB�ַ�
2�������ǰ��Ϊ�գ������Ϊ���У�ת����8
3������ǿ����Ƚ�����ת�����\"��\'�ֱ�ת����Ƨ�ַ���͡�������Ӱ�첽��4����ȷ��
4����""��''�ַ�����ַ���ɾ��
5�����ͬʱ�е���ע�ͷ�a�Ͷ���ע�Ϳ�����b��a��bǰ��
	��λ����ͬ(��lua��--��--[[)�������Ӷ���ע�Ϳ�����b
6���жϵ�ǰ״̬
	6.1 ��ע��״̬�����Ҷ���ע�Ϳ�����b
		6.1.1 �ҵ�������b,�����ע��״̬,���Ҷ���ע����ֹ��e
			6.1.1 �ҵ���ֹ��e����ɾ��������b����ֹ��e�м�������ַ���
				  �˳�ע��״̬��תΪ����5
			6.1.2 û�ҵ���ֹ��e�����ж��Ƿ��д����С�ע���У���������7
		6.1.2 û�ҵ�������b�����жϵ�ǰ���Ƿ��д����С�ע���У���������7
	6.2 ע��״̬�����Ҷ���ע����ֹ��e
		6.2.1 �ҵ���ֹ��e����ɾ��������b����ֹ��e�м�������ַ���
			  �˳�ע��״̬��תΪ����5
		6.2.2 û�ҵ���ֹ��e�����ж��Ƿ��д����С�ע���У���������7
7�������ϲ��裬ͳ�Ƶ�ǰ���Ƿ��д����С�ע����
8�������жϽ���,������һ��
///////////////////////////////////////////////////////////////////*/

#pragma once

#include "CounterDef.h"
#include "Thread\Thread.h"

class CCodeCounterMana;

class CFileCounter:public ::rkt::IRunnable
{
public:
	CFileCounter();
	CFileCounter(CCodeCounterMana * pMana, bool (*ConuterEndCallBack)(CCodeCounterMana * pMana, 
		stFileMsg sFileMg, string &strPath, stCounterConfig &stConfig));
	~CFileCounter(void);	

public:
	// �ַ�UTF-8ת�ɶ��ֽ�
	bool	UTF8ToMB(vector<char>& pmb, const char* pu8, int utf8Len);

	// �ļ�ͳ�ƺ���
	// param@ lpPath:	�ļ�·��
	// param@ stConfig:	�ļ�ͳ������
	// param@ sFileMg:	�ļ�ͳ�ƽ���������
	// return: bool		�Ƿ�ͳ�Ƴɹ�
	bool	GetFileMsg(const char *lpPath, stCounterConfig stConfig, stFileMsg &sFileMg);

	// �����߳�
	void	Close();	

	/////////// IRunnable ///////////////////////////
	virtual void release();
	virtual void run();

private:
	// �ļ�·��
	string				m_strPath;

	// �ļ�ͳ���������
	stCounterConfig		m_stConfig;

	// ����ͳ�ƹ�����ָ�루�ص��ã�
	CCodeCounterMana *  m_pMana;

	// �ص�����ָ�����
	bool (*m_ConuterEndCallBack)(CCodeCounterMana * pMana, stFileMsg sFileMg, 
		string &strPath, stCounterConfig &stConfig);
};
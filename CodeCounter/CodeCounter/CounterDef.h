/*******************************************************************
** �ļ���:	CounterDef.h
** ������:	�Ľ���
** ��  ��:	2015-06-27  11:02
** ��  ��:	1.0
** ��  ��:	Դ����ͳ��ͷ�ļ�����
** Ӧ  ��:  	
	
**************************** �޸ļ�¼ ******************************
** �޸���: 
** ��  ��: 
** ��  ��: 
********************************************************************/
#pragma once

#include <string.h>
#include <vector>

using namespace std;

#define INI_FILE_NAME	"D:\\CodeCounterConfig.ini"
#define CODE_TYPE_NAME_MAX		10
#define CODE_TYPE_NUM_MAX		100

#define deleteSafe(x)	delete []x; x=NULL;

// ͳ�Ʒ�Χö��
enum EM_COUTER_AREA
{
	CODE_COUNTER_FOLDER	= 0,	// �ļ���
	CODE_COUNTER_FILE,			// �ļ�
	CODE_COUNTER_MAX,			// ���
};

// �ļ������������
struct stFileMsg
{
	int nAllCount;			// ������
	int nNullCount;			// ��������
	int nNoteCount;			// ע������
	int nCodeCount;			// ��Ч��������
	int nNodeCodeCount;		// ����&ע������

	stFileMsg()
	{
		clear();
	}	
	stFileMsg(const stFileMsg &stTemp)
	{
		nAllCount = stTemp.nAllCount;
		nNullCount = stTemp.nNullCount;
		nNoteCount = stTemp.nNoteCount;
		nCodeCount = stTemp.nCodeCount;
		nNodeCodeCount = stTemp.nNodeCodeCount;
	}
	void clear()
	{
		memset(this, 0, sizeof(*this));
	}
};

// ��˫ע�ͷ�
struct stPairChars
{
	string strBegin;	// ��ʼע�ͷ�
	string strEnd;		// ����ע�ͷ�
	stPairChars()
	{
		strBegin = "";
		strEnd = "";
	}
	bool bCheakConfig()
	{
		return strBegin.size() > 0 && strEnd.size() > 0;
	}
};

// ע�ͷ�����
struct stCounterConfig
{
	string		strNote;			// ����ע�ͷ�
	stPairChars strPairNote;		// ����ע�ͷ�

	stCounterConfig()
	{
		strNote = "";
	}	
	stCounterConfig(const stCounterConfig &stTemp)
	{
		strNote = stTemp.strNote;
		strPairNote.strBegin = stTemp.strPairNote.strBegin;
		strPairNote.strEnd = stTemp.strPairNote.strEnd;
	}
	bool bCheakConfig()
	{
		return strNote.size() > 0 && strPairNote.bCheakConfig();
	}
};

// ��������
struct stTask
{
	string strFilePath;
	stCounterConfig stConfig;
	stTask()
	{
		strFilePath = "";
	}
	stTask(const stTask &stTemp)
	{
		strFilePath = stTemp.strFilePath;
		stConfig = stTemp.stConfig;
	}
	stTask(string filePath, stCounterConfig config)
	{
		strFilePath = filePath;
		stConfig = config;
	}
};

// ����ɵ���������
struct stTaskDone
{
	string		strFilePath;
	stFileMsg	sFileMsg;

	stTaskDone()
	{
	}
	stTaskDone(string filePath, stFileMsg fileMsg)
	{
		strFilePath = filePath;
		sFileMsg	= fileMsg;
	}
};
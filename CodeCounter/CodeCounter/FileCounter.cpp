/*******************************************************************
** �ļ���:	FileCounter.cpp
** ������:	�Ľ���
** ��  ��:	2015-06-27
** ��  ��:	1.0
** ��  ��:				
********************************************************************/

#include "StdAfx.h"
#include "FileCounter.h"
#include "CodeCounterMana.h"
#include <afx.h>

CFileCounter::CFileCounter()
{
}

CFileCounter::CFileCounter(CCodeCounterMana * pMana, bool (*ConuterEndCallBack)(CCodeCounterMana * pMana,	
	stFileMsg sFileMg, string &strPath, stCounterConfig &stConfig))
{
	m_pMana = pMana;
	m_ConuterEndCallBack = ConuterEndCallBack;
}

CFileCounter::~CFileCounter()
{
}

//////////////////////////////////////////////
//		�����߳�
//////////////////////////////////////////////
void CFileCounter::Close()
{
	release();
	delete this;
}

//////////////////////////////////////////////
//		�������
//////////////////////////////////////////////
void CFileCounter::release()
{
	m_strPath = "";
	m_stConfig.strNote = "";
	m_stConfig.strPairNote.strBegin = "";
	m_stConfig.strPairNote.strEnd = "";
}

//////////////////////////////////////////////
//		�����߳�
//////////////////////////////////////////////
void CFileCounter::run()
{
	stFileMsg sFileMg;
	while (m_ConuterEndCallBack(m_pMana, sFileMg, m_strPath, m_stConfig))
	{
		sFileMg.clear();
		GetFileMsg(m_strPath.c_str(), m_stConfig, sFileMg);
	}
}

//////////////////////////////////////////////
//	UTF8����תΪ���ֽڱ���
//////////////////////////////////////////////
bool CFileCounter::UTF8ToMB(vector<char>& pmb, const char* pu8, int utf8Len)  
{  
	// convert an UTF8 string to widechar   
	int nLen = MultiByteToWideChar(CP_UTF8, 0, pu8, utf8Len, NULL, 0);  

	WCHAR* lpszW = NULL;  
	try  
	{  
		lpszW = new WCHAR[nLen];  
	}  
	catch(bad_alloc)  
	{  
		return false;  
	}  

	int nRtn = MultiByteToWideChar(CP_UTF8, 0, pu8, utf8Len, lpszW, nLen);  

	if(nRtn != nLen)  
	{  
		delete[] lpszW;  
		return false;  
	}  

	// convert an widechar string to Multibyte   
	int MBLen = WideCharToMultiByte(CP_ACP, 0, lpszW, nLen, NULL, 0, NULL, NULL);  
	if (MBLen <=0)  
	{  
		return false;  
	}  
	pmb.resize(MBLen);  
	nRtn = WideCharToMultiByte(CP_ACP, 0, lpszW, nLen, &*pmb.begin(), MBLen, NULL, NULL);  
	delete[] lpszW;  

	if(nRtn != MBLen)  
	{  
		pmb.clear();  
		return false;  
	}  
	return true;  
}  

//////////////////////////////////////////////
//	ͳ���ļ������Ϣ
// param@ lpPath:	�ļ�·��
// param@ stConfig:	�ļ�ͳ������
// param@ sFileMg:	�ļ�ͳ�ƽ���������
// return: bool		�Ƿ�ͳ�Ƴɹ�			
//////////////////////////////////////////////
bool CFileCounter::GetFileMsg(const char *lpPath, stCounterConfig stConfig, stFileMsg &sFileMg)
{
	if (lpPath == NULL || !stConfig.bCheakConfig())
	{
		return false;
	}
	CStdioFile myFile;
	if(myFile.Open(lpPath, CFile::typeText|CFile::modeRead))
	{
		myFile.SeekToBegin();
		CString str;
		// �Ƿ��ڶ���ע��״̬��
		bool bNoteBegined = false;
		bool bBegin = true;
		while (myFile.ReadString(str))
		{
			if (bBegin)
			{
				vector<char> vctTemp;
				UTF8ToMB(vctTemp, (LPSTR)(LPCTSTR)str, str.GetLength());
				bBegin = false;
				if (vctTemp.size() > 0)
				{
					str = (char *)&vctTemp[0];
				}	
			}
			++sFileMg.nAllCount;			
			// ȥ�ո�TAB�ַ���Ҳ�ɼ������������ã�
			str.TrimLeft(_T("\t "));
			str.TrimRight(_T("\t "));
			if (str.GetLength() == 0)
			{
				++sFileMg.nNullCount;
			}
			else
			{
				bool bNoteLine = false;		// ��¼�����Ƿ���ע�Ͷ�
				bool bCodeLine = false;		// ��¼�����Ƿ��д����
				bool bDeleted = false;		// ��¼�ַ����Ƿ���ɾ��һ����
				do 
				{		
					bDeleted = false;
					str.Replace("\\\"", "��");
					str.Replace("\\\'", "��");
					// ȥ��""�ַ���					
					int nDelBegin = str.Find("\"");		// ע��ת���ַ�\'��\"
					int nDelEnd = -1;
					if (nDelBegin >= 0)
					{
						nDelEnd = str.Find("\"", nDelBegin+1);
						if (nDelEnd > nDelBegin)
						{
							str.Delete(nDelBegin, nDelEnd-nDelBegin+1);
						}
					}
					// ȥ��''�ַ���					
					nDelBegin = str.Find("\'");		// ע��ת���ַ�\'��\"
					nDelEnd = -1;
					if (nDelBegin >= 0)
					{
						nDelEnd = str.Find("\'", nDelBegin+1);
						if (nDelEnd > nDelBegin)
						{
							str.Delete(nDelBegin, nDelEnd-nDelBegin+1);
						}
					}
					
					// ��ǰ�Ĵ���û�п�������ע��
					if (!bNoteBegined)
					{
						// ���Ҷ���ע�Ϳ�����������ע�ͷ�
						int nBeginIndex = str.Find(stConfig.strPairNote.strBegin.c_str());
						int nNoteIndex = str.Find(stConfig.strNote.c_str());
						// ����ע�ͷ��ڶ���ע�ͷ�ǰ�棬�����ע�ͷ���Ч
						if (nNoteIndex >= 0 && nNoteIndex < nBeginIndex)
						{
							if (nNoteIndex > 0)
							{
								bCodeLine = true;
							}							
							bNoteLine = true;						
						}
						else
						{												
							if (nBeginIndex >=0)		// ��������ע��״̬
							{													
								bNoteBegined = true;								
								bNoteLine = true;
								if (nBeginIndex != 0)	// ���ִ����
								{
									bCodeLine = true;
								}
								// ���Ҷ�����ֹ��
								int nEndIndex = str.Find(stConfig.strPairNote.strEnd.c_str());
								// �ڱ��з�����ֹ�����뿪ʼ��û���ص�����ر�ע��״̬
								if (nEndIndex >=0 && nEndIndex-nBeginIndex >= (int)stConfig.strPairNote.strBegin.size())
								{
									str = str.Mid(nEndIndex + stConfig.strPairNote.strEnd.size());
									bDeleted = true;
									bNoteBegined = false;
									str.TrimLeft(_T("\t "));
								}
							}
							else if (nNoteIndex >=0)	// ���ֵ���ע��
							{
								bNoteLine = true;
								if (nNoteIndex != 0)		// ���ִ����
								{
									bCodeLine = true;
								}
							}
							else
							{
								bCodeLine = true;
							}
						}								
					}
					else	// ��ǰ����ע�ʹ���״̬
					{
						bNoteLine = true;
						// ���Ҷ���ע����ֹ��������ע�ͷ�
						int nEndIndex = str.Find(stConfig.strPairNote.strEnd.c_str());
						int nNoteIndex = str.Find(stConfig.strNote.c_str());
						if (nEndIndex >=0)
						{
							str = str.Mid(nEndIndex + stConfig.strPairNote.strEnd.size());
							bDeleted = true;
							bNoteBegined = false;
							str.TrimLeft(_T("\t "));
						}
					}
				} while (str.GetLength() != 0 && bDeleted );			
				if (bNoteLine)
				{
					++sFileMg.nNoteCount;
				}
				if (bCodeLine)
				{
					++sFileMg.nCodeCount;
				}	
				if (bNoteLine && bCodeLine)
				{
					++sFileMg.nNodeCodeCount;
				}
			}
		}		
	}
	else
	{
		myFile.Close();
		return false;
	}
	myFile.Close();
	return true;
}
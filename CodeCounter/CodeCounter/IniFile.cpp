// m_IniFile.cpp: implementation of the CIniFile class. 
// 
//////////////////////////////////////////////////////////////////////

#include "stdafx.h" 
#include "IniFile.h"

#ifdef _DEBUG 
#undef THIS_FILE 
static char THIS_FILE[]=__FILE__; 
#define new DEBUG_NEW 
#endif

////////////////////////////////////////////////////////////////////// 
// Construction/Destruction 
//////////////////////////////////////////////////////////////////////

CIniFile::CIniFile() 
{ 
	m_sFileName = "ini.ini"; 
}

CIniFile::~CIniFile() 
{ 
	if(m_FileContainer.GetSize() > 0) 
	{ 
		m_FileContainer.RemoveAll(); 
	} 
}

void CIniFile::SetFileName(CString FileName) 
{ 
	m_sFileName = FileName; 
}

void CIniFile::SetInt(CString Section, CString Item, int Value) 
{ 
	CString strtemp; 
	//  strtemp.Format("%d",Value); 
	//���浽m_FileContainer 
	WriteFileString(Section, Item, strtemp); 
	//���浽ini�ļ� 
	WriteIniFile(); 
}

void CIniFile::SetString(CString Section, CString Item, CString Value) 
{ 
	//���浽m_FileContainer 
	WriteFileString(Section, Item, Value); 
	//���浽ini�ļ� 
	WriteIniFile(); 
}

CString CIniFile::GetString(CString Section, CString Item, CString Value) 
{ 
	return GetFileString(Section, Item, Value); 
}

int CIniFile::GetInt(CString Section, CString Item, int Value) 
{ 
	CString strtemp; 
	strtemp.Format("%d",Value); 
	return atoi(GetFileString(Section, Item, strtemp)); 
	//return GetFileString(Section, Item, strtemp); 
}

BOOL CIniFile::ReadIniFile() 
{ 
	m_bFileExist = m_IniFile.Open(m_sFileName, CFile::modeRead);

	if(!m_bFileExist) 
	{ 
		return FALSE; 
	}

	CString strline; 
	m_FileContainer.RemoveAll();

	//��m_IniFile�ļ����ݶ���m_FileContainer 
	while(m_IniFile.ReadString(strline)) 
	{ 
		m_FileContainer.Add(strline); 
	}

	m_IniFile.Close(); 
	return TRUE; 
}

BOOL CIniFile::WriteIniFile() 
{ 
	m_bFileExist=m_IniFile.Open(m_sFileName, CFile::modeCreate | CFile::modeWrite); 
	if(!m_bFileExist) 
		return FALSE; 

	CString strline;

	//��m_FileContainerд��m_IniFile�ļ� 
	for(int i = 0; i< m_FileContainer.GetSize(); i++) 
	{ 
		strline = m_FileContainer[i]; 
		m_IniFile.WriteString(strline+"/n"); 
	}

	m_IniFile.Close();

	return m_bFileExist; 
}

CString CIniFile::GetFileString(CString Section, CString Item, CString Value) 
{ 
	ReadIniFile();//���ļ�

	if(m_bFileExist == FALSE || m_FileContainer.GetSize() < 0) 
		return Value;//�ļ��򿪳�����ļ�Ϊ�գ�����Ĭ��ֵ

	int i = 0; 
	int iFileLines = m_FileContainer.GetSize(); 
	CString strline,str;

	while(i<iFileLines) 
	{ 
		strline = m_FileContainer.GetAt(i++);

		strline.TrimLeft(); 
		if(strline.GetAt(0)=='[')//����Section����һ������Ϊ[ 
		{ 
			str=strline.Left(strline.Find("]"));//ȥ��]�ұ� 
			str=str.Right(str.GetLength()-str.Find("[")-1);//ȥ��[���

			str.TrimLeft(); 
			str.TrimRight();

			if(Section == str)//�ҵ�Section 
			{ 
				while(i<iFileLines) 
				{ 
					strline = m_FileContainer.GetAt(i++); 
					strline.TrimLeft();

					if(strline.GetAt(0)=='[') 
						return Value;//���������һ��[]�����Ҳ���,����Ĭ��ֵ 

					str = strline.Left(strline.Find("="));//ȥ��=�ұ�

					str.TrimLeft(); 
					str.TrimRight();

					if(Item == str)//�ҵ�Item 
					{ 
						str=strline.Right(strline.GetLength()-strline.Find("=")-1);//ȥ��=���

						str.TrimLeft(); 
						str.TrimRight();

						return str; 
					} 
				} 
				return Value;//�Ҳ���,����Ĭ��ֵ 
			}

		}

	} 
	return Value;//�Ҳ���,����Ĭ��ֵ 
}

void CIniFile::WriteFileString(CString Section, CString Item, CString Value) 
{ 
	int i = 0; 
	int iFileLines = m_FileContainer.GetSize(); 
	CString strline,str;

	while(i<iFileLines) 
	{ 
		strline = m_FileContainer.GetAt(i++);

		strline.TrimLeft(); 
		if(strline.GetAt(0)=='[')//����Section����һ������Ϊ[ 
		{ 
			str=strline.Left(strline.Find("]"));//ȥ��]�ұ� 
			str=str.Right(str.GetLength()-str.Find("[")-1);//ȥ��[���

			str.TrimLeft(); 
			str.TrimRight();

			if(Section == str)//�ҵ�Section 
			{ 
				while(i<iFileLines) 
				{ 
					strline = m_FileContainer.GetAt(i++); 
					strline.TrimLeft();

					if(strline.GetAt(0)=='[')//���������һ��[]�����Ҳ���Item 
					{ 
						break; 
					} 

					str = strline.Left(strline.Find("="));//ȥ��=�ұ�

					str.TrimLeft(); 
					str.TrimRight();

					if(Item == str)//�ҵ�Item 
					{ 
						strline = Item + "=" + Value; 
						m_FileContainer[i-1] = strline; 
						return; 
					} 
				} 
				//�Ҳ���Item 
				strline = Item + "=" + Value; 
				m_FileContainer.InsertAt(i-1, strline); 
				return;

			}

		}

	} 
	//�Ҳ���Section 
	//ֱ����������Section��Item��Value 
	m_FileContainer.Add("[" + Section + "]"); 
	m_FileContainer.Add(Item + "=" + Value); 
	return; 
}